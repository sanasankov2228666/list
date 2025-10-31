#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "cheack.h"

//дебаг 
//#define DEBUG
#include "debug.h"


int main(void)
{
    list_s list = {};
    
    list_creator(&list, 3);
    list_dump(list, "123" );

    insert_begin(&list, 11);
    list_dump(list, "123" );

    insert_end(&list, 22);
    list_dump(list, "123" );

    insert_begin(&list, 33);
    list_dump(list, "123" );

    insert_end(&list, 55);
    list_dump(list, "123" );

    insert_after(&list, 4444, 3);
    list_dump(list, "123" );

    insert_before(&list, 5555, 2);
    list_dump(list, "123" );

    delete_begin(&list);
    list_dump(list, "123" );

    delete_begin(&list);
    list_dump(list, "123" );

    delete_end(&list);
    list_dump(list, "123" );

    insert_begin(&list, 99);
    list_dump(list, "123" );

    delete_after(&list, 3);
    list_dump(list, "123" );

    //-------------------------------------------

    insert_begin(&list, 11);
    list_dump(list, "123");

    insert_begin(&list, 22);
    list_dump(list, "123" );

    insert_begin(&list, 33);
    list_dump(list, "123" );

    insert_begin(&list, 44);
    list_dump(list, "123" );

    delete_before(&list, 1);
    list_dump(list, "123" );

    delete_after(&list, 1);
    list_dump(list, "123" );

    insert_after(&list, 999, 0);
    list_dump(list, "123");

    insert_before(&list, 999, 1);
    list_dump(list, "123" );

    list_deleter(&list);

    return 0;
}

//--------------------------------------------------------ФУНЦИИ-СПИСКА-----------------------------------------------------------------


//!функция создания списка
error_t list_creator(list_s* list, size_t capacity)
{
    if (capacity < 1)
    {
        printf("bad capacity\n");
        return ERROR;
    }

    list->data = (list_t*) calloc (capacity + 2, sizeof(list_t));
    PTR_MEM_CHECK(list->data);

    list->next = (size_t*) calloc (capacity + 2, sizeof(size_t));
    PTR_MEM_CHECK(list->next);

    list->prev = (size_t*) calloc (capacity + 2, sizeof(size_t));
    PTR_MEM_CHECK(list->prev);

    list->capacity = capacity;
    list->size = 0;

    list->data[0] = 0;
    list->data[capacity + 1] = 0;

    list->next[0] = 0;
    list->next[capacity + 1] = 0;

    list->prev[0] = 0;
    list->prev[capacity + 1] = PZN;

    list->free_i = 1;

    for (size_t i = 1; i <= capacity; i++)
    {

        list->data[i] = i + 1;
        list->next[i] = i + 1;
        list->prev[i] = PZN;
    }

    VERIFY;

    return SUCCSES;
}

//!функция вставки в начало
error_t insert_begin(list_s* list, list_t val)
{   
    if ( list->size == list->capacity )
    {
        list_realloc(list);
    }

    size_t index = list->free_i;

    list->free_i = list->data[list->free_i];
    list->data[index] = val;

    size_t old_head = get_head(list);
    list->next[0] = index;
    list->next[get_head(list)] = old_head;
    list->prev[old_head] = get_head(list);
    list->prev[get_head(list)] = 0;

    list->size++;

    return SUCCSES;
}

//!функция вставки в конец
error_t insert_end(list_s* list, list_t val)
{   
    VERIFY;
    
    if ( list->size == list->capacity )
    {
        list_realloc(list);
    }

    size_t index = list->free_i;

    list->free_i = list->data[list->free_i];
    list->data[index] = val;

    size_t old_tail = get_tail(list);
    list->prev[0] = index;

    list->next[old_tail] = get_tail(list);
    list->prev[get_tail(list)] = old_tail;
    list->next[get_tail(list)] = 0;

    list->size++;

    VERIFY;

    return SUCCSES;
}

//!фунция вставки после элемента
error_t insert_after(list_s* list, list_t val, size_t after)
{      
    VERIFY;

    if ( list->size == list->capacity )
    {
        list_realloc(list);
    }

    after++;

    if (after > list->size || list->prev[after] == PZN)
    {
        printf("elem[%zu] - doesent exist\n", after - 1);
        return ERROR;
    }

    size_t index = list->free_i;
    
    list->free_i = list->data[list->free_i];
    list->data[index] = val;

    list->prev[index] = after;
    list->prev[list->next[after]] = index;

    size_t old_way = list->next[after];
    list->next[after] = index;
    list->next[index] = old_way;

    list->size++;

    VERIFY;

    return SUCCSES;
}

