#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include "list.h"

typedef LIST CDATAFRAME;

/**
 * @brief Creates a dataframe
 * @param cdftype The type of the data
 * @param size The size of the list
 * @return A pointer to the dataframe
 */
CDATAFRAME *create_cdataframe_fa(ENUM_TYPE *cdftype, int typesize, int size);

/**
 * @brief Variatic wrapper for the create_cdataframe_fa function
 */
CDATAFRAME *create_cdataframe(int size, int typesize, ...);

/**
 * @brief Column deletion
 * @param cdf Pointer to the CDataframe to delete
 */
void delete_cdataframe(CDATAFRAME **cdf);

/**
 * @brief Delete column by name
 * @param cdf Pointer to the CDataframe
 * @param col_name Column name
 * @return success
 */
int delete_column_by_name(CDATAFRAME *cdf, char *col_name);

/**
 * @brief Number of columns
 * @param cdf Point to the CDataframe
 * @return Number of columns in the CDataframe
 */
int get_cdataframe_cols_size(CDATAFRAME *cdf);

/**
 * @brief Prints the whole dataframe
 * @param cdf Point to the CDataframe
 * @param index The number of rows to be printed. If -1 then all the rows are printed
 * @param strsize The max size of the string to be printed
 */
void display_cdataframe(CDATAFRAME *cdf, int index, int strsize);

/**
 * @brief Add a rown to a dataframe
 * @param cdf A pointer to the CDataFrame
 * @param values An array of pointers to values
*/
void add_row(CDATAFRAME *cdf, void **values);

void delete_row(CDATAFRAME *cdf, unsigned int index);

void add_column_fa(CDATAFRAME *cdf, unsigned int index, char *title, unsigned int typesize, ENUM_TYPE *types);

void add_column(CDATAFRAME *cdf, unsigned int index, char *title, unsigned int typesize, ...);

void delete_column(CDATAFRAME *cdf, unsigned int index);

/**
 * @brief
 * @return
*/
int value_exists(CDATAFRAME *cdf, void *data);

void *get_value(CDATAFRAME *cdf, unsigned int row, unsigned int col);

void display_names(CDATAFRAME *cdf);

int number_rows(CDATAFRAME *cdf);

int number_cols(CDATAFRAME *cdf);

int occurences_of(CDATAFRAME *cdf, void *data);

int greater_than(CDATAFRAME *cdf, void *data);

int less_than(CDATAFRAME *cdf, void *data);

/**
 * @brief Create a CDataframe from csvfile
 * @param CSV filename
 * @param Array of types
 * @param Size of array
 */
CDATAFRAME *load_from_csv(char *file_name, ENUM_TYPE *dftype, int size);

#endif