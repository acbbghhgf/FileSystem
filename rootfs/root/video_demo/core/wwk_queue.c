#include "wwk_queue.h"

wwk_queue_t* wwk_queue_new()
{
    wwk_queue_t *q;
    q = (wwk_queue_t *)wwk_malloc(sizeof(*q));
    wwk_queue_init(q);
    return q;
}

int wwk_queue_delete(wwk_queue_t *q)
{
    wwk_free(q);
    return 0;
}

int wwk_queue_push(wwk_queue_t *q, wwk_queue_node_t *n)
{
    n->prev = q->push;
    if(q->push)
    {
        q->push->next = n;
    }
    n->next = 0;
    q->push = n;
    if(!q->pop)
    {
        q->pop = n;
    }
    ++q->length;
    return 0;
}

wwk_queue_node_t* wwk_queue_pop(wwk_queue_t *q)
{
    wwk_queue_node_t *n;
    n = 0;
    if(!q->pop){
        goto end;
    }
    n = q->pop;
    q->pop = q->pop->next;
    if(q->pop)
    {
        q->pop->prev = 0;
    }
    else
    {
        q->push = 0;
    }
    --q->length;

end:
    return n;
}

//test queue push and pop
void wwk_queue_test(void)
{
    wwk_debug("xxxxxxxx test start xxxxxxxx\n");
    int i = 0;
    struct wwk_queue_test
    {
        wwk_queue_node_t *n;
        int data;
    };
    wwk_queue_node_t *qn;
    struct wwk_queue_test *t;
    wwk_queue_t *q = wwk_queue_new();
    for (i = 0; i < 5; i++)
    {
        t = wwk_malloc(sizeof(*t));
        t->data = i;
        wwk_queue_push(q, t->n);
    }
    
    for (i = 0; i < 5; i++)
    {
        qn = wwk_queue_pop(q);
        if(!n)
        {
            wwk_debug("node is null.break \n");
            break;
        }
        t = data_offset2(qn, struct wwk_queue_test, n);
        wwk_debug("print t[%d] data = [%d]\n", i, t->data);
        wwk_free(t);
    }
    wwk_debug("xxxxxxxx test end xxxxxxxx\n");
    
}