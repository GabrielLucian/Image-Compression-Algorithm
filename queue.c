#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "queue.h"

QNode_t *initQueueNode(QuadtreeNode *data)
{
    QNode_t *newNode = malloc(sizeof(QNode_t));
    newNode->data = data;
    newNode->prev = newNode->next = NULL;
    return newNode;
}
QNode_t_mat *initQueueNode_mat(Image *data)
{
    QNode_t_mat *newNode = malloc(sizeof(QNode_t_mat));
    newNode->data = data;
    newNode->prev = newNode->next = NULL;
    return newNode;
}
queue_t *initQueue()
{
    queue_t *newQueue = malloc(sizeof(queue_t));
    newQueue->head = newQueue->tail = NULL;
    newQueue->len = 0;
    return newQueue;
}
queue_t_mat *initQueue_mat()
{
    queue_t_mat *newQueue = malloc(sizeof(queue_t_mat));
    newQueue->head = newQueue->tail = NULL;
    newQueue->len = 0;
    return newQueue;
}

void enqueue(queue_t *queue, QuadtreeNode *data)
{
    if (data == NULL)
    {
        return;
    }
    QNode_t *new = initQueueNode(data);
    if (queue->len == 0)
    {
        queue->head = new;
        queue->tail = new;
    }
    else
    {
        new->next = queue->head;
        queue->head->prev = new;
        queue->head = new;
    }
    ++queue->len;
}
void enqueue_mat(queue_t_mat *queue, Image *data)
{
    if (data == NULL)
    {
        return;
    }
    QNode_t_mat *new = initQueueNode_mat(data);
    if (queue->len == 0)
    {
        queue->head = new;
        queue->tail = new;
    }
    else
    {
        new->next = queue->head;
        queue->head->prev = new;
        queue->head = new;
    }
    ++queue->len;
}
QuadtreeNode *dequeue(queue_t *queue) {
    QuadtreeNode *ret;
    if (queue->len == 0)
    {
        return NULL;
    }
    QNode_t *aux = queue->tail;
    if (queue->len == 1)
    {
        queue->tail = NULL;
        queue->head = NULL;
    }
    else
    {
        queue->tail = aux->prev;
        queue->tail->next = NULL;
    }
    --queue->len;
    ret = aux->data;
    free(aux);
    return ret;
}
Image *dequeue_mat(queue_t_mat *queue)
{
    Image *ret;
    if (queue->len == 0)
    {
        return NULL;
    }
    QNode_t_mat *aux = queue->tail;
    if (queue->len == 1)
    {
        queue->tail = NULL;
        queue->head = NULL;
    }
    else
    {
        queue->tail = aux->prev;
        queue->tail->next = NULL;
    }
    --queue->len;
    ret = aux->data;
    free(aux);
    return ret;
}

