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
    list_dump(list, "list creator");

    insert_begin(&list, 11);
    list_dump(list, "insert_begin, val 11" );

    insert_end(&list, 22);
    list_dump(list, "insert_end, val 22" );

    insert_begin(&list, 33);
    list_dump(list, "insert_begin, val 33" );

    insert_end(&list, 55);
    list_dump(list, "insert_end, val 55" );

    insert_after(&list, 4444, 3);
    list_dump(list, "insert_after, index 3, val 4444" );

    //  list.next[2] = 300; 
    //  list.prev[1] = 200; 

    insert_before(&list, 5555, 2);
    list_dump(list, "insert_before, index 2, val 5555" );

    delete_begin(&list);
    list_dump(list, "delete_begin" );

    delete_begin(&list);
    list_dump(list, "delete_begin" );

    delete_end(&list);
    list_dump(list, "delete_end" );

    insert_begin(&list, 99);
    list_dump(list, "insert_begin, val 99" );

    delete_after(&list, 3);
    list_dump(list, "delete_after, index 3" );

    //-------------------------------------------

    insert_begin(&list, 11);
    list_dump(list, "insert_begin, val 11");

    insert_begin(&list, 22);
    list_dump(list, "insert_begin, val 22" );

    insert_begin(&list, 33);
    list_dump(list, "insert_begin, val 33" );

    insert_begin(&list, 44);
    list_dump(list, "insert_begin, val 44" );

    delete_before(&list, 1);
    list_dump(list, "delete_before, index 1" );

    delete_after(&list, 1);
    list_dump(list, "delete_after, index 1" );

    insert_after(&list, 999, 0);
    list_dump(list, "insert_after, index 0, val 999");

    insert_before(&list, 999, 1);
    list_dump(list, "insert_before, index 1, val 999" );

    // delete_end(&list);
    // list_dump(list, "delete_end" );

    // delete_end(&list);
    // list_dump(list, "delete_end" );

    // delete_end(&list);
    // list_dump(list, "delete_end" );

    // delete_end(&list);
    // list_dump(list, "delete_end" );

    // delete_end(&list);
    // list_dump(list, "delete_end" );

    // delete_begin(&list);
    // list_dump(list, "delete_begin" );

    // delete_begin(&list);
    // list_dump(list, "delete_begin" );

    // delete_begin(&list);
    // list_dump(list, "delete_begin" );

    // delete_begin(&list);
    // list_dump(list, "delete_begin" );

    // delete_begin(&list);
    // list_dump(list, "delete_begin" );

    // sort_list(&list);
    // list_dump(list, "sort list" );

    compact_list(&list);
    list_dump(list, "compact list" );

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
    PTR_CLC_CHECK(list->data);

    list->next = (size_t*) calloc (capacity + 2, sizeof(size_t));
    PTR_CLC_CHECK(list->next);

    list->prev = (size_t*) calloc (capacity + 2, sizeof(size_t));
    PTR_CLC_CHECK(list->prev);

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

    list->html_out = fopen("out_dump.html", "w");
    if (list->html_out == NULL) return ERROR;

    VERIFY;

    return SUCCSES;
}

//!функция вставки в начало
error_t insert_begin(list_s* list, list_t val)
{   
    VERIFY;

    if ( list->size == list->capacity )
    {
        if (list_realloc(list)) return ERROR;
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

    VERIFY;

    return SUCCSES;
}

//!функция вставки в конец
error_t insert_end(list_s* list, list_t val)
{   
    VERIFY;
    
    if ( list->size == list->capacity )
    {
        if (list_realloc(list)) return ERROR;
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
        if (list_realloc(list)) return ERROR;
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
        if (list_realloc(list)) return ERROR;
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
    list->prev[get_head(list)] = 0;

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
    PTR_RLC_CHECK(list->data)

    list->next = (size_t*) realloc ( list->next, sizeof(size_t) * (list->capacity + 1) * 2);
    PTR_RLC_CHECK(list->next)

    list->prev = (size_t*) realloc ( list->prev, sizeof(size_t) * (list->capacity + 1) * 2);
    PTR_RLC_CHECK(list->prev)

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

error_t compact_list(list_s* list)
{
    VERIFY;
    
    if (list->capacity < 10 || list->capacity / list->size < 4)
        return SUCCSES;
    
    sort_list(list);

    list->capacity = list->capacity / 2;

    list->data = (list_t*) realloc ( list->data, sizeof(list_t) * (list->capacity + 2));
    PTR_RLC_CHECK(list->data)

    list->next = (size_t*) realloc ( list->next, sizeof(size_t) * (list->capacity + 2));
    PTR_RLC_CHECK(list->next)

    list->prev = (size_t*) realloc ( list->prev, sizeof(size_t) * (list->capacity + 2));
    PTR_RLC_CHECK(list->prev)

    list->data[list->capacity + 1] = 0;
    list->next[list->capacity + 1] = 0;
    list->prev[list->capacity + 1] = 222;

    VERIFY;

    return SUCCSES;
}

//!сортировка свободных ячеек
error_t sort_list(list_s* list)
{   
    size_t index = 0;
    
    for (size_t i = 1; i < list->capacity + 1; i++)
    {
        if (list->prev[i] == PZN)
        {
            index = search_last(list);
            
            if (index == 0 || list->prev[index] == PZN || index <= i)
            {
                break;
            }
            
            list_t old_data = list->data[index];
            size_t old_next = list->next[index];
            size_t old_prev = list->prev[index];

            list->data[index] = list->data[i];
            list->next[index] = list->next[i];
            list->prev[index] = list->prev[i];

            list->data[i] = old_data;
            list->next[i] = old_next;
            list->prev[i] = old_prev;

            if (list->prev[i] < list->capacity)
                list->next[list->prev[i]] = i;

            if (list->next[i] < list->capacity)
                list->prev[list->next[i]] = i;
        }
    }

    list->free_i = search_first_free(list);

    for (size_t i = list->free_i; i < list->capacity + 1; i++)
    {
        list->next[i] = i + 1;
        list->prev[i] = PZN;
    }
    
    return SUCCSES;
}

//!поиск последней занятой ячейки
size_t search_last(list_s* list)
{
    size_t cur = 0;
    for (size_t i = list->capacity - 1; i >= 1; i--)
    {
        if (list->prev[i] != PZN)
        {
            cur = i;
            break; 
        }
    }

    return cur;
}

//!поиск первой свободной
size_t search_first_free(list_s* list)
{
    for (size_t i = 1; i < list->capacity; i++)
    {
        if (list->prev[i] == PZN) return i;
    }

    return list->capacity;
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

    fclose(list->html_out);

    printf("list was deleted\n");

    return SUCCSES;
}