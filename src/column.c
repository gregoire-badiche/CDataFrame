#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "column.h"
#include "sort.h"

/**
 * @brief Compares to blobs of data bytes by bytes
 * @param d1 Pointer to the first blob of data
 * @param d2 Pointer to the second blob of data
 * @param size Size of the blobs
 * @return 1 if d1 > d2 ; 0 if d1 = d2 ; -1 if d1 < d2
 */
int compare_data(void *d1, void *d2, int size);

/**
 * @brief Writes the data at the provided address, depending on its type
 * @param addr The address to write at
 * @param data The data to be written
 * @param type The type of the data
*/
void write_data(void *addr, void* data, ENUM_TYPE type);

/**
 * @brief Converts a value into a string
 * @param str The string to be written
 * @param size The size of the string
 * @param type The type of the data
 * @param data The data to be converted
 * @return Error code
*/
int convert_var(char *str, int size, ENUM_TYPE type, void *data);

/**
 * @brief Converts a struct into a string
 * @param str The string to be written
 * @param size The size of the string
 * @param data The data to be written
 * @param typec The number of types composing the struct
 * @param typev The types composing the struct
*/
void convert_struct(char *str, int size, void *data, int typec, ENUM_TYPE *typev);

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

    default:
        break;
    }
    return s;
}

COLUMN *create_column_fa(char *title, int datasize, ENUM_TYPE* datatypes)
{
    COLUMN *col = (COLUMN *)malloc(sizeof(COLUMN));
    if (col != NULL)
    {
        col->title = title;
        col->size = 0;
        col->max_size = 0;
        col->max_size = REALLOC_SIZE;
        col->datasize = datasize;
        col->type = datatypes;
        col->data = NULL;
        col->index = NULL;
        col->valid_index = NOT_SORTED;
        col->sorting_dir = ASC;
    }
    return col;
};

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
    return create_column_fa(title, datasize, type);
}

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
        float p = *((float *)addr);
        break;

    case DOUBLE:
        *((double *)addr) = *((double *)data);
        break;

    case STRING:
        char *str = *((char **)data);
        unsigned int size = 0;
        while (str[size] != '\0')
        {
            size++;
        }
        size++;
        char *target = (char *)malloc(size * sizeof(char));
        *((char **)addr) = target;
        for (unsigned int i = 0; i < size; i++)
        {
            target[i] = str[i];
        }
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
        col->index = (long long unsigned int *)malloc(REALLOC_SIZE);
        col->size = 0;
        col->max_size = REALLOC_SIZE;
    }
    // If the list doesn't have any space left realloc
    if ((col->size + 1) * sizeof(ENUM_TYPE *) > col->max_size)
    {
        col->max_size += REALLOC_SIZE;
        col->data = (void **)realloc(col->data, col->max_size);
        if(col->valid_index != INDEX_DELETED)
            col->index = (long long unsigned int *)realloc(col->index, col->max_size);
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
            update_size(&s, i, col->type, col->datasize);
            // s += get_size(col->type[i]);
        }
        col->data[col->size] = (void *)malloc((s) * sizeof(char));
        for (int i = 0; i < col->datasize; i++)
        {
            ENUM_TYPE t = col->type[i];
            write_data(col->data[col->size] + s2, value + s2, t);
            s2 += get_size(t);
            if (t == CHAR && i < col->datasize - 1)
            {
                if (col->type[i + 1] != CHAR)
                {
                    int k = 4 - (s2 % 4);
                    // Adds padding for the char to fit in a memory block, and fills it with 0
                    for (int j = 0; j < k; j++)
                    {
                        write_data(col->data[col->size] + s2, "\0", CHAR);
                        s2++;
                    }
                }
            }
            if (s2 % 8 != 0 && i < col->datasize - 1)
            {
                if (get_size(col->type[i + 1]) == 8)
                {
                    // Adds padding for the value to fit in a memory block, and fills it with 0
                    for (int j = 0; j < 4; j++)
                    {
                        write_data(col->data[col->size] + s2, "\0", CHAR);
                        s2++;
                    }
                }
            }
        }
    }
    if(col->valid_index != INDEX_DELETED)
    {
        col->index[col->size] = col->size;
        if(check_index(col))
        {
            col->valid_index = WAS_SORTED;
            update_index(col);
        }
    }
    col->size++;
    return 1;
};

void delete_value(COLUMN *col, unsigned int index)
{
    free(col->data);
    col->data = NULL;
}

