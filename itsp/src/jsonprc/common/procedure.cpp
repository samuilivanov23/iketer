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
        parameterType = ( jsontype_t )va_arg( procedureParameters, int );
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
        parameterType = ( jsontype_t )va_arg( procedureParameters, int );
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

const parameterNameList_t &Procedure::GetProcedureParameters() const { return this->procedureParametersName; }
procedure_t Procedure::GetProcedureType() const { return this->procedureType; }
const std::string &Procedure::GetProcedureName() const { return this->procedureName; }
jsontype_t Procedure::GetProcedureReturnType() const { return this->procedureReturnType; }
parameterDeclaration_t Procedure::GetProcedureParameterDeclarationType() const { return this->procedureParameterDeclarationType; }

void Procedure::SetProcedureName( const std::string &procedureName ) { this->procedureName = procedureName; }
void Procedure::SetProcedureType( procedure_t procedureType ) { this->procedureType = procedureType; }
void Procedure::SetProcedureReturnType( jsontype_t procedureReturnType ) { this->procedureReturnType = procedureReturnType; }
void Procedure::SetProcedureParameterDeclarationType( parameterDeclaration_t procedureParameterDeclarationType ) { this->procedureParameterDeclarationType = procedureParameterDeclarationType; }

void Procedure::AddParameter( const std::string &procedureName, jsontype_t parameterType )
{
    this->procedureParametersName[procedureName] = parameterType;
    this->procedureParametersPosition.push_back( parameterType );
}

bool Procedure::ValidateNamedParameters( const Json::Value &parameters ) const
{
    bool areParametersValid = parameters.isObject() || parameters.isNull();
    for( map<std::string, jsontype_t>::const_iterator i = this->procedureParametersName.begin(); areParametersValid == true && i != this->procedureParametersName.end(); ++i )
    {
        if( !parameters.isMember( i->first ) )
        {
            areParametersValid = false;
        }
        else
        {
            areParametersValid = this->ValidateSingleParameter( i->second, parameters[i->first] );
        }
    }

    return areParametersValid;
}

bool Procedure::ValidatePositionalParameters( const Json::Value &parameters ) const
{
    bool areParametersValid = true;
    if( parameters.size() != this->procedureParametersPosition.size() ) { return false; }

    for( uint16_t i = 0; i < parameters.size(); i++) 
    {
        areParametersValid = this->ValidateSingleParameter( this->procedureParametersPosition.at(i), parameters[i] );  
    }

    return areParametersValid;
}

bool Procedure::ValidateSingleParameter( jsontype_t expectedParameterType, const Json::Value &parameterValue ) const
{
    bool isParameterValid = true;
    switch( expectedParameterType )
    {
        case JSON_INTEGER:  if( !parameterValue.isInt() )       isParameterValid = false; break;
        case JSON_REAL:     if( !parameterValue.isDouble() )    isParameterValid = false; break;
        case JSON_NUMERIC:  if( !parameterValue.isNumeric() )   isParameterValid = false; break;
        case JSON_STRING:   if( !parameterValue.isString() )    isParameterValid = false; break;
        case JSON_BOOLEAN:  if( !parameterValue.isBool() )      isParameterValid = false; break;
        case JSON_OBJECT:   if( !parameterValue.isObject() )    isParameterValid = false; break;
        case JSON_ARRAY:    if( !parameterValue.isArray() )     isParameterValid = false; break;
    }

    return isParameterValid;
}

