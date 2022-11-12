#ifndef ERRORS_H
#define ERRORS_H

#include <map>
#include <string>

#include "jsonparser.h"

namespace itsp
{
    class JsonRpcException;

    class Errors
    {
        public:
            static std::string GetErrorMessage( int errorCode );

            static class _init 
            {
                public:
                    _init();
            } _initializer;

            //JSON-RPC 2.0 official Errors
            static const int ERROR_RPC_JSON_PARSE_ERROR;
            static const int ERROR_RPC_METHOD_NOT_FOUND;
            static const int ERROR_RPC_INVALID_REQUEST;
            static const int ERROR_RPC_INVALID_PARAMS;
            static const int ERROR_RPC_INTERNAL_ERROR;
    
            //Custom Server Protocol Errors
            static const int ERROR_SERVER_PROCEDURE_IS_METHOD;
            static const int ERROR_SERVER_PROCEDURE_IS_NOTIFICATION;
            static const int ERROR_SERVER_PROCEDURE_POINTER_IS_NULL;
            static const int ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND;
            static const int ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX;
            static const int ERROR_SERVER_CONNECTOR;

            //Custom Client Protocol Errors
            static const int ERROR_CLIENT_CONNECTOR;
            static const int ERROR_CLIENT_INVALID_RESPONSE;

        private:
            static std::map<int, std::string> possibleErrors;
    };
}

#endif //ERRORS_H
