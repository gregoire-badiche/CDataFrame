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

void add_row();

void delete_row();

void add_column();

void delete_column();

int search();

void* get_value();

void display_names();

int number_rows();

int number_cols();

int occurences_of();

int greater_than();

int less_than();

#endif