#ifndef CHECKER_H_
#define CHECKER_H_

#include "list.h"

#define INF 1000000000

enum
{   
    CORRECT       = 0,
    BAD_POINTER   = 1,
    BAD_CAPACITY  = 2,
    BAD_SIZE      = 4,
    BAD_FREE      = 8,
    CANARY_CHANGE = 16,
    BAD_HEAD      = 32,
    BAD_TAIL      = 64,
    BAD_ORDER     = 128,
};

#define VERIFY if (verify(list,  __FUNCTION__, __FILE__, __LINE__)) return ERROR;\

error_t generate_dot_dump(list_s* list, const char* filename, const char* reason);

void list_dump(list_s list, const char* reason);

void mass_output(list_s list, size_t* arr);

error_t checker(list_s* list);

error_t verify(list_s* list, const char* func, const char* file, int LINE);

#endif