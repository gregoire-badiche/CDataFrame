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

typedef struct column
{
    char *title;
    unsigned int size;     // logical size
    unsigned int max_size; // physical size
    unsigned int datasize; // The size of the `ENUM_TYPE* type` array
    ENUM_TYPE *type;
    void **data;                   // array of pointers to stored data
    unsigned long long int *index; // array of integers
    // index valid
    // 0 : no index
    // -1 : invalid index
    // 1 : valid index
    int valid_index;
    // direction de tri Ascendant ou Déscendant
    // 0 : ASC
    // 1 : DESC
    int sorting_dir;
} COLUMN;

/**
 * @brief `create_column_from_array` : Create a new column described with an array of types
 * @param title Column title
 * @param datasize The number of types in the data
 * @param datatypes all the `ENUM_TYPE` types
 * @return Pointer to the created column
 */
COLUMN *create_column_fa(char *title, int datasize, ENUM_TYPE *datatypes);

/**
 * @brief Create a new column
 * @param title Column title
 * @param datasize The number of types in the data
 * @param ... all the `ENUM_TYPE` types
 * @return Pointer to the created column
 */
COLUMN *create_column(char *title, int datasize, ...);

/**
 * @brief Insert a new value into a column
 * @param col Pointer to the column
 * @param value Pointer to the value to insert
 * @return 1 if the value is correctly inserted 0 otherwise
 */
int insert_value(COLUMN *col, void *value);

void delete_value(COLUMN *col, unsigned int index);

/**
 * @brief Free the space allocated by a column
 * @param col Pointer to the column
 */
void free_column(COLUMN **col);

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
void print_col(COLUMN *col, int index, int size);

void print_col_by_index(COLUMN *col, int index, int size);

/**
 * @brief Compare two values
 * @param s1 Pointer to the first value
 * @param s2 Pointer to the second value
 * @param types A pointer to an array of the types composing the two structures
 * @param size The size of the array
 * @return 1 if s1 > s2 ; 0 if s1 = s2 ; -1 if s1 < s2
 */
int compare_val(void *s1, void *s2, ENUM_TYPE *types, int size);

/**
 * @brief Return the value present at position n
 * @param col The pointer to the column
 * @param n The position of the value
 * @return A `void` pointer to the value, or NULL if the value doesn't exists
 */
void *value_at(COLUMN *col, int n);

/**
 * @brief Return the number of occurrences of a value
 * @param col The pointer to the column
 * @param val The pointer to the value
 * @return The number of occurrences of a value
 */
int are_occurences_of(COLUMN *col, void *val);

/**
 * @brief Return the number of values that are greater than one provided
 * @param col The pointer to the column
 * @param val The pointer to the value
 * @return The number of values that are greater
 */
int are_less_than(COLUMN *col, void *val);

/**
 * @brief Return the number of values that are less than one provided
 * @param col The pointer to the column
 * @param val The pointer to the value
 * @return The number of values that are less
 */
int are_greater_than(COLUMN *col, void *val);

/**
 * @brief Remove the index of a column
 * @param Pointer to the column
 */
void erase_index(COLUMN *col);

/**
 * @brief Check if an index is correct
 * @param Pointer to the column
 * @return 0: index not existing, -1: the index exists but invalid, 1: the index is correct
 */
int check_index(COLUMN *col);

/**
 * @brief Update the index
 * @param Pointer to the column
 */
void update_index(COLUMN *col);

/**
 * @brief Check if a value exists in a column
 * @param Pointer to the column
 * @param Pointer to the value to search for
 * @return -1: column not sorted, 0: value not found, 1: value found
 */
int search_value_in_column(COLUMN *col, void *val);

#endif
