#ifndef COLUMN_H
#define COLUMN_H

#define REALLOC_SIZE 256

typedef enum
{
    NULLVAL = 1,
    UINT,
    INT,
    CHAR,
    FLOAT,
    DOUBLE,
    STRING,
    STRUCTURE
} ENUM_TYPE;

// typedef union
// {
//     unsigned int uint_value;
//     signed int int_value;
//     char char_value;
//     float float_value;
//     double double_value;
//     char *string_value;
//     void *struct_value;
// } COL_TYPE;

typedef struct column
{
    char* title;
    unsigned int size;     // logical size
    unsigned int max_size; // physical size
    unsigned int datasize; // The size of the `ENUM_TYPE* type` array
    ENUM_TYPE* type;
    void** data;               // array of pointers to stored data
    unsigned long long int* index; // array of integers
} COLUMN;

/**
* @brief Create a new column
* @param title Column title
* @param datasize The number of types in the data
* @param ... all the `ENUM_TYPE` types
* @return Pointer to the created column
*/
COLUMN* create_column(char* title, int datasize, ...);

/**
* @brief Insert a new value into a column
* @param col Pointer to the column
* @param value Pointer to the value to insert
* @return 1 if the value is correctly inserted 0 otherwise
*/
int insert_value(COLUMN *col, void *value);

/**
* @brief Free the space allocated by a column
* @param col Pointer to the column
*/
void free_column(COLUMN** col);

/**
* @brief Convert a value into a string
* @param col Pointer to the column
* @param i Position of the value in the data array
* @param str The string in which the value will be written
* @param size Maximum size of the string
*/
void convert_value(COLUMN *col, unsigned long long int i, char *str, int size);

/**
* @brief Display the content of a column
* @param col Pointer to the column to display
* @param size The size max (exclusive) of the string to be printed
*/
void print_col(COLUMN* col, int size);

#endif
