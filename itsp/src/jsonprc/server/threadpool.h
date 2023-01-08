#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

namespace itsp
{
	class ThreadPool 
	{
		public:
			ThreadPool( size_t );
			template <class F, class... Args>
			auto enqueue( F &&f, Args &&...args ) -> std::future<typename std::result_of<F( Args... )>::type>;
			~ThreadPool();

		private:
			// To keep track of threads and join them when needed
			std::vector<std::thread> workers;
			// Keep track of tasks in the queue waiting to be accepted 
			std::queue<std::function<void()>> tasks;

			// Synchronization
			std::mutex queueMutex;
			std::condition_variable condition;
			bool stop;		
	};

	// The constructor launches some amount of workers
	inline ThreadPool::ThreadPool( size_t threads ) : stop( false )
	{
		for( size_t i = 0; i < threads; ++i )
		{
			workers.emplace_back( [this] {
				for( ;; )
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock( this->queueMutex );
						this->condition.wait( lock, [this] { return this->stop || !this->tasks.empty(); } );
						if( this->stop && this->tasks.empty() )
						{
							return;
						}

						task = std::move( this->tasks.front() );
						this->tasks.pop();
					}
					task();
				}
			} );
		}
	}

	// Add new worker to the pool
	template <class F, class... Args>
	auto ThreadPool::enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> 
	{
		using return_type = typename std::result_of<F(Args...)>::type;
		auto task = std::make_shared<std::packaged_task<return_type()>>( std::bind( std::forward<F>( f ), std::forward<Args>( args )... ) );

		std::future<return_type> result = task->get_future();
		{
			std::unique_lock<std::mutex> lock( queueMutex );

			// don't allow enqueueing after stopping the pool
			if (stop)
			{
				throw std::runtime_error( "enqueue on stopped ThreadPool" );
			}
			
			tasks.emplace([task]() { ( *task )(); });
		}
		
		condition.notify_one();
		return result;
	}

	inline ThreadPool::~ThreadPool() 
	{
		{
			std::unique_lock<std::mutex> lock( queueMutex );
			stop = true;
		}
		
		condition.notify_all();
		for ( std::thread &worker : workers )
		{
			worker.join();
		}
	}
}

#endif //THREADPOOL_H
