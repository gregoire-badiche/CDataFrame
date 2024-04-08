#include <stdio.h>
#include "column.h"

typedef struct _x
{
    int a;
    char b;
    char c;
    char* d;
    float e;
    int f;
} x;


int main(int argc, char const *argv[])
{
    COLUMN *mycol = create_column("Column 1", 6, INT, CHAR, CHAR, STRING, FLOAT, INT);
    x a = {1, 'a', 'b', "hello", .3, 2};
    x c = {2, 'b', 'c', "world", .2, 57};
    insert_value(mycol, &a);
    insert_value(mycol, NULL);
    insert_value(mycol, &c);
    print_col(mycol, 30);
    return 0;
}
