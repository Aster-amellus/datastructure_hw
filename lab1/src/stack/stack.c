#include "stack.h"

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

