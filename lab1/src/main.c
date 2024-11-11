#include "common.h"
#include "stack/stack.h"
#include "queue/queue.h"

#define CARS_NUM 1005

int n;
tStack *ins;
tStack *outs;
tQueue *waits;

int arrive_time[1005];
int leave_time[1005];

void init()
{
    ins = create_stack(n);
    outs = create_stack(n);
    waits = create_queue(CARS_NUM);
    memset(arrive_time, -1, sizeof(arrive_time));
    memset(leave_time, -1, sizeof(leave_time));
}

void car_arrive(int id, int car_time)
{
    if(is_full_stack(ins))
    {
        push_queue(waits, id);
    }
    else
    {
        push_stack(ins, id);
        arrive_time[id] = car_time;
    }
    printf("id: %d arrive: %d\n", id, car_time);
}

void car_departure(int id, int car_time)
{
    int now_id = -1;
    while(now_id != id)
    {
        now_id = pop_stack(ins);
        //printf("now_id: %d\n", now_id);
        if(now_id != id)
        {
            push_stack(outs, now_id);
        }
    }
    leave_time[id] = car_time;
    while(!is_empty_stack(outs))
    {
        now_id = pop_stack(outs);
        push_stack(ins, now_id);
    }
    while(!is_empty_queue(waits) && !is_full_stack(ins))
    {
        now_id = pop_queue(waits);
        push_stack(ins, now_id);
        arrive_time[now_id] = car_time;
    }
    printf("id: %d departure: %d\n", id, car_time);
}

void calc()
{
    for(int i = 0; i < CARS_NUM; i++)
    {
        if(arrive_time[i] != -1)
        {
            if(leave_time[i] != -1)
            {
                printf("id: %d arrive: %d departure: %d stay: %d\n", i, arrive_time[i], leave_time[i], leave_time[i] - arrive_time[i]);
            }
            else 
            {
                printf("id: %d arrive: %d departure: NONE stay: still staying\n", i, arrive_time[i]);
            }
        }
    }
}

int main()
{
    scanf("%d\n", &n);
    init();
    while(1)
    {
        char type;
        int id, car_time;
        scanf("%c %d %d", &type, &id, &car_time);
        //printf("type: %c id: %d time: %d\n", type, id, car_time);
        if(type == 'E')
        {
            printf("OVER\n");
            break;
        }
        else if(type == 'A')
        {
            car_arrive(id, car_time);
        }
        else if(type == 'D')
        {
            car_departure(id, car_time);
        }
    }
    calc();
}