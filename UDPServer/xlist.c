//
//  xlist.c
//  effectslib
//
//  Created by xi penggang on 1/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "xlist.h"

xlist*
xlist_create()
{
    xlist* list = (xlist*)malloc(sizeof(xlist));
    if (!list) {return NULL;}
    
    list->listback = NULL;
    list->listfrout = NULL;
    list->ncout = 0;
    pthread_mutex_init(&list->mutex, NULL);
    return list;
}

int
xlist_clear(xlist *list)
{
    if (!list) {return -1;}
    pthread_mutex_lock(&list->mutex);
    while (list->ncout > 0) {
        xnode *p = list->listback;
        if (p) {
            list->listback = p->last;
            if (!(p->last)) { list->listfrout = NULL; }
            list->ncout--;
            xlist_deletenode(p);
        }
    }
    pthread_mutex_unlock(&list->mutex);
    return 0;
}

int
xlist_destroy(xlist *list)
{
    if (!list) {return -1;}
    xlist_clear(list);
    
    pthread_mutex_destroy(&list->mutex);
    free(list);
    return 0;
}

int
xlist_push(xlist* list, xnode* node)
{
    if (!list || !node) {return -1;}
    pthread_mutex_lock(&list->mutex);
    xnode *p = list->listfrout;
    list->listfrout = node;
    list->listfrout->last = NULL;
    list->listfrout->next = p;
    if (p) { p->last = list->listfrout; }
    if (!(list->listback)) { list->listback = list->listfrout; }
    list->ncout++;
    pthread_mutex_unlock(&list->mutex);
    return 0;
}

xnode*
xlist_pop(xlist* list)
{
    if (!list) {return NULL;}
    if (!(list->listback)) { return NULL; }
    xnode *p = NULL;
    pthread_mutex_lock(&list->mutex);
    p = list->listback;
    list->listback = p->last;
    if (!(p->last)) { list->listfrout = NULL; }
    list->ncout--;
    pthread_mutex_unlock(&list->mutex);
    return p;
}

xnode*
xlist_createnode()
{
    xnode *pret = (xnode*)malloc(sizeof(xnode));
    if (!pret) return NULL;
    
    pret->data = NULL;
    pret->expand = NULL;
    pret->last = NULL;
    pret->next = NULL;
    
    return pret;
}

int 
xlist_deletenode(xnode* node)
{
    if (!node) {return -1;}
    if (node->data) free(node->data);
    if (node->expand) free(node->expand);

    node->data = NULL;
    node->expand = NULL;
    node->last = NULL;
    node->next = NULL;
    free(node);
    return 0;
}

int
xlist_getcout(xlist *list)
{
    if(!list) { return -1; }
    return list->ncout;
}
