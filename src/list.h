#ifndef LIST_H
#define LIST_H

#include "column.h"

/**
 * Élément LNODE
 */
typedef struct LNODE_
{
    COLUMN *data;
    struct LNODE_ *prev;
    struct LNODE_ *next;
} LNODE;

/**
 * Une LISTe
 */
typedef struct LIST_
{
    LNODE *head;
    LNODE *tail;
} LIST;

/**
 * création d'un noeud
 */
LNODE *lst_create_lnode(void *dat);

/**
 * crée la LISTe et retourne un pointeur sur cette dernière
 */
LIST *lst_create_list();

/**
 * supprimer la LISTe
 */
void lst_delete_list(LIST *lst);

/**
 * Insère pnew au début de la LISTe lst
 */
void lst_insert_head(LIST *lst, LNODE *pnew);

/**
 * Insère pnew à la fin de la LISTe lst
 */
void lst_insert_tail(LIST *lst, LNODE *pnew);

/**
 * Insère l'élément pnew juste après ptr dans la LISTe lst
 */
void lst_insert_after(LIST *lst, LNODE *pnew, LNODE *ptr);

/**
 * Supprime le premier élément de la LISTe
 */
void lst_delete_head(LIST *lst);

/**
 * Supprime le dernier élément de la LISTe
 */
void lst_delete_tail(LIST *lst);

/**
 * Supprime le LNODE pointé par ptr
 */
void lst_delete_lnode(LIST *lst, LNODE *ptr);

/**
 * Supprime tous les éléments de la LISTe lst
 */
void lst_erase(LIST *lst);

/**
 * retourne le premier node s'il existe sinon NULL
 */
LNODE *get_first_node(LIST *lst);

/**
 * retourne le denier node s'il existe sinon NULL
 */
LNODE *get_last_node(LIST *lst);

/**
 * retourne le node suivant
 */
LNODE *get_next_node(LIST *lst, LNODE *LNODE);

/**
 * retourne le node precedent
 */
void *get_previous_elem(LIST *lst, LNODE *LNODE);

LNODE *get_element(LIST *lst, unsigned int index);

#endif
