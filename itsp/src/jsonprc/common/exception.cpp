#include "exception.h"

using namespace itsp;

JsonRpcException::JsonRpcException( int code )
{
	this->code = code;
	this->exceptionMessage = Errors::GetErrorMessage( code );
	this->SetWhatMessage();
}

JsonRpcException::JsonRpcException( int code, const std::string &exceptionMessage )
{
	this->code = code;
	this->exceptionMessage = Errors::GetErrorMessage( code );

	if( !this->exceptionMessage.empty() ) 
	{ 
		this->exceptionMessage = this->exceptionMessage + ": ";
	}

	this->exceptionMessage += exceptionMessage;

	this->SetWhatMessage();
}

JsonRpcException::JsonRpcException( int code, const std::string &exceptionMessage, const Json::Value &data )
{
	this->code = code;
	this->exceptionMessage = Errors::GetErrorMessage( code );

	if( !this->exceptionMessage.empty() ) 
	{ 
		this->exceptionMessage = this->exceptionMessage + ": ";
	}

	this->exceptionMessage += exceptionMessage;
	this->data = data;

	this->SetWhatMessage();
}

JsonRpcException::JsonRpcException( const std::string &exceptionMessage )
{
	this->code = 0;
	this->exceptionMessage = exceptionMessage;
	this->SetWhatMessage();
}

JsonRpcException::~JsonRpcException() throw() {}

int JsonRpcException::GetCode() const { return this->code; }

const std::string &JsonRpcException::GetMessage() const { return this->exceptionMessage; }

const Json::Value &JsonRpcException::GetData() const { return this->data; }

const char *JsonRpcException::what() const throw() { return this->whatString.c_str(); }

void JsonRpcException::SetWhatMessage() 
{
	if( this->code != 0 )
	{
		std::stringstream stringStream;
		stringStream << "Exception: " << this->code << " : " << this->exceptionMessage;
		
		if( this->data != Json::nullValue )
		{
			stringStream << ", data: " << this->data.toStyledString();
		}

		this->whatString = stringStream.str();
	}
	else
	{
		this->whatString = this->exceptionMessage;
	}
}
