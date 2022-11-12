#ifndef PROCEDURE_H
#define PROCEDURE_H

#include <map>
#include <string>

#include "jsonparser.h"
#include "specification.h"

namespace itsp
{
    typedef std::map<std::string, jsontype_t> parameterNameList_t;
    typedef std::vector<jsontype_t> parameterPositionList_t;
    typedef enum { PARAMS_BY_NAME, PARAMS_BY_POSITION } parameterDeclaration_t;

    class Procedure
    {
        public:
            Procedure();

            //Constructor for notification with parameters as list. Last parameter must be NULL
            //If no parameters are passed, parameters do not exists and can not be checked for type by the protocol functions
            Procedure( const std::string &procedureName, parameterDeclaration_t parameterType, ... );

            //Constructor for method with parameters as list. Last parameter must be NULL
            //If no parameters are passed, parameters do not exists and can not be checked for type by the protocol functions
            Procedure( const std::string &procedureName, parameterDeclaration_t parameterType, jsontype_t returnType, ... );

            //Method for validating the parameters for each procedure  
            bool ValidateParameters() const;

            //Get methods
            const parameterNameList_t &GetProcedureParameters() const;
            procedure_t GetProcedureType() const;
            const std::string &GetProcedureName() const;
            jsontype_t GetProcedureReturnType() const;
            parameterDeclaration_t GetProcedureParameterDeclarationType() const;

            //Set methods
            void SetProcedureName( const std::string &procedureName );
            void SetProcedureType( procedure_t procedureType );
            void SetProcedureReturnType( jsontype_t  procedureReturnType );
            void SetProcedureParameterDeclarationType( parameterDeclaration_t procedureParameterDeclarationType );

            void AddParameter( const std::string &procedureName, jsontype_t parameterType );

            bool ValidateNamedParameters( const Json::Value &parameters ) const;
            bool ValidatePositionalParameters( const Json::Value &parameters ) const;

        private:
            std::string procedureName;
            parameterNameList_t procedureParametersName;
            parameterPositionList_t procedureParametersPosition;
            procedure_t procedureType;
            jsontype_t returnType;
            parameterDeclaration_t procedureParameterDeclarationType;
    };
}

#endif //PROCEDURE_H
