#include "data_struc.h"

Queue* create_queue()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if(queue == NULL)
    {
        printf("Failed to allocate memory for queue.\n");
        exit(EXIT_FAILURE);
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

bool is_empty_queue(Queue* queue)
{
    return queue->front == NULL;
}

void push_queue(Queue* queue, int data, int arrive_time)
{
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if(new_node == NULL)
    {
        printf("Failed to allocate memory for queue node.\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->arrive_time = arrive_time; // 设置到达时间
    new_node->next = NULL;
    if (is_empty_queue(queue))
    {
        queue->front = new_node;
        queue->rear = new_node;
    }
    else
    {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
}

QueueNode* pop_queue(Queue* queue)
{
    if (is_empty_queue(queue))
    {
        printf("Queue is empty\n");
        return NULL;
    }
    QueueNode* temp = queue->front;
    queue->front = temp->next;
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
    return temp;
}

void free_queue(Queue* queue)
{
    while (!is_empty_queue(queue))
    {
        pop_queue(queue);
    }
    free(queue);
}

Stack* create_stack(int size)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if(s == NULL)
    {
        printf("Failed to allocate memory for stack.\n");
        exit(EXIT_FAILURE);
    }
    s->data = (int *)malloc(size * sizeof(int));
    s->top = -1;
    s->size = size;
    return s;
}

void push_stack(Stack *s, int data)
{
    if (s->top == s->size - 1)
    {
        printf("Stack is full\n");
        return;
    }
    s->top++;
    s->data[s->top] = data;
}   

int pop_stack(Stack *s)
{
    if (s->top == -1)
    {
        printf("Stack is empty\n");
        return -1;
    }
    int data = s->data[s->top];
    s->top--;
    return data;
}

bool is_empty_stack(Stack *s)
{
    return s->top == -1;
}

bool is_full_stack(Stack *s)
{
    return s->top == s->size - 1;
}

void free_stack(Stack *s)
{
    free(s->data);
    free(s);
}

