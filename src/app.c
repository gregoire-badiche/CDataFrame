#include <stdio.h>
#include "column.h"

typedef struct _x
{
    int b;
    char *a;
} x;


int main(int argc, char const *argv[])
{
    COLUMN *mycol = create_column("Column 1", 2, INT, STRING);
    x a = {1, "abcd"};
    x c = {1, "abc"};
    insert_value(mycol, &a);
    insert_value(mycol, NULL);
    insert_value(mycol, &c);
    print_col(mycol, 30);
    int res;
    res = compare_val(mycol->data[0], mycol->data[2], mycol->type, mycol->datasize);
    printf("%d", res);
    return 0;
}
