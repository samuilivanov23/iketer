#ifndef SPECIFICATIONPARSER_H
#define SPECIFICATIONPARSER_H

#include <exception.h>
#include <procedure.h>

namespace itsp
{
    class SpecificationParser
    {
        public:
            static std::vector<Procedure> GetProceduresFromFile( const std::string &fileName );
            static std::vector<Procedure> GetProceduresFromString( const std::string &specificationString );
            static void GetFileContent( const std::string &fileName, std::string &target );

        private:
            static void GetProcedure( Json::Value  &value, Procedure &target );
            static void GetMethod( Json::Value &value, Procedure &target );
            static void GetNotification( Json::Value &value, Procedure &target );
            static jsontype_t toJsonType( Json::Value &value );

            static void GetPositionalParameters( Json::Value &value, Procedure &target );
            static void GetNamedParameters( Json::Value &value, Procedure &target );
            static std::string GetProcedureName( Json::Value &procedureSignature );
    };
}

#endif //SPECIFICATIONPARSER_H
