#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "cheack.h"


//!генератор графов
error_t generate_dot_dump(list_s* list, const char* filename, const char* reason)
{
    char dot_filename[256] = {};
    snprintf(dot_filename, sizeof(dot_filename), "dot/%s.dot", filename);
    
    FILE* dot_file = fopen(dot_filename, "w");
    if (!dot_file)
    {
        printf("Error creating DOT file\n");
        return ERROR;
    }
    
    fprintf(dot_file, "digraph List {\n");
    fprintf(dot_file, "\trankdir=LR;\n");
    fprintf(dot_file, "\tnodesep=1;\n");
    fprintf(dot_file, "\tranksep=0.4;\n");
    fprintf(dot_file, "\tnode [shape=record, style=filled];\n");
    fprintf(dot_file, "\tsplines=ortho;\n");
    fprintf(dot_file, "\tlabel=\"%s\";\n", reason);
    fprintf(dot_file, "\tlabelloc=t;\n\n");
    
    fprintf(dot_file, "\n");

    for (size_t i = 0; i < list->capacity + 2; i++)
    {
        fprintf(dot_file, "\tnode%zu [label = \"index %zu|data = %zu|next = %zu|prev = %zu\"];\n",
        i, i, list->data[i], list->next[i], list->prev[i]);
    }
    
    fprintf(dot_file, "\n");

    for (size_t i = list->next[get_head(list)]; i != get_tail(list); i = list->next[i])
    {
        if (i != 0)
        fprintf(dot_file, "node%zu [fillcolor=\"lightblue\", color=\"darkblue\", style=\"filled\"];", i);
    }

    fprintf(dot_file, "\n");

    for (size_t i = 0; i < list->capacity + 1; i++)
    {
        fprintf(dot_file, "\tnode%zu -> node%zu [style=invis, weight=1000];\n", i, i + 1);
    }

    fprintf(dot_file, "\n");

    for (size_t i = get_head(list); i != get_tail(list); i = list->next[i])
    {
        fprintf(dot_file, "\tnode%zu->node%zu [color = blue, constraint=false];\n", i, list->next[i]);
    }

    fprintf(dot_file, "\n");

    for (size_t i = get_tail(list); i != get_head(list); i = list->prev[i])
    {
        fprintf(dot_file, "\tnode%zu->node%zu [color = red, constraint=false];\n", i, list->prev[i]);
    }

    fprintf(dot_file, "\n");


    fprintf(dot_file, "\tHEAD [shape=plaintext];\n");
    fprintf(dot_file, "\tTAIL [shape=plaintext];\n");
    fprintf(dot_file, "\tFREE [shape=plaintext];\n");
    
    fprintf(dot_file, "\t{rank = same ;HEAD; node%zu [color=blue]};\n", get_head(list));
    
    fprintf(dot_file, "\t{rank = same ;TAIL; node%zu [color=red]};\n", get_tail(list));

    fprintf(dot_file, "\t{rank = same ; FREE; node%zu [color=green]};\n", list->free_i);
    
    
    fprintf(dot_file, "}\n");
    fclose(dot_file);
    
    char command[400];
    snprintf(command, sizeof(command), "dot -Tpng dot/%s.dot -o png/%s.png", filename, filename);
    
    int result = system(command);
    if (result != 0)
    {
        printf("ERROR\n");
        return ERROR;
    }
    
    printf("PNG was generated\n");

    return SUCCSES;
}

