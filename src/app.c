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
    ENUM_TYPE types[5] = {CHAR, STRING, INT, FLOAT};
    CDATAFRAME *cdf = load_from_csv("/home/gregoire/Development/CDataFrame/data.csv", types, 5);
    display_cdataframe(cdf, -1, 30);
    return 0;
}