//!функция вставки перед элементом
error_t insert_before(list_s* list, list_t val, size_t before)
{      
    VERIFY;

    if ( list->size == list->capacity )
    {
        list_realloc(list);
    }

    before++;
    
    if (before > list->size || list->prev[before] == PZN)
    {
        printf("elem[%zu] - doesent exist\n", before - 1);
        return ERROR;
    }

    size_t index = list->free_i;
    
    list->free_i = list->data[list->free_i];
    list->data[index] = val;

    size_t old_way = list->prev[before];
    list->next[index] = before;
    list->prev[before] = index;
    list->prev[index] = old_way;
    list->next[old_way] = index;

    list->size++;

    VERIFY;

    return SUCCSES;
}

//!функция удаления начала
error_t delete_begin(list_s* list)
{
    VERIFY;
    
    if (list->size == 0) return ERROR;

    size_t index = list->free_i;

    size_t old_head = get_head(list);
    list->next[0] = list->next[get_head(list)];

    list->prev[old_head] = PZN;
    list->free_i = old_head;
    list->data[old_head] = index;
    list->next[old_head] = index;

    list->size--;

    VERIFY;

    return SUCCSES;
}

//!функция удаления конца
error_t delete_end(list_s* list)
{
    VERIFY;

    if (list->size == 0) return ERROR;

    size_t index = list->free_i;

    size_t old_tail = get_tail(list);
    list->prev[0] = list->prev[get_tail(list)];

    list->next[get_tail(list)] = 0;
    list->prev[old_tail] = PZN;
    list->free_i = old_tail;
    list->data[old_tail] = index;
    list->next[old_tail] = index;

    list->size--;

    VERIFY;

    return SUCCSES;
}

//!функция удаления после элемента
error_t delete_after(list_s* list, size_t after)
{
    VERIFY;

    after++;

    if (list->size == 0) return ERROR;

    if (after == get_tail(list))
    {
        printf("cant delete after las element\n");
        return ERROR;
    }

    size_t old_free = list->free_i;
    list->free_i = list->next[after];

    size_t old_next = list->next[after];

    list->next[after] = list->next[list->next[after]];
    list->prev[list->next[after]] = after; //old_next

    list->data[old_next] = old_free;
    list->next[old_next] = old_free;
    list->prev[old_next] = PZN;

    list->size--;

    VERIFY;

    return SUCCSES;
}

//!функция удаления перед элемента
error_t delete_before(list_s* list, size_t before)
{   
    VERIFY;

    before++;

    if (before > list->size)
    {
        printf("elem[%zu] - doesent exist\n", before - 1);
        return ERROR;
    }

    if (list->size == 0) return ERROR;

    if (before == get_head(list))
    {
        printf("cant delete before first element\n");
        return ERROR;
    }

    size_t old_free = list->free_i;
    list->free_i = list->prev[before];

    size_t old_prev = list->prev[before];
    list->prev[before] = list->prev[list->prev[before]];

    list->next[list->prev[before]] = before; //list->next[old_prev]

    list->data[old_prev] = old_free;
    list->next[old_prev] = old_free;
    list->prev[old_prev] = PZN;

    list->size--;

    VERIFY;

    return SUCCSES;
}

//!функция удлинения
error_t list_realloc(list_s* list)
{
    list->data = (list_t*) realloc ( list->data, sizeof(list_t) * (list->capacity + 1) * 2);
    PTR_MEM_CHECK(list->data)

    list->next = (size_t*) realloc ( list->next, sizeof(size_t) * (list->capacity + 1) * 2);
    PTR_MEM_CHECK(list->next)

    list->prev = (size_t*) realloc ( list->prev, sizeof(size_t) * (list->capacity + 1) * 2);
    PTR_MEM_CHECK(list->prev)

    for (size_t i = list->capacity + 1 ; i < (list->capacity + 1) * 2; i++ )
    {
        list->data[i] = i + 1;
        list->next[i] = i + 1;
        list->prev[i] = PZN;
    }

    list->free_i = list->capacity + 1;
    list->capacity = list->capacity * 2;

    list->data[list->capacity + 1] = 0;
    list->next[list->capacity + 1] = 0;

    return SUCCSES;
}

//!обращение к head
size_t get_head(list_s* list)
{
    return list->next[0];
}

//!обращение к tail
size_t get_tail(list_s* list)
{
    return list->prev[0];
}

//!удаление списка
error_t list_deleter(list_s* list)
{
    free(list->data);
    free(list->next);
    free(list->prev);
    
    list->capacity = 0;
    list->size = 0;
    list->free_i = 0;

    printf("list was deleted\n");

    return SUCCSES;
}