//!распечатка списка
void list_dump(list_s list, const char* reason)
{   
    printf("\n-------------------------------------------list-dump------------------------------------------------\n\n");

    printf("Reason - %s\n", reason);

    printf("head = %zu\n", get_head(&list));
    printf("tail = %zu\n", get_tail(&list));
    printf("size = %zu\n", list.size);
    printf("capacity = %zu\n", list.capacity);
    printf("free_index = %zu\n", list.free_i);
    printf("\n");

    printf("---------------------------------------------physical-location--------------------------------------\n\n");

    mass_output(list, list.data);
    mass_output(list, list.next);
    mass_output(list, list.prev);
    
    printf("\n\n\n");

    printf("----------------------------------------------logical-location----------------------------------------\n\n");

    for (size_t i = 0; i < list.capacity; i++)
    {
        printf("%5zu  ", i);
    }
    
    printf("\n");
    
    for (size_t i = 0; i < list.capacity; i++)
    {
        printf("-------");
    }
    
    printf("\n");

    for (size_t i = get_head(&list); i != get_tail(&list); i = list.next[i])
    {
        printf("|%5zu", list.data[i]);
    }
    
    printf("|%5zu|\n", list.data[get_tail(&list)]);

    for (size_t i = 0; i < list.capacity; i++)
    {
        printf("-------");
    }

    printf("\n");

    printf("\n--------------------------------------------------END------------------------------------------------\n");

    static int dump_count = 0;
    char filename[40] = {};
    snprintf(filename, sizeof(filename), "list_dump_%d", dump_count++);
    
    if (generate_dot_dump(&list, filename, reason) ) printf("PNG generate failed\n");

    printf("press enter to continue\n");
    getchar();
}

//!функция вывода массивов
void mass_output(list_s list, size_t* arr)
{
    for (size_t i = 0; i < list.capacity + 2; i++)
    {
        printf("%5zu  ", i);
    }
    
    printf("\n");
    
    for (size_t i = 0; i < list.capacity + 2 ; i++)
    {
        printf("-------");
    }
    
    printf("\n");
    
    for (size_t i = 0; i < list.capacity + 2; i++)
    {
        printf("|");
        printf("%6zu", arr[i]);
    }
    
    printf("|\n");
    
    for (size_t i = 0; i < list.capacity + 2; i++)
    {
        printf("-------");
    }

    printf("\n\n");
}

//!функция проверки
error_t checker(list_s* list)
{   
    if (list->data == NULL || list->next == NULL || list->prev == NULL)
    list->error |= BAD_POINTER;

    if (list->capacity < 1 || list->capacity > INF)
    list->error |= BAD_CAPACITY ;

    if (list->size > list->capacity || list->size > INF) 
    list->error |= BAD_SIZE;

    if (list->prev[list->free_i] != PZN && list->prev[list->free_i] != 0)
    list->error |= BAD_FREE;

    if (list->data[0] != 0 || list->data[list->capacity + 1] != 0) 
    list->error |= CANARY_CHANGE;

    if (list->prev[get_head(list)] == PZN)
    list->error |= BAD_HEAD;

    if (list->prev[get_tail(list)] == PZN)
    list->error |= BAD_TAIL;

    for (size_t i = get_head(list); i != 0; i = list->next[i])
    {
        if ( list->prev[list->next[i]] != i)
        {   
            list->error |= BAD_ORDER;
            break;
        }
    }

    return (int) list->error;
}

//!функция верефикатор
error_t verify(list_s* list, const char* func, const char* file, int LINE)
{
    int error = checker(list);

    if (list->error & 1)
    {
        printf("bad pointer!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
    }

    if (list->error & 2)
    {
        printf("bad capacity!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_dump(*list, "BAD CAPACITY in");
    }

    if (list->error & 4)
    {
        printf("bad size!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_dump(*list, "BAD SIZE");
    }

    if (list->error & 8)
    {
        printf("bad free!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_dump(*list, "BAD FREE");
    }

    if (list->error & 16)
    {
        printf("canary change!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_dump(*list, "CANARY CHANGED");
        list_deleter(list);
    }

    if (list->error & 32)
    {
        printf("bad head!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_dump(*list, "BAD HEAD");
    }

    if (list->error & 64)
    {
        printf("bad tail!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_dump(*list, "BAD TAIL");
    }

    if (list->error & 128)
    {
        printf("bad order!\n");
        printf("In function %s, %s:%d\n", func, file, LINE);
        list_dump(*list, "BAD ORDER");
    }

    if (error != 0)
    {
        list_deleter(list);
        return ERROR;
    }

    return SUCCSES;
}