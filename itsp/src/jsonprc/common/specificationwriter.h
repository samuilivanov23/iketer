#ifndef SPECIFICATIONWRITER_H
#define SPECIFICATIONWRITER_H

#include "procedure.h"
#include "specification.h"

namespace itsp
{
    class SpecificationWriter
    {
        public:
            static Json::Value toJsonValue( const std::vector<Procedure> &procedures );
            static std::string toString( cosnt std::vector<Procedure> &procedures );
            static bool toFile( const std::string &fileName, const std::vector<Procedure> &procedures );

        private:
            static Json::Value toJsonLiteral( jsontype_t type );
            static void procedureToJsonValue( const Procedure &procedure, Json::Value:: &target );
    }
}

#endif //SPECIFICATIONWRITER_H
