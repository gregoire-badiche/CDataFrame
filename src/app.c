#include <stdio.h>
#include "column.h"
#include "cdataframe.h"
#include "sort.h"
#include "csv.h"

typedef struct _x
{
    int b;
    char c;
} x;

int main(int argc, char const *argv[])
{
    COLUMN *mycol = create_column("sorted column", 1, INT);
    int a = 52;
    int b = 44;
    int c = 15;
    int d = 18;
    insert_value(mycol, &a);
    insert_value(mycol, &b);
    insert_value(mycol, &c);
    insert_value(mycol, &d);
    ENUM_TYPE p = INT;
    printf("%d %d %d\n", *(int *)mycol->data[0], *(int *)mycol->data[1], compare_val(mycol->data[0], mycol->data[1], &p, 1));
    printf("Column content before sorting : \n");
    print_col(mycol, -1, 30);
    sort(mycol, ASC);
    printf("Column content after sorting : \n");
    print_col_by_index(mycol, -1, 30);
    return 0;
}
