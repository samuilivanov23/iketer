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


