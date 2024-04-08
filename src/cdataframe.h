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
CDATAFRAME *create_cdataframe(ENUM_TYPE *cdftype, int size);

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
int delete_column(CDATAFRAME *cdf, char *col_name);

/**
 * @brief Number of columns
 * @param cdf Point to the CDataframe
 * @return Number of columns in the CDataframe
 */
int get_cdataframe_cols_size(CDATAFRAME *cdf);

#endif