#include "../common.h"

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
