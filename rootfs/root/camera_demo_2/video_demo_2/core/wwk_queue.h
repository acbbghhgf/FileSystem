#ifndef WWK_QUEUE_H
#define WWK_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wwk_type.h"

#define wwk_queue_init(q) memset(q,0,sizeof(*q))

typedef struct wwk_queue_node
{
    struct wwk_queue_node *next;
    struct wwk_queue_node *prev;
}wwk_queue_node_t;

typedef struct wwk_queue{
    wwk_queue_node_t *pop;
    wwk_queue_node_t *push;
    int length;
    void *data;
}wwk_queue_t;

/*
* create new queue, just malloc queue memory and init.
*/
wwk_queue_t* wwk_queue_new();
/*
* delete a queue, free malloc memory
*/
int wwk_queue_delete(wwk_queue_t *q);
/*
* define queue push function
*/
int wwk_queue_push(wwk_queue_t *q, wwk_queue_node_t *n);
/*
* define queue pop function
*/
wwk_queue_node_t* wwk_queue_pop(wwk_queue_t *q);


/**************test function****************/
void wwk_queue_test(void);

#ifdef __cplusplus
};
#endif

#endif