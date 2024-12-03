#include "data_struc.h"

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


tStack* create_stack(int size)
{
    tStack *s = (tStack *)malloc(sizeof(tStack));
    s->data = (int *)malloc(size * sizeof(int));
    s->top = -1;
    s->size = size;
    return s;
}

void push_stack(tStack *s, int data)
{
    if (s->top == s->size - 1)
    {
        printf("tStack is full\n");
        return;
    }
    s->top++;
    s->data[s->top] = data;
}   

int pop_stack(tStack *s)
{
    if (s->top == -1)
    {
        printf("tStack is empty\n");
        return -1;
    }
    int data = s->data[s->top];
    s->top--;
    return data;
}

bool is_empty_stack(tStack *s)
{
    return s->top == -1;
}

bool is_full_stack(tStack *s)
{
    return s->top == s->size - 1;
}

void free_stack(tStack *s)
{
    free(s->data);
    free(s);
}

