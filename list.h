#ifndef LIST_H_
#define LIST_H_

#define PZN 222

#include <stdio.h>

typedef size_t list_t;

typedef int error_t;

struct list
{
   list_t* data = NULL;
   size_t* next = NULL;
   size_t* prev = NULL; 

   size_t capacity = 0;
   size_t size     = 0;

   size_t free_i = 0; 

   size_t error  = 00000000;
};

typedef struct list list_s;

enum
{
   ERROR   = 1,
   SUCCSES = 0,
};

enum
{
    correct = 0,
    bad_size = 1,
    bad_capacity = 2,
    bad_pointer = 4,
    kanareika_error = 8,
    poison_error = 16,
    hash_error = 32,
};

//макрос проверки указателя 
#define PTR_MEM_CHECK(ptr)\
         if (ptr == NULL) \
         {\
            printf("NULL pointer after mem func\n");\
            return ERROR;\
         }\



//-----------------------------------------------------------ВСЯКОЕ--------------------------------------------------------------

size_t get_head(list_s* list);

size_t get_tail(list_s* list);

//--------------------------------------------------------ФУНКЦИИ-СПИСКА----------------------------------------------------------

error_t list_creator(list_s* list, size_t capacity);

error_t insert_begin(list_s* list, list_t val);

error_t list_deleter(list_s* list);

error_t insert_after(list_s* list, list_t val, size_t after);

error_t insert_end(list_s* list, list_t val);

error_t insert_before(list_s* list, list_t val, size_t before);

error_t delete_begin(list_s* list);

error_t delete_end(list_s* list);

error_t delete_after(list_s* list, size_t after);

error_t delete_before(list_s* list, size_t before);

error_t list_realloc(list_s* list);

#endif