#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "column.h"

int get_size(ENUM_TYPE type)
{
    int s = 0;
    switch (type)
    {
    case UINT:
        s = sizeof(int);
        break;

    case INT:
        s = sizeof(int);
        break;

    case CHAR:
        s = sizeof(char);
        break;

    case FLOAT:
        s = sizeof(float);
        break;

    case DOUBLE:
        s = sizeof(double);
        break;

    case STRING:
        s = sizeof(char *);
        break;

    case STRUCTURE:
        s = sizeof(void *);
        break;

    default:
        break;
    }
    return s;
}

COLUMN *create_column(char *title, int datasize, ...)
{
    va_list args;
    va_start(args, datasize);
    ENUM_TYPE *type = (ENUM_TYPE *)malloc(sizeof(ENUM_TYPE) * datasize);
    ENUM_TYPE typev;
    for (int i = 0; i < datasize; i++)
    {
        typev = va_arg(args, ENUM_TYPE);
        type[i] = typev;
    }
    COLUMN *col = (COLUMN *)malloc(sizeof(COLUMN));
    if (col != NULL)
    {
        col->title = title;
        col->size = 0;
        col->max_size = 0;
        col->max_size = REALLOC_SIZE;
        col->datasize = datasize;
        col->type = type;
        col->data = NULL;
        col->index = 0;
    }
    return col;
};

void write_data(void *addr, void *data, ENUM_TYPE type)
{
    int s;
    switch (type)
    {
    case UINT:
        *((unsigned int *)addr) = *((unsigned int *)data);
        break;

    case INT:
        *((int *)addr) = *((int *)data);
        break;

    case CHAR:
        *((char *)addr) = *((char *)data);
        break;

    case FLOAT:
        *((float *)addr) = *((float *)data);
        float p = *((float *) addr);
        break;

    case DOUBLE:
        *((double *)addr) = *((double *)data);
        break;

    // TODO: deep copy of the string ?
    case STRING:
        *((char **)addr) = *((char **)data);
        break;

    case STRUCTURE:
        *((void **)addr) = *((void **)data);
        break;

    default:
        break;
    }
}

int insert_value(COLUMN *col, void *value)
{
    // If the list is empty malloc a chunk
    if (col->data == NULL)
    {
        col->data = (void **)malloc(REALLOC_SIZE);
        col->size = 0;
        col->max_size = REALLOC_SIZE;
    }
    // If the list doesn't have any space left realloc
    if ((col->size + 1) * sizeof(ENUM_TYPE *) > col->max_size)
    {
        col->max_size += REALLOC_SIZE;
        col->data = (void **)realloc(col->data, col->max_size);
    }
    if (value == NULL)
    {
        col->data[col->size] = NULL;
    }
    else
    {
        unsigned long s = 0, s2 = 0;
        for (int i = 0; i < col->datasize; i++)
        {
            s += get_size(col->type[i]);
        }
        col->data[col->size] = (void *)malloc(s);
        for (int i = 0; i < col->datasize; i++)
        {
            ENUM_TYPE t = col->type[i];
            write_data(col->data[col->size] + s2, value + s2, t);
            s2 += get_size(t);
            if(t == CHAR && i < col->datasize - 1) {
                if(col->type[i + 1] != CHAR) {
                    // Adds padding for the char to fit in a memory block
                    s2 += 4 - (s2 % 4);
                }
            }
        }
    }
    col->size++;
    return 1;
};

// ! Not a complete free, memory leak
void free_column(COLUMN **col)
{
    for (int i = 0; i < (*col)->size; i++)
    {
        int s = 0;
        for (int j = 0; j < (*col)->datasize; j++)
        {
            free((*col)->data[i]);
            s += get_size((*col)->type[j]);
        }
        free((*col)->data[i]);
    }
    free(*col);
    *col = NULL;
}

int convert_var(char *str, int size, ENUM_TYPE type, void *data)
{
    int i;
    if (data == NULL)
    {
        i = snprintf(str, size, "%s", "NULL");
    }
    else
    {
        switch (type)
        {
        case UINT:
            i = snprintf(str, size, "%u", *((unsigned int *)data));
            break;

        case INT:
            i = snprintf(str, size, "%d", *((int *)data));
            break;

        case CHAR:
            i = snprintf(str, size, "%c", *((char *)data));
            break;

        case FLOAT:
            i = snprintf(str, size, "%f", *((float *)data));
            break;

        case DOUBLE:
            i = snprintf(str, size, "%d", *((double *)data));
            break;

        case STRING:
            i = snprintf(str, size, "%s", *(char **)data);
            break;
        }
    }
    if (i > size)
        i = size;
    return i;
}

void convert_struct(char *str, int size, void *data, int typec, ENUM_TYPE *typev)
{
    ENUM_TYPE argv;
    int bc = 0; // Byte counter
    int wc = 0; // Word counter (for char)
    // If the data is NULL we want to only print NULL once, and not overflow with pointers pointing to nothing
    if(data == NULL) typec = 1;
    for (int k = 0; k < typec; k++)
    {
        argv = typev[k];
        char *s = (char *)malloc(sizeof(char) * size);
        int i;
        i = convert_var(s, size, argv, (data + bc));
        bc += get_size(argv);
        if(argv == CHAR && k < typec - 1) {
            if(typev[k + 1] != CHAR) {
                // Takes padding into account
                bc += 4 - (bc % 4);
            }
        }
        int j;
        for (j = 0; (j < i) && (wc + j < size); j++)
        {
            str[wc + j] = s[j];
        }
        wc += j;
        if (wc < size - 1 && (k != typec - 1))
        {
            str[wc] = ' ';
            wc++;
        }
        free(s);
    }
    if (wc < size)
        str[wc] = '\0';
    else
        str[size - 1] = '\0';
}

void convert_value(COLUMN *col, unsigned long long int i, char *str, int size)
{
    convert_struct(str, size, col->data[i], col->datasize, col->type);
}

void print_col(COLUMN *col, int size)
{
    printf("%.*s\n", size, col->title);
    char s[size];
    char s2[size];
    for (int i = 0; i < col->size; i++)
    {
        convert_value(col, i, s, size);
        snprintf(s2, size, "[%d] %s", i, s);
        printf("%s\n", s2);
    }
}