#ifndef LIST_H
#define LIST_H

#include "column.h"

/**
 * List node element
 */
typedef struct lnode_
{
    COLUMN* data; // Pointer to a column
    struct lnode_ *prev;
    struct lnode_ *next;
} LNODE;

/**
 * List element
 */
typedef struct list_
{
    LNODE *head;
    LNODE *tail;
} LIST;

void delete_node(LNODE* n);

#endif