void free_column(COLUMN **col)
{
    for (unsigned int i = 0; i < (*col)->size; i++)
    {
        if((*col)->data[i] != NULL) {
            unsigned long int size = 0;
            for (unsigned int j = 0; j < (*col)->datasize; j++)
            {
                if ((*col)->type[j] == STRING)
                {
                    free(*(char **)((*col)->data[i] + size));
                }
                update_size(&size, j, (*col)->type, (*col)->datasize);
            }
            free((*col)->data[i]);
        }
    }
    free((*col)->data);
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
            i = snprintf(str, size, "%lf", *((double *)data));
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
    unsigned long int bc = 0; // Byte counter
    int wc = 0; // Word counter (for char)
    // If the data is NULL we want to only print NULL once, and not overflow with pointers pointing to nothing
    if (data == NULL)
        typec = 1;
    for (int k = 0; k < typec; k++)
    {
        argv = typev[k];
        char *s = (char *)malloc(sizeof(char) * size);
        int i;
        i = convert_var(s, size, argv, (data + bc));
        update_size(&bc, k, typev, typec);
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

void print_col(COLUMN *col, int index, int size)
{
    printf("%.*s\n", size, col->title);
    char s[size];
    char s2[size];
    for (int i = 0; i < col->size && (i < index || index == -1); i++)
    {
        convert_value(col, i, s, size);
        snprintf(s2, size, "[%d] %s", i, s);
        printf("%s\n", s2);
    }
}

int compare_val(void *s1, void *s2, ENUM_TYPE *types, int size)
{
    if(s1 == NULL)
    {
        return 1;
    }
    unsigned long int s = 0;
    unsigned long int bc = 0;
    char res;
    for (int i = 0; i < size; i++)
    {
        s = get_size(types[i]);
        if (types[i] == STRING)
        {
            unsigned long int j = 0;

            char *str1 = *(char **)(s1 + bc);
            char *str2 = *(char **)(s2 + bc);
            while (str1[j] != 0 && str2[j] != 0)
                j++;
            res = compare_data(str1, str2, j + 1);
        }
        else
        {
            res = compare_data(s1 + bc, s2 + bc, s);
        }
        if (res != 0)
        {
            return res;
        }
        update_size(&bc, i, types, size);
    }
    return 0;
}

int compare_data(void *d1, void *d2, int size)
{
    unsigned long int bc;
    for (bc = 0; bc < size; bc++)
    {
        char a = *(char *)(d1 + bc);
        char b = *(char *)(d2 + bc);
        if (a < b)
            return -1;
        if (a > b)
            return 1;
    }
    return 0;
}

void *value_at(COLUMN *col, int n)
{
    if(n > col->size)
        return NULL;
    return (void *) col->data[n];
}

int are_occurences_of(COLUMN *col, void *val)
{
    unsigned int c = 0;
    for(int i = 0; i < col->size; i++)
    {
        if(col->data[i] == NULL || val == NULL) {
            if(col->data[i] == NULL && val == NULL)
                c++;
        }
        else if(compare_val(val, &col->data[i], col->type, col->datasize) == 0)
            c++;
    }
    return c;
}

int are_less_than(COLUMN *col, void *val)
{
    unsigned int c = 0;
    for(int i = 0; i < col->size; i++)
    {
        if(col->data[i] != NULL && val != NULL)
            if(compare_val(val, &col->data[i], col->type, col->datasize) == 1)
                c++;
    }
    return c;
}

int are_greater_than(COLUMN *col, void *val)
{
    unsigned int c = 0;
    for(int i = 0; i < col->size; i++)
    {
        if(col->data[i] != NULL && val != NULL)
            if(compare_val(val, &col->data[i], col->type, col->datasize) == -1)
                c++;
    }
    return c;
}

void print_col_by_index(COLUMN *col, int index, int size)
{
    printf("%.*s\n", size, col->title);
    char s[size];
    char s2[size];
    for (int i = 0; i < col->size && (i < index || index == -1); i++)
    {
        int it = col->index[i];
        convert_value(col, it, s, size);
        snprintf(s2, size, "[%d] %s", it, s);
        printf("%s\n", s2);
    }
}

void erase_index(COLUMN *col)
{
    free(col->index);
    col->index = NULL;
    col->valid_index = INDEX_DELETED;
}

int check_index(COLUMN *col)
{
    return !(col->valid_index == INDEX_DELETED || col->valid_index == NOT_SORTED);
}

void update_index(COLUMN *col)
{
    sort(col, col->sorting_dir);
}

int search_value_in_column(COLUMN *col, void *val)
{
    if(col->valid_index != IS_SORTED)
        return -1;
    unsigned int hi = col->size;
    unsigned int lo = 0;
    while (hi != lo)
    {
        unsigned int pi = (hi - lo) / 2 + lo;
        if(pi == hi || pi == lo)
        {
            return 0;
        }
        int c = compare_val(value_at(col, pi), val, col->type, col->datasize);
        if(c == -1)
        {
            hi = pi;
        }
        else if(c == 1)
        {
            lo = pi;
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

void update_size(unsigned long int *bc, int index, ENUM_TYPE* types, int size)
{
    *bc += get_size(types[index]);
    // Adds padding for CHAR
    if (index < size - 1 && types[index] == CHAR)
    {
        if (types[index + 1] != CHAR)
        {
            *bc += 4 - (*bc % 4);
        }
    }
    // Adds padding to fit in 8 block
    if (*bc % 8 != 0 && index < size - 1)
    {
        if (get_size(types[index + 1]) == 8)
        {
            *bc += 4;
        }
    }
}
