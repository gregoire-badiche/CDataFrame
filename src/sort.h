#ifndef SORT_H
#define SORT_H

#include "column.h"

#define ASC 1
#define DESC -1

#define INDEX_DELETED 0
#define NOT_SORTED 1
#define WAS_SORTED 2
#define IS_SORTED 3

/**
 * @brief Quicksorts the data indexes
 * @param col The column
 * @param left
 * @param right
 * @param d
 */
void quicksort(COLUMN *col, unsigned int left, unsigned int right, int d);

/**
 * @brief Sorts the data indexes
 */
void insertionsort(COLUMN *col, int d);

/**
 * @brief: Sort a column according to a given order
 * @param: Pointer to the column to sort
 * @param: Sort type (ASC or DESC)
 */
void sort(COLUMN *col, int sort_dir);

#endif
