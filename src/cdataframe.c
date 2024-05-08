#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "list.h"
#include "cdataframe.h"
#include "csv.h"

CDATAFRAME *create_cdataframe_fa(ENUM_TYPE *cdftype, int typesize, int size)
{
    CDATAFRAME *cdf;
    cdf = (CDATAFRAME *)malloc(sizeof(CDATAFRAME));
    LNODE *prev = (LNODE *)malloc(sizeof(LNODE));
    LNODE *current = (LNODE *)malloc(sizeof(LNODE));
    cdf->head = prev;
    prev->prev = NULL;
    prev->data = create_column_fa(NULL, typesize, cdftype);
    for (int i = 1; i < size; i++)
    {
        current->next = NULL;
        current->prev = prev;
        current->data = create_column(NULL, typesize, cdftype);
        prev->next = current;
    }
    cdf->tail = current;
    return cdf;
}

CDATAFRAME *create_cdataframe(int size, int typesize, ...)
{
    va_list args;
    va_start(args, typesize);
    ENUM_TYPE *type = (ENUM_TYPE *)malloc(sizeof(ENUM_TYPE) * typesize);
    ENUM_TYPE typev;
    for (int i = 0; i < typesize; i++)
    {
        typev = va_arg(args, ENUM_TYPE);
        type[i] = typev;
    }
    return create_cdataframe_fa(type, typesize, size);
}

void delete_cdataframe(CDATAFRAME **cdf)
{
    // Free the dataframe, all the lists
    LNODE *c = (*cdf)->head;
    LNODE *n = c->next;
    while (n != NULL)
    {
        free_column(&c->data);
        free(c);
        c = n;
        n = c->next;
    }
    free(*cdf);
    *cdf = NULL;
}

int delete_column_by_name(CDATAFRAME *cdf, char *col_name)
{
    LNODE *node = cdf->head;
    char found = 0;
    while (node->next != NULL && !found)
    {
        if (node->data->title == col_name)
        {
            found = 1;
        }
        else
        {
            node = node->next;
        }
    }
    if (found)
    {
        node->prev->next = node->next;
        if (node->next == NULL)
            cdf->tail = node->prev;
        node->next->prev = node->prev;
        if (node->prev == NULL)
            cdf->head = node->next;
        lst_delete_lnode(cdf, node);
    }
    return (int)found;
}

int get_cdataframe_cols_size(CDATAFRAME *cdf)
{
    LNODE *node = cdf->head;
    int c = 0;
    while (node->next != NULL)
    {
        c++;
        node = node->next;
    }
    return c;
}

void display_cdataframe(CDATAFRAME *cdf, int index, int strsize)
{
    LNODE *current = cdf->head;
    while (current != NULL)
    {
        print_col(current->data, index, strsize);
        current = current->next;
    }
}

void add_row(CDATAFRAME *cdf, void **values)
{
    int s = 0;
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        insert_value(n->data, values + s);
        s++;
        n = n->next;
    }
}

void delete_row(CDATAFRAME *cdf, unsigned int index)
{
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        delete_value(n->data, index);
        n = n->next;
    }
}

void add_column_fa(CDATAFRAME *cdf, unsigned int index, char *title, unsigned int typesize, ENUM_TYPE *types)
{
    LNODE *n = cdf->head;
    COLUMN *col = create_column_fa(title, typesize, types);
    LNODE *nn = lst_create_lnode(col);
    lst_insert_after(cdf, nn, n);
}

void add_column(CDATAFRAME *cdf, unsigned int index, char *title, unsigned int typesize, ...)
{
    va_list args;
    va_start(args, typesize);
    ENUM_TYPE *type = (ENUM_TYPE *)malloc(sizeof(ENUM_TYPE) * typesize);
    ENUM_TYPE typev;
    for (int i = 0; i < typesize; i++)
    {
        typev = va_arg(args, ENUM_TYPE);
        type[i] = typev;
    }
    return add_column_fa(cdf, index, title, typesize, type);
}

void delete_column(CDATAFRAME *cdf, unsigned int index)
{
    LNODE *n = get_element(cdf, index);
    lst_delete_lnode(cdf, n);
}

int value_exists(CDATAFRAME *cdf, void *data)
{
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        if (are_occurences_of(n->data, data) > 0)
            return 1;
        n = n->next;
    }
    return 0;
}

void *get_value(CDATAFRAME *cdf, unsigned int row, unsigned int col)
{
    LNODE *n = get_element(cdf, col);
    void *res = value_at(n->data, row);
    return res;
}

void display_names(CDATAFRAME *cdf)
{
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        printf("%s\n", n->data->title);
        n = n->next;
    }
}

int number_rows(CDATAFRAME *cdf)
{
    return cdf->head->data->size;
}

int number_cols(CDATAFRAME *cdf)
{
    int c = 0;
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        n = n->next;
        c++;
    }
    return c;
}

int occurences_of(CDATAFRAME *cdf, void *data)
{
    int c = 0;
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        c += are_occurences_of(n->data, data);
        n = n->next;
    }
    return c;
}

int greater_than(CDATAFRAME *cdf, void *data)
{
    int c = 0;
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        c += are_greater_than(n->data, data);
        n = n->next;
    }
    return c;
}

int less_than(CDATAFRAME *cdf, void *data)
{
    int c = 0;
    LNODE *n = cdf->head;
    while (n != NULL)
    {
        c += are_less_than(n->data, data);
        n = n->next;
    }
    return c;
}

CDATAFRAME *load_from_csv(char *file_name, ENUM_TYPE *dftypes, int size)
{
    int *widths;
    int height;
    char ***csv = load_csv(file_name, &widths, &height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < widths[i]; j++)
        {
            char *nsdata = csv[i][j];
            int s;
            char **sdata = split(nsdata, ',', &s);
            ENUM_TYPE t[s];
            void **d[s];
            for (int k = 0; k < s; k++)
            {
                char *data = sdata[k];
                ENUM_TYPE type = dftypes[i * s + k];
                t[k] = type;
                void *ptr;
                switch (type)
                {
                // case UINT:
                //     unsigned int val;
                //     ptr = (void *)&val;
                //     sscanf(data, "%u", ptr);
                //     break;

                // case INT:
                //     int val;
                //     ptr = &val;
                //     sscanf(data, "%d", ptr);
                //     break;

                // case CHAR:
                //     char val;
                //     ptr = &val;
                //     sscanf(data, "%c", ptr);
                //     break;

                // case FLOAT:
                //     float val;
                //     ptr = &val;
                //     sscanf(data, "%f", ptr);
                //     break;

                // case DOUBLE:
                //     double val;
                //     ptr = &val;
                //     sscanf(data, "%lf", ptr);
                //     break;

                // case STRING:
                //     char *val;
                //     ptr = &val;
                //     sscanf(data, "%s", val);
                //     break;

                default:
                    break;
                }
                d[k] = ptr;
            }
            
        }
    }
}
