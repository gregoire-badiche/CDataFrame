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
    COLUMN *mycol = create_column("String column", 1, STRING);
    char *a = "Lima";
    char *b = "Bravo";
    char *c = "Zulu";
    char *d = "Tango";
    insert_value(mycol, &a);
    insert_value(mycol, &b);
    insert_value(mycol, &c);
    insert_value(mycol, &d);
    printf("Column content before sorting : \n");
    print_col(mycol, -1, 30);
    sort(mycol, DESC);
    printf("Column content after sorting : \n");
    print_col_by_index(mycol, -1, 30);
    return 0;
}
