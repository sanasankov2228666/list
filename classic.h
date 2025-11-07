#ifndef CLASSIC_H_
#define CLASSIC_H_

#include <stdio.h>

typedef int err_t;

typedef int list_t;


struct node_t
{
    list_t data  = 0;
    node_t* next = NULL;
    node_t* prev = NULL;
};

struct list
{
    node_t* head    = NULL;
    size_t size     = 0;
    size_t error    = 0;
    FILE*  html_out = NULL;
};


enum
{
   ERROR   = 1,
   SUCCSES = 0,
};


enum
{   
    CORRECT       = 0,
    BAD_POINTER   = 1,
    BAD_SIZE      = 2,
    BAD_TAIL_HEAD = 4,
};

#define VERIFY if (verify(data,  __FUNCTION__, __FILE__, __LINE__)) return ERROR;\

node_t* node_creator(list* data);

err_t insert_begin(list* data, list_t val);

err_t insert_end(list* data, list_t val);

err_t insert_after(list* data, node_t* node_aft, list_t val);

err_t insert_before(list* data, node_t* node_bfr, list_t val);

err_t remove_begin(list* data);

err_t remove_end(list* data);

err_t remove_after(list* data, node_t* node_aft);

err_t remove_before(list* data, node_t* node_bfr);

void list_dump(list* data);

void list_deleter(list* data);

node_t* get_head(list* data);

node_t* get_tail(list* data);

err_t checker(list* data);

err_t verify(list* data, const char* func, const char* file, int LINE);

#endif
