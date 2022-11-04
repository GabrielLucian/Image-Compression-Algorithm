
#ifndef TEMA2_QUEUE_H
#define TEMA2_QUEUE_H

#include <stddef.h>


typedef struct QNode {
    QuadtreeNode *data;
    struct QNode *next;
    struct QNode *prev;
} QNode_t;

typedef struct queue {
    QNode_t *head;
    QNode_t *tail;
    size_t len;
} queue_t;

typedef struct QNode_mat {
    Image *data;
    struct QNode_mat *next;
    struct QNode_mat *prev;
} QNode_t_mat;

typedef struct queue_mat {
    QNode_t_mat *head;
    QNode_t_mat *tail;
    size_t len;
} queue_t_mat;

QNode_t *initQueueNode(QuadtreeNode *data);
QNode_t_mat *initQueueNode_mat(Image *data);
queue_t *initQueue();
queue_t_mat *initQueue_mat();
void enqueue(queue_t *queue, QuadtreeNode *data);
void enqueue_mat(queue_t_mat *queue, Image *data);
QuadtreeNode *dequeue(queue_t *queue);
Image *dequeue_mat(queue_t_mat *queue);

#endif //TEMA2_QUEUE_H
