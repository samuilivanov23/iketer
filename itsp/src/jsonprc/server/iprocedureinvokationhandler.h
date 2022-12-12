#ifndef IPROCEDUREINVOKATIONHANDLER_H
#define IPROCEDUREINVOKATIONHANDLER_H

namespace Json
{
    class Value;
}

namespace itsp
{
    class Procedure;

    class IProcedureInvokationHandler 
    {
        public:
            virtual ~IProcedureInvokationHandler() {}
            virtual void HandleMethodCall( Procedure &procedure, const Json::Value &input, Json::Value &output ) = 0;
            virtual void HandleNotificationCall( Procedure &procedure, const Json::Value &input ) = 0;
            
    };
}

#endif //IPROCEDUREINVOKATIONHANDLER_H
