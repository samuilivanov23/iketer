#ifndef SPECIFICATION_H
#define SPECIFICATION_H

#define KEY_SPEC_PROCEDURE_NAME "name"
#define KEY_SPEC_PROCEDURE_METHOD "method"
#define KEY_SPEC_PROCEDURE_NOTIFICATION "notification"
#define KEY_SPEC_PROCEDURE_PARAMETERS "parameters"
#define KEY_SPEC_RETURN_TYPE "returns"

namespace itsp
{
    //Defines if the procedure is a method or notification
    typedef enum { RPC_METHOD, RPC_NOTIFICATION };

    //Defines all valid and processable types of this protocol
    enum jsontype_t { JSON_STRING = 1, JSON_BOOLEAN = 2, JSON_INTEGER = 3, JSON_UINTEGER = 4, JSON_REAL = 5, JSON_OBJECT = 6, JSON_ARRAY = 7, JSON_NUMERIC = 8 }
}


#endif //SPECIFICATION_H
