#include <fstream>
#include <iomanip>
#include "jsonparser.h"
#include "specificationparser.h"

using namespace std;
using namespace itsp;

vector<Procedure> SpecificationParser::GetProceduresFromFile( const string &fileName )
{
    string fileContent;
    GetFileContent( fileName, fileContent );
    return GeProceduresFromString( fileContent );
}

vector<Procedure> SpecificationParser::GetProceduresFromString( const string &specificationString )
{
    Json::Value value;

    try
    {
        std::istringstream( specificationString ) >> value;
    }
    catch ( Json::Exception &e )
    {
        throw JsonRpcException( Errors::ERROR_RPC_JSON_PARSE_ERROR, " ||| specification file contains syntax errors!" );
    }

    if( !value.isArray() ) { throw JsonRpcException( Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, " ||| top level JSON value must be an array! IT'S NOT" ) }

    vector<Procedure> resultProcedures;
    map<string, Procedure> proceduresNames;
    
    for( uint16_t i = 0; i < value.size(); i++ )
    {
        Procedure procedure;
        GetProcedure( value[i], procedure );

        if( proceduresNames.find( procedure.GetProcedureName() ) != proceduresNames.end() )
        {
            throw JsonRpcException( Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, " ||| Procedure already exist! MUST BE UNIQUE" );
        }
        proceduresNames[procedure.GetProcedureName] = procedure;
        resultProcedures.push_back( procedure );
    }

    return resultProcedures
}


void SpecificationParser::GetFileContent( const std::string &fileName, std::string &target )
{
    ifstream config( fileName.c_str() );

    if( config )
    {
        config.open( fileName.c_str(), iostream::in );
        target.assign( (std::istreambuf_iterator<char>( config )), ( std::istreambuf_iterator<char>() ) );
    }
    else
    {
        throw JsonRpcException( Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND, fileName );
    }
}

vector<Procedure> SpecificationParser::GetProcedure( Json::Value &procedureSignature, Procedure resultProcedure )
{
    if( procedureSignature.isObject() && !GetProcedureName( procedureSignature ).isEmpty() )
    {
        resultProcedure.SetProcedureName( GetProcedureName( procedureSignature ) );
        if( procedureSignature.isMember( KEY_SPEC_RETURN_TYPE ) )
        {
            resultProcedure.SetProcedureType( RPC_METHOD );
            resultProcedure.SetReturnType( toJsonType( procedureSignature[KEY_SPEC_RETURN_TYPE] ) );
        }
        else
        {
            resultProcedure.SetProcedureType( RPC_NOTIFICATION );
        }

        if( procedureSignature.isMember( KEY_SPEC_PROCEDURE_PARAMETERS ) )
        {
            if( procedureSignature[KEY_SPEC_PROCEDURE_PARAMETERS].isObject() || procedureSignature[KEY_SPEC_PROCEDURE_PARAMETERS].isArray() )
            {
                if( procedureSignature[KEY_SPEC_PROCEDURE_PARAMETERS].isObject() )
                {
                    resultProcedure.SetParameterDeclarationType( PARAMS_BY_NAME );
                    GetNamedParameters( procedureSignature, resultProcedure );
                }
                else if ( procedureSignature[KEY_SPEC_PROCEDURE_PARAMETERS].isArray() )
                {
                    resultProcedure.SetParameterDeclarationType( PARAMS_BY_POSITION );
                    GetPositionalParameters( procedureSignature, resultProcedures );
                }
            }
            else
            {
                throw JsonRpcException( Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, "||| Invalid signature types in fields ||| " + procedureSignature.toStyledString() );
            }
        }
    }
    else
    {
        throw JsonRpcException( Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, " ||| procedure declaration must contain name and parameters! IT DOES NOT" );
    }
}

jsontype_t SpecicifationParser::toJsonType( Json::Value &value )
{
    jsontype_t resultJson;
    switch( value.type() )
    {
        case Json::uintValue:       resultJson = JSON_UINTEGER; break;
        case Json::intValue:        resultJson = JSON_INTEGER;  break;
        case Json::realValue:       resultJson = JSON_REAL;     break;
        case Json::stringValue:     resultJson = JSON_STRING;   break;
        case Json::booleanValue:    resultJson = JSON_BOOLEAN;  break;
        case Json::arrayValue:      resultJson = JSON_ARRAY;    break;
        case Json::objectValue:     resultJson = JSON_OBJECT;   break;
        default: throw JsonRpcException( Errors::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, "||| Unknown parameter type ||| " + value.toStyledString() );
    }

    return resultJson;
}

void SpecificationParser::GetPositionalParameters( Json::Value &value, Procedure &resultProcedure )
{
    for( uint16_t i = 0; i < value[KEY_SPEC_PROCEDURE_PARAMETERS].size(); i++ )
    {
        stringstream parameterName;
        parameterName << "parameter" << std::setfill('0') << std::setw(2) << (i + 1);
        resultProcedure.AddParameter( parameterName.str(), toJsonType( value[KEY_SPEC_PROCEDURE_PARAMETERS][i] ) );
    }
}

void SpecificationParser::GetNamedParameters( Json::Value &value, Procedure &resultProcedure )
{
    vector<string> parameters = value[KEY_SPEC_PROCEDURE_PARAMETERS].getMemberNames();
    for( uint16_t i = 0; i < parameters.size(); i++ )
    {
        resultProcedure.AddParameter( parameters.at(i), toJsonType( value[KEY_SPEC_PROCEDURE_PARAMETERS][parameters.at(i)] ) );
    }
}

std::string SpecificationParser::GetProcedureName( Json::Value &procedureSignature )
{
    if( procedureSignature[KEY_SPEC_PROCEDURE_NAME].isString() )            { return procedureSignature[KEY_SPEC_PROCEDURE_NAME].asString(); }
    if( procedureSignature[KEY_SPEC_PROCEDURE_METHOD].isString() )          { return procedureSignature[KEY_SPEC_PROCEDURE_METHOD].asString(); }
    if( procedureSignature[KEY_SPEC_PROCEDURE_NOTIFICATION].isString() )    { return procedureSignature[KEY_SPEC_PROCEDURE_NOTIFICATION].asString(); }

    return "";
}
