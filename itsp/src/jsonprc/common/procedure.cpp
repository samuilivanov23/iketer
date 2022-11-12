#include "procedure.h"
#include "errors.h"
#include "exception.h"

#include <cstdarg>
#include <vector>

using namespace std;
using namespace itsp;

Procedure::Procedure() : procedureName(""), procedureType( RPC_METHOD ), procedureReturnType( JSON_BOOLEAN ), procedureParameterDeclarationType( PARAMS_BY_NAME ) {}

Procedure::Procedure( const string &procedureName, parameterDeclaration_t parameterDeclarationType, jsontype_t procedureReturnType, ... )
{
    va_list procedureParameters;
    va_start( procedureParameters, procedureReturnType );
    const char *parameterName = va_arg( procedureParameters, const char* );
    jsontype_t parameterType;
    
    while( parameterName != NULL )
    {
        parameterType = ( jsontype_t )va_arg( procedureParameters, const char* );
        this->AddParameter( parameterName, parameterType );
        parameterName = va_arg( procedureParameters, const char* );
    }

    va_end( procedureParameters );
    this->procedureName = procedureName;
    this->procedureReturnType = procedureReturnType;
    this->procedureType = RPC_METHOD;
    this->procedureParameterDeclarationType = parameterDeclarationType;
}

Procedure::Procedure( const string &procedureName, parameterDeclaration_t parameterDeclarationType, ... )
{
    va_list procedureParameters;
    va_start( procedureParameters, parameterDeclarationType );
    const char *parameterName = va_arg( procedureParameters, const char* );
    jsontype_t parameterType;

    while( parameterName != NULL )
    {
        parameterType = ( jsontype_t )va_arg( procedureParameters, const char* );
        this->AddParameter( parameterName, parameterType );
        parameterName = va_arg( procedureParameters, const char* );
    }

    va_end( procedureParameters );
    this->procedureName = procedureName;
    this->procedureReturnType = JSON_BOOLEAN;
    this->procedureType = RPC_NOTIFICATION;
    this->procedureParameterDeclarationType = parameterDeclarationType;
}

bool Procedure::ValidateParameters( const Json::Value &parameters ) const
{
    if( this->procedureParametersName.empty() ) { return true; }

    if( parameters.isArray() && this->procedureParameterDeclarationType == PARAMS_BY_POSITION )
    {
        return this->ValidatePositionalParameters( parameters );
    }
    else if( parameters.isObject() && this->procedureParameterDeclarationType == PARAMS_BY_NAME )
    {
        return this->ValidateNamedParameters( parameters );
    }
    else { return false; }
}

const parameterNameList_t &Procedure::GetProcedureParameters() { return this->procedureParametersName; }
procedure_t Procedure::GetProcedureType() { return this->procedureType; }
const std::string &Procedure::GetProcedureName() { return this->procedureName; }
jsontype_t Procedure::GetProcedureReturnType() { return this->procedureReturnType; }
parameterDeclaration_t Procedure::GetProcedureParameterDeclarationType() { return this->procedureParameterDeclarationType; }

void Procedure::SetProcedureName( const std::string &procedureName ) { this->procedureName = procedureName; }
void Procedure::SetProcedureType( procedure_t procedureType ) { this->procedureType = procedureType; }
void Procedure::SetReturnType( jsontype_t procedureReturnType ) { this->procedureReturnType = procedureReturnType; }
void Procedure::SetParameterDeclarationType( parameterDeclaration_t procedureParameterDeclarationType ) { this->procedureParameterDeclarationType = procedureParameterDeclarationType; }

void Procedure::AddParameter( const std::string &procedureName, jsontype_t parameterType )
{
    this->procedureParametersName[procedureName] = parameterType;
    this->procedureParametersPosition.push_babk( parameterType );
}


