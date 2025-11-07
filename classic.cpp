#include <stdio.h>
#include <stdlib.h>
#include "classic.h"

int main(void)
{
    struct list data = {};
    data.html_out = fopen("dump_classic.html", "w");

    insert_begin(&data, 11);
    list_dump(&data);

    insert_begin(&data, 22);
    list_dump(&data);

    insert_end(&data, 33);
    list_dump(&data);

    insert_begin(&data, 44);
    list_dump(&data);

    insert_begin(&data, 55);
    list_dump(&data);

    insert_after(&data, data.head->next->next->next, 4444 );
    list_dump(&data);

    insert_before(&data, data.head->next, 5555 );
    list_dump(&data);

    remove_begin(&data);
    list_dump(&data);

    remove_end(&data);
    list_dump(&data);

    remove_after(&data, data.head->next->next);
    list_dump(&data);

    remove_before(&data, data.head->next->next->next->next);
    list_dump(&data);

    list_deleter(&data);

    return 0;
}

node_t* node_creator(list* data)
{
    node_t* node = (node_t*) calloc (1, sizeof(node_t) );
    if (node == NULL)
    {
        printf("calloc NULL\n");
        return NULL;
    }

    data->size++;

    return node; 
}



err_t insert_begin(list* data, list_t val)
{
    VERIFY;

    if (data->head == NULL)
    {
        data->head = node_creator(data);
        if (data->head == NULL) return ERROR;
        data->head->data = val;
        data->head->next = data->head;
        data->head->prev = data->head;
        
        return SUCCSES;
    }

    node_t* node = node_creator(data);
    if (node == NULL) return ERROR;
    
    node->data = val;
    node->next = data->head;
    node->prev = data->head->prev;
    
    data->head->prev->next = node;
    data->head->prev = node;
    data->head = node;
    
    VERIFY;
    return SUCCSES;
}

err_t insert_end(list* data, list_t val)
{       
    VERIFY;

    if (data->head == NULL)
    {
        data->head = node_creator(data);
        if (data->head == NULL) return ERROR;

        data->head->data = val;
        data->head->next = get_head(data);
        data->head->prev = get_head(data);

        return SUCCSES;
    }

    node_t* node = node_creator(data);
    if (node == NULL) return ERROR;

    node->data = val;
    node_t* tail = data->head->prev;
    
    node->next = data->head;
    node->prev = tail;
    tail->next = node;
    data->head->prev = node;
    
    return SUCCSES;
}

err_t insert_after(list* data, node_t* node_aft, list_t val)
{    
    //data->head = NULL;
    VERIFY;

    if (data->head == NULL)
    {
        data->head = node_creator(data);
        if (data->head == NULL) return ERROR;

        data->head->data = val;
        data->head->next = get_head(data);
        data->head->prev = get_head(data);

        return SUCCSES;
    }

    node_t* node = node_creator(data);
    if (node == NULL) return ERROR;

    node->data = val;
    node_t* old_next = node_aft->next;
    node_aft->next = node;
    old_next->prev = node;

    node->next = old_next;
    node->prev = node_aft;

    VERIFY;
    return SUCCSES;
}

err_t insert_before(list* data, node_t* node_bfr, list_t val)
{       
    VERIFY;

    if (data->head == NULL)
    {
        data->head = node_creator(data);
        if (data->head == NULL) return ERROR;

        data->head->data = val;
        data->head->next = get_head(data);
        data->head->prev = get_head(data);

        return SUCCSES;
    }

    node_t* node = node_creator(data);
    if (node == NULL) return ERROR;

    node->data = val;
    node_t* old_prev = node_bfr->prev;
    node_bfr->prev = node;
    old_prev->next = node;

    node->prev = old_prev;
    node->next = node_bfr;

    VERIFY;

    return SUCCSES;
}

