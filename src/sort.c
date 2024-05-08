#include "sort.h"
#include "column.h"

/**
 * @brief Swaps to values and their corresponding indexes
 * @param p1 A pointer to the 1st value
 * @param p2 A pointer to the 2nd value
 */
void swap(unsigned int *p1, unsigned int *p2);

int partition(COLUMN* col, unsigned int left, unsigned int right);

void quicksort(COLUMN *col, unsigned int left, unsigned int right, int d)
{
    int *tab = col->index;
    if(left < right)
    {
        int pi;
        if(d == -1)
        {
            pi = partition(col, right, left);
        }
        else
        {
            pi = partition(col, left, right);
        }
        quicksort(col, left, pi - 1, d);
        quicksort(col, pi + 1, right, d);
    }
}

int partition(COLUMN* col, unsigned int left, unsigned int right)
{
    int *tab = col->index;
    void *pivot = value_at(col, tab[right]);
    int i = left - 1;
    for(int j = left; j < right - 1; j++)
    {
        int c = compare_val(value_at(col, tab[j]), pivot, col->type, col->datasize);
        if(c == 0 || c == -1)
        {
            i++;
            swap(&tab[i], &tab[j]);
        }
    }
    swap(&tab[i + 1], &tab[right]);
    return i + 1;
}

void swap(unsigned int *p1, unsigned int *p2)
{
    unsigned int tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

void insertionsort(COLUMN *col, int d)
{
    int *tab = col->index;
    int size = col->size;
    for(int i = 2; i < size; i++)
    {
        void *k = value_at(col, tab[i]);
        int kb = tab[i];
        int j = i - 1;
        int c = compare_val(value_at(col, tab[j]), k, col->type, col->size);
        while(j > 0 && c == d)
        {
            tab[j + 1] = tab[j];
            j--;
        }
        tab[j + 1] = kb;
    }
}

void sort(COLUMN* col, int sort_dir)
{
    if(col->valid_index == 0)
    {
        quicksort(col, 0, col->size, sort_dir);
    }
    else
    {
        insertionsort(col, sort_dir);
    }
}
