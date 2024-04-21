#include <stdio.h>
#include "cdataframe.h"

typedef struct _x
{
    int b;
    char *a;
} x;


int main(int argc, char const *argv[])
{
    CDATAFRAME *cdf = create_cdataframe(1);
    
    return 0;
}