err_t remove_begin(list* data)
{
    VERIFY;

    if (data->size == 0)
    {
        printf("list doesnt have elements\n");
        return ERROR;
    }

    node_t* old_head = get_head(data);
    data->head = old_head ->next;
    data->head->prev = old_head->prev;

    free(old_head);

    data->size--;

    VERIFY;

    return SUCCSES;
}

err_t remove_end(list* data)
{
    VERIFY;

    if (data->size == 0)
    {
        printf("list doesnt have elements\n");
        return ERROR;
    }

    node_t* old_tail = get_tail(data);
    old_tail->prev->next = data->head;
    data->head->prev = old_tail->prev;

    free(old_tail);

    data->size--;

    VERIFY;

    return SUCCSES;
}

err_t remove_after(list* data, node_t* node_aft)
{   
    VERIFY;

    if (data->size == 0)
    {
        printf("list doesnt have elements\n");
        return ERROR;
    }

    if (node_aft == NULL)
    {
        printf("node is NULL\n");
        return ERROR;
    }

    node_t* node_to_remove = node_aft->next;
    node_aft->next = node_to_remove->next;
    
    free(node_to_remove);
    data->size--;

    VERIFY;

    return SUCCSES;
}

err_t remove_before(list* data, node_t* node_bfr)
{   
    VERIFY;

    if (data->size == 0)
    {
        printf("list doesnt have elements\n");
        return ERROR;
    }

    if (node_bfr == NULL)
    {
        printf("node is NULL\n");
        return ERROR;
    }

    node_bfr->prev = node_bfr->prev->prev;
    node_bfr->prev->prev->next = node_bfr;

    free(node_bfr->prev);

    data->size--;

    VERIFY;

    return SUCCSES;
}

void list_deleter(list* data)
{

    if (data->size == 0) return;
    
    node_t* current = data->head;
    for (size_t i = 0; i < data->size; i++)
    {
        node_t* next = current->next;
        free(current);
        current = next;
    }

    data->head = NULL;
    data->size = 0;

    fclose(data->html_out);
}


node_t* get_head(list* data)
{
    return data->head;
}

node_t* get_tail(list* data)
{
    return data->head->prev;
}


void list_dump(list* data)
{
    printf("==========================================DUMP==============================================\n\n");

    if (data->size == 0)
    {
        printf("\n[]\n");
        return;
    }

    printf("size = %zu\n", data->size);

    printf("error = %zu\n", data->error);

    printf("\nlist:");

    node_t* current = data->head;
    printf("\n[");
    
    for (size_t i = 0; i < data->size; i++)
    {
        printf("%d  |", current->data);
        current = current->next;
    }
    printf("]\n");

    printf("\n\n==========================================================================================\n\n");
}

// err_t dump_generator(list* data)
// {
    
// }

err_t checker(list* data)
{
    data->error = 0;

    if (data->head == NULL)
    {
        if (data->size != 0) 
            data->error |= BAD_TAIL_HEAD;
        return data->error;
    }

    if (data->size > 1000000000)
    data->error |= BAD_SIZE;

    node_t* next = data->head;
    for (size_t i = 0; i < data->size; i++)
    {
        if (next == NULL) data->error |= BAD_POINTER;
        next = next->next;
    }

    return data->error;
}

err_t verify(list* data, const char* func, const char* file, int LINE)
{
    checker(data);

    if (data->error & BAD_POINTER) 
    {
        printf("\nbad pointer!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        fprintf(data->html_out, "bad pointer!\n");
        fprintf(data->html_out, "In function %s, %s:%d\n", func, file, LINE);
    }

    if (data->error & BAD_SIZE) 
    {
        printf("\nbad size!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        fprintf(data->html_out, "bad size!\n");
        fprintf(data->html_out, "In function %s, %s:%d\n", func, file, LINE);
    }

    if (data->error & BAD_TAIL_HEAD) 
    {
        printf("\nbad tail or head!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        fprintf(data->html_out, "bad tail or head!\n");
        fprintf(data->html_out, "In function %s, %s:%d\n", func, file, LINE);
    }

    if (data->error) list_dump(data);

    return data->error;
}