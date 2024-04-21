#include <stdlib.h>

#include "list.h"
#include "cdataframe.h"

CDATAFRAME *create_cdataframe(ENUM_TYPE* cdftype, int typesize, int size) {
    CDATAFRAME* cdf;
    cdf = (CDATAFRAME *) malloc(sizeof(CDATAFRAME));
    LNODE* prev = (LNODE *) malloc(sizeof(LNODE));
    LNODE* current = (LNODE *) malloc(sizeof(LNODE));
    cdf->head = prev;
    prev->prev = NULL;
    prev->data = create_column(NULL, typesize, cdftype);
    for(int i = 1; i < size; i++) {
        current->next = NULL;
        current->prev = prev;
        current->data = create_column(NULL, typesize, cdftype);
        prev->next = current;
    }
    cdf->tail = current;
    return cdf;
}

void delete_cdataframe(CDATAFRAME **cdf) {
    // Free the dataframe, all the lists
    LNODE* c = (*cdf)->head;
    LNODE* n = c->next;
    while(n != NULL) {
        free_column(&c->data);
        free(c);
        c = n;
        n = c->next;
    }
    free(*cdf);
    *cdf = NULL;
}

int delete_column(CDATAFRAME *cdf, char *col_name) {
    LNODE* node = cdf->head;
    char found = 0;
    while(node->next != NULL && !found) {
        if(node->data->title == col_name) {
            found = 1;
        } else {
            node = node->next;
        }
    }
    if(found) {
        node->prev->next = node->next;
        if(node->next == NULL) 
            cdf->tail = node->prev;
        node->next->prev = node->prev;
        if(node->prev == NULL)
            cdf->head = node->next;
        delete_node(node);
    }
    return (int) found;
}

int get_cdataframe_cols_size(CDATAFRAME *cdf) {
    LNODE* node = cdf->head;
    int c = 0;
    while(node->next != NULL) {
        c++;
        node = node->next;
    }
    return c;
}

void display_cdataframe(CDATAFRAME *cdf)
{
    LNODE* current = cdf->head;
    while(current != NULL)
        print_col(current->data, 30);
}
