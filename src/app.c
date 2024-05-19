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
    ENUM_TYPE t[11] =  {FLOAT, CHAR, STRING, STRING, INT, STRING, INT, STRING, INT, INT, INT};
    CDATAFRAME *cdf = load_from_csv_fa("/home/gregoire/Development/CDataFrame/data.csv", t, 11);
    display_cdataframe(cdf, -1, 100);
    return 0;
}
