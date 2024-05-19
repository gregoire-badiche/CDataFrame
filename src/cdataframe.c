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
    cdf->tail = prev;
    prev->prev = NULL;
    prev->data = create_column_fa(NULL, typesize, cdftype);
    prev->next = NULL;
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

CDATAFRAME *load_from_csv_fa(char *file_name, ENUM_TYPE *dftypes, int size)
{
    int stringscounter = 0;
    for(int i = 0; i < size; i++)
        if(dftypes[i] == STRING)
            stringscounter++;

    int *widths = NULL;
    int height;
    char ***csv = load_csv(file_name, &widths, &height);
    stringscounter *= height;
    char **stringspointers = (char **)malloc(sizeof(char *) * stringscounter);
    stringscounter = 0;
    CDATAFRAME *cdf = create_cdataframe(1, 1, INT);
    for (int i = 0; i < height; i++)
    {
        unsigned int tc = 0;
        for (int j = 0; j < widths[i]; j++)
        {
            unsigned int typecounter = 0;
            char *nsdata = csv[i][j];
            int s;
            char **sdata = split(nsdata, ',', &s);
            unsigned long int size = 0;
            // Get the size of the chunk
            for (unsigned int k = 0; k < s; k++)
            {
                int _ = k < s - 1 ? tc + 2 : tc + 1;
                update_size(&size, tc, dftypes, _);
            }
            ENUM_TYPE *t = (ENUM_TYPE *)malloc(sizeof(ENUM_TYPE) * s);
            void *d = malloc((size + 1) * sizeof(char));
            unsigned long int bc = 0;
            for (int k = 0; k < s; k++)
            {
                char *data = sdata[k];
                ENUM_TYPE type = dftypes[tc];
                t[k] = type;
                switch (type)
                {
                case UINT:
                {
                    sscanf(data, "%u", d + bc);
                }
                break;

                case INT:
                {
                    sscanf(data, "%d", d + bc);
                }
                break;

                case CHAR:
                {
                    sscanf(data, "%c", d + bc);
                }
                break;

                case FLOAT:
                {
                    sscanf(data, "%f", d + bc);
                }
                break;

                case DOUBLE:
                {
                    sscanf(data, "%lf", d + bc);
                }
                break;

                case STRING:
                {
                    unsigned int _size = 1;
                    while (data[_size] != '\0')
                    {
                        _size++;
                    }
                    _size++;
                    char *target = (char *)malloc((_size + 1) * sizeof(char));
                    *((char **)(d + bc)) = target;
                    for (unsigned int l = 0; l < _size; l++)
                    {
                        target[l] = data[l];
                    }
                    stringspointers[stringscounter] = target;
                    stringscounter++;
                }
                break;

                default:
                    break;
                }
                {
                    int _ = k < s - 1 ? tc + 2 : tc + 1;
                    update_size(&bc, tc, dftypes, _);
                }
                tc++;
            }
            free_split(&sdata);
            if (i == 0)
            {
                char *title = (char *)malloc(sizeof(char) * 8);
                snprintf(title, 8, "Col %d", j + 1);
                COLUMN *col = create_column_fa(title, s, t);
                insert_value(col, d);
                if (j == 0)
                {
                    cdf->head->data = col;
                }
                else
                {
                    LNODE *n = lst_create_lnode(col);
                    lst_insert_tail(cdf, n);
                }
            }
            else
            {
                LNODE *n = get_element(cdf, j);
                insert_value((COLUMN *)n->data, d);
            }
            typecounter += s;
            free(d);
        }
    }
    free(widths);
    free_load_csv(&csv, height);
    for(int i = 0; i < stringscounter; i++)
    {
        char *k = stringspointers[i];
        free(stringspointers[i]);
    }
    free(stringspointers);
    return cdf;
}

CDATAFRAME *load_from_csv(char *file_name, int size, ...)
{
    va_list args;
    va_start(args, size);
    ENUM_TYPE *type = (ENUM_TYPE *)malloc(sizeof(ENUM_TYPE) * size);
    ENUM_TYPE typev;
    for (int i = 0; i < size; i++)
    {
        typev = va_arg(args, ENUM_TYPE);
        type[i] = typev;
    }
    return load_from_csv_fa(file_name, type, size);
}

char *data_to_csv(FILE *f, void *data, ENUM_TYPE *types, int size)
{
    unsigned long int bc = 0;
    for (int i = 0; i < size; i++)
    {
        switch (types[i])
        {
        case UINT:
            fprintf(f, "%u", *(unsigned int *)(data + bc));
            break;

        case INT:
            fprintf(f, "%i", *(int *)(data + bc));
            break;

        case CHAR:
            fprintf(f, "%c", *(char *)(data + bc));
            break;

        case FLOAT:
            fprintf(f, "%f", *(float *)(data + bc));
            break;

        case DOUBLE:
            fprintf(f, "%lf", *(double *)(data + bc));
            break;

        case STRING:
            fprintf(f, "%s", *(char **)(data + bc));
            break;

        default:
            break;
        }
        if (i < size - 1)
        {
            fprintf(f, ",");
        }
        update_size(&bc, i, types, size);
    }
}

void save_into_csv(CDATAFRAME *cdf, char *file_name)
{
    FILE *f = fopen(file_name, "w+");
    int width = number_cols(cdf);
    int height = number_rows(cdf);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            LNODE *n = get_element(cdf, i);
            COLUMN *col = n->data;
            void *data = get_value(cdf, j, i);
            data_to_csv(f, data, col->type, col->datasize);
            if (i != width - 1)
                fprintf(f, ";");
            else if(j != height - 1)
                fprintf(f, "\n");
        }
    }
}
