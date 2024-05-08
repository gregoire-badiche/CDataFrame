#include <stdio.h>
#include "column.h"
#include "cdataframe.h"
#include "csv.h"

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
    x a = {1, "Hello", 'a', 3.87, "enorme"};
    x c = {128, "World!", 'b', 64.42902, "pouleto dodu"};

    // print_col(mycol, -1, 30);
    
    CDATAFRAME* cdf = create_cdataframe(1, 5, INT, STRING, CHAR, FLOAT, STRING);
    insert_value(cdf->head->data, &a);
    insert_value(cdf->head->data, NULL);
    insert_value(cdf->head->data, &c);
    display_cdataframe(cdf, -1, 30);

    // char ***p = load_csv("/home/gregoire/Development/CDataFrame/data.csv");
    return 0;
}
