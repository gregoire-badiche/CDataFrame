#include <stdlib.h>

#include "list.h"

void delete_node(LNODE* n) {
    free(n->data->data);
    free(n->data);
    free(n);
}
