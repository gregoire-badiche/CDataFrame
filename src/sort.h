#ifndef SORT_H
#define SORT_H

#include "column.h"

#define ASC 0
#define DESC 1

/**
 * @brief Quicksorts the data, and keeps track of the original indexes of the data
 * @param size The size of the array
 * @param data NOT MODIFIED, the data to be sorted
 * @param index MODIFIED, the original indexes of the data
 */
void quicksort(int size, void **data, int *index);

/**
 * @brief Sorts the data, and keeps track of the original indexes of the data
 * @param size The size of the array
 * @param data NOT MODIFIED, the data to be sorted
 * @param index MODIFIED, the original indexes of the data
 */
void insertion_sort(int size, void **data, int *index);

/**
 * @brief: Sort a column according to a given order
 * @param: Pointer to the column to sort
 * @param: Sort type (ASC or DESC)
 */
void sort(COLUMN *col, int sort_dir);

/**
 * @brief: Display the content of a sorted column
 * @param: Pointer to a column
 */
void print_col_by_index(COLUMN *col);

#endif
