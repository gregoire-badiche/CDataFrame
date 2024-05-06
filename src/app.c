#include <stdio.h>
#include "column.h"
#include "cdataframe.h"

typedef struct _x
{
    int b;
    char *a;
    char c;
    float d;
    char* e;
} x;

int main(int argc, char const *argv[])
{
    COLUMN *mycol = create_column("Column 1", 5, INT, STRING, CHAR, FLOAT, STRING);
    x a = {1, "Hello", 'a', 3.87, "enorme"};
    x c = {128, "World!", 'b', 64.42902, "pouleto dodu"};
    insert_value(mycol, &a);
    insert_value(mycol, NULL);
    insert_value(mycol, &c);
    
    CDATAFRAME* cdf = create_cdataframe(1, 5, INT, STRING, CHAR, FLOAT, STRING);

    return 0;
}
