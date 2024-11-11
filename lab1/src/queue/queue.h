#include "../common.h" 

// queue
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
