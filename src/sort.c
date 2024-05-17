#include "sort.h"
#include "column.h"

/**
 * @brief Swaps to values and their corresponding indexes
 * @param p1 A pointer to the 1st value
 * @param p2 A pointer to the 2nd value
 */
void swap(unsigned long long int *p1, unsigned long long int *p2);

int partition(COLUMN* col, unsigned int left, unsigned int right, int d);

void quicksort(COLUMN *col, unsigned int left, unsigned int right, int d)
{
    unsigned long long int *tab = col->index;
    if(left < right && left >= 0)
    {
        int pi = partition(col, left, right, d);
        quicksort(col, left, pi - 1, d);
        quicksort(col, pi + 1, right, d);
    }
}

int partition(COLUMN* col, unsigned int left, unsigned int right, int d)
{
    unsigned long long int *tab = col->index;
    void *pivot = value_at(col, tab[right]);
    int i = left;
    for(int j = left; j <= right - 1; j++)
    {
        int c = compare_val(value_at(col, tab[j]), pivot, col->type, col->datasize);
        if(c != d)
        {
            swap(&tab[i], &tab[j]);
            i++;
        }
    }
    swap(&tab[i], &tab[right]);
    return i;
}

void swap(unsigned long long int *p1, unsigned long long int *p2)
{
    unsigned long long int tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void insertionsort(COLUMN *col, int d)
{
    unsigned long long int *tab = col->index;
    int size = col->size;
    for(int i = 1; i < size; i++)
    {
        void *k = value_at(col, tab[i]);
        int kb = tab[i];
        int j = i - 1;
        while(j >= 0 && compare_val(value_at(col, tab[j]), k, col->type, col->size) == d)
        {
            tab[j + 1] = tab[j];
            j--;
        }
        tab[j + 1] = kb;
    }
}

void sort(COLUMN* col, int sort_dir)
{
    if(col->valid_index == -1)
    {
        quicksort(col, 0, col->size - 1, sort_dir);
    }
    else
    {
        insertionsort(col, sort_dir);
    }
    col->valid_index = IS_SORTED;
    col->sorting_dir = sort_dir;
}
