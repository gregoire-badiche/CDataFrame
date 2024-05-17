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
    CDATAFRAME *cdf = load_from_csv("/home/gregoire/Development/CDataFrame/data.csv", 5, CHAR, STRING, INT, FLOAT);
    display_cdataframe(cdf, -1, 30);
    save_into_csv(cdf, "/home/gregoire/Development/CDataFrame/data2.csv");
    return 0;
}
