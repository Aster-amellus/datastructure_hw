#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct QueueNode {
    int data;
    int arrive_time; // 新增：记录到达时间
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

Queue *create_queue();
void push_queue(Queue *q, int data, int arrive_time);
QueueNode* pop_queue(Queue *q);
bool is_empty_queue(Queue *q);
void free_queue(Queue *q);

// Stack

typedef struct {
    int *data;
    int top; //top means used size
    int size;
} Stack;

Stack *create_stack(int size);
void push_stack(Stack *s, int data);
int pop_stack(Stack *s);
bool is_empty_stack(Stack *s);
bool is_full_stack(Stack *s);
void free_stack(Stack *s);