#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>
#include <string>

#include "errors.h"

namespace jsonrpc
{
    class JsonRpcException : public std::exception 
    {
        public:
            JsonRpcException( int code );
            JsonRpcException( int code, const std::string &exceptionMessage );
            JsonRpcException( int code, const std::string &exceotionMessage, const Json::Value &data );
            virtual ~JsonRpcException() throw();

            int GetCode() const;
            const std::string &GetMessage() const;
            const Json::Value &GetData() const;
            virtual const char *what() const throw();

        private:
            int code;
            std::string exceptionMessage, whatString;
            Json::Value data;
            void SetWhatMessage();
    };
}

#endif EXCEPTION_H
