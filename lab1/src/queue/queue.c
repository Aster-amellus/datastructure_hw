#include "queue.h"

tQueue* create_queue(int size)
{
    tQueue* queue = (tQueue*)malloc(sizeof(tQueue));
    queue->data = (int*)malloc(sizeof(int) * size);
    queue->front = 0;
    queue->rear = 0;
    queue->size = size;
    return queue;
}

bool is_full_queue(tQueue* queue)
{
    return (queue->rear + 1) % queue->size == queue->front;
}

bool is_empty_queue(tQueue* queue)
{
    return queue->front == queue->rear;
}

void push_queue(tQueue* queue, int data)
{
    if (is_full_queue(queue))
    {
        printf("Queue is full\n");
        return;
    }
    queue->data[queue->rear] = data;
    queue->rear = (queue->rear + 1) % queue->size;
}

int pop_queue(tQueue* queue)
{
    if (is_empty_queue(queue))
    {
        printf("Queue is empty\n");
        return -1;
    }
    int data = queue->data[queue->front];
    queue->front = (queue->front + 1) % queue->size;
    return data;
}

void free_queue(tQueue* queue)
{
    free(queue->data);
    free(queue);
}