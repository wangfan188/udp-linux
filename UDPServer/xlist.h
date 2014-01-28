//
//  xlist.h
//  effectslib
//
//  Created by xi penggang on 1/31/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef effectslib_xlist_h
#define effectslib_xlist_h
#include <pthread.h>

//xlist:the list 
typedef struct stru_xnode {
    void*                   data;
    void*                   expand;      //扩展
    struct stru_xnode       *next;
    struct stru_xnode       *last;
}xnode;

typedef struct stru_xlist {
    struct stru_xnode       *listfrout;
    struct stru_xnode       *listback;
    int                     ncout;
    pthread_mutex_t         mutex;
}xlist;

extern xlist*
xlist_create();

extern int
xlist_clear(xlist *list);

extern int
xlist_destroy(xlist *list);

extern int
xlist_push(xlist* list, xnode* node);

extern xnode*
xlist_pop(xlist* list);

xnode*
xlist_createnode();

extern int 
xlist_deletenode(xnode* node);

extern int
xlist_getcout(xlist *list);

#endif
