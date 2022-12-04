#include "specificationwriter.h"
#include "jsonparser.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace itsp;

Json::Value SpecificationWriter::toJsonValue( const vector<Procedure> &procedures )
{
    Json::Value resultJson;
    Json::Value row;
    for( uint16_t i = 0 ; i < procedures.size(); i++ )
    {
        procedureToJsonValue( procedures.at(i), row );
        resultJson[i] = row;
        row.clear();
    }

    return resultJson;
}

std::string SpecificationWriter::toString( const vector<Procedure> &procedures )
{
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["indentation"]  = "   ";
    return Json::writeString( writerBuilder, toJsonValue( procedures ) );
}

bool SpecificationWriter::toFile( const std::string &fileName, const vector<Procedure> &procedures )
{
    ofstream fileOutputStream;
    fileOutputStream.open( fileName.c_str(), ios_base::out );
    if( !fileOutputStream.is_open() ) { return false; }
    fileOutputStream << toString( procedures );
    return true;
}

Json::Value SpecificationWriter::toJsonLiteral( jsontype_t type )
{
    Json::Value jsonLiteral;
    switch( type )
    {
        case JSON_BOOLEAN:  jsonLiteral = true;                         break;
        case JSON_STRING:   jsonLiteral = "someDefaultString";          break;
        case JSON_INTEGER:  jsonLiteral = 1;                            break;
        case JSON_UINTEGER: jsonLiteral = 1;                            break;
        case JSON_REAL:     jsonLiteral = 1.0;                          break;
        case JSON_NUMERIC:  jsonLiteral = 1.0;                          break;
        case JSON_ARRAY:    jsonLiteral = Json::arrayValue;                  break;
        case JSON_OBJECT:   jsonLiteral["objectKey"] = "objectValue";   break;
    }

    return jsonLiteral;
}

void SpecificationWriter::procedureToJsonValue( const Procedure &procedure, Json::Value &targetJson )
{
    targetJson[KEY_SPEC_PROCEDURE_NAME] = procedure.GetProcedureName();
    if( procedure.GetProcedureType() == RPC_METHOD )
    {
        targetJson[KEY_SPEC_RETURN_TYPE] = toJsonLiteral( procedure.GetProcedureReturnType() );
    }

    for( parameterNameList_t::const_iterator i = procedure.GetProcedureParameters().begin(); i != procedure.GetProcedureParameters().end(); ++i )
    {
        if( procedure.GetProcedureParameterDeclarationType() == PARAMS_BY_POSITION )
        {
            targetJson[KEY_SPEC_PROCEDURE_PARAMETERS].append( toJsonLiteral(i->second) );
        }
        else
        {
            targetJson[KEY_SPEC_PROCEDURE_PARAMETERS][i->first] = toJsonLiteral(i->second);
        }
    }
}
