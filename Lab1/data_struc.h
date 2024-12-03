#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int *data;
    int front;
    int rear;
    int size;
}tQueue;

tQueue *create_queue(int size);
void push_queue(tQueue *q, int data);
int pop_queue(tQueue *q);
bool is_empty_queue(tQueue *q);
bool is_full_queue(tQueue *q);
void free_queue(tQueue *q);

// tStack

typedef struct {
    int *data;
    int top; //top means used size
    int size;
} tStack;

tStack *create_stack(int size);
void push_stack(tStack *s, int data);
int pop_stack(tStack *s);
bool is_empty_stack(tStack *s);
bool is_full_stack(tStack *s);
void free_stack(tStack *s);