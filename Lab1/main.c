#include "data_struc.h"

#define CARS_NUM 1005
#define PARKING_FEE_PER_TIME 1.0
#define WAITING_FEE_PER_TIME (PARKING_FEE_PER_TIME / 3.0)

int n;
Stack *ins;
Stack *outs;
Queue *waits;

int arrive_time[1005];
int leave_time[1005];

void init()
{
    ins = create_stack(n);
    outs = create_stack(n);
    waits = create_queue();
    memset(arrive_time, -1, sizeof(arrive_time));
    memset(leave_time, -1, sizeof(leave_time));
}

void free_resources()
{
    free_stack(ins);
    free_stack(outs);
    free_queue(waits);
}

void car_arrive(int id, int car_time)
{
    if(id < 0 || id >= CARS_NUM)
    {
        printf("Invalid car id: %d\n", id);
        return;
    }
    if(is_full_stack(ins))
    {
        push_queue(waits, id, car_time);
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
    if(is_empty_stack(ins))
    {
        printf("No cars in the parking lot.\n");
        return;
    }
    int found = 0;
    int now_id = -1;
    while(!is_empty_stack(ins))
    {
        now_id = pop_stack(ins);
        if(now_id != id)
        {
            push_stack(outs, now_id);
        }
        else
        {
            leave_time[id] = car_time;
            found = 1;
            break;
        }
    }
    if(!found)
    {
        printf("Car %d not found in the parking lot.\n", id);
        while(!is_empty_stack(outs))
        {
            now_id = pop_stack(outs);
            push_stack(ins, now_id);
        }
        return;
    }
    while(!is_empty_stack(outs))
    {
        now_id = pop_stack(outs);
        push_stack(ins, now_id);
    }
    while(!is_empty_queue(waits) && !is_full_stack(ins))
    {
        QueueNode* node = pop_queue(waits);
        int waiting_id = node->data;
        int waiting_arrive_time = node->arrive_time;

        // 计算等待车辆的等待时间和费用
        int waiting_time = car_time - waiting_arrive_time;
        float waiting_fee = waiting_time * WAITING_FEE_PER_TIME;
        printf("id: %d waited for %d units, waiting fee: %.2f\n", waiting_id, waiting_time, waiting_fee);

        push_stack(ins, waiting_id);
        arrive_time[waiting_id] = car_time;
        free(node);
    }
    printf("id: %d departure: %d\n", id, car_time);
}


void calc_waiting_fee(int current_time)
{
    float total_waiting_fee = 0.0;
    QueueNode *current = waits->front;
    while(current != NULL)
    {
        int id = current->data;
        int waiting_time = current_time - current->arrive_time;
        float fee = waiting_time * WAITING_FEE_PER_TIME;
        total_waiting_fee += fee;
        printf("id: %d waiting time: %d fee: %.2f\n", id, waiting_time, fee);
        current = current->next;
    }
    printf("Total waiting fee: %.2f\n", total_waiting_fee);
}

void calc()
{
    float total_parking_fee = 0.0;
    int current_time = 0;
    for(int i = 0; i < CARS_NUM; i++)
    {
        if(arrive_time[i] != -1)
        {
            if(leave_time[i] != -1)
            {
                int stay_time = leave_time[i] - arrive_time[i];
                float fee = stay_time * PARKING_FEE_PER_TIME;
                total_parking_fee += fee;
                printf("id: %d arrive: %d departure: %d stay: %d fee: %.2f\n", i, arrive_time[i], leave_time[i], stay_time, fee);
                if(leave_time[i] > current_time)
                {
                    current_time = leave_time[i];
                }
            }
            else 
            {
                printf("id: %d arrive: %d departure: NONE stay: still staying\n", i, arrive_time[i]);
            }
        }
    }
    printf("Total parking fee: %.2f\n", total_parking_fee);
    calc_waiting_fee(current_time);
}

// 修改 main 函数，避免使用 freopen，改用文件指针读取输入文件
int main()
{
    printf("请输入停车场容量 n：\n");
    scanf("%d", &n);
    init();

    printf("请输入输入文件的文件名，例如：input.txt\n");
    char filename[100];
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("无法打开文件 %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        char type;
        int id, car_time;
        if(fscanf(fp, " %c %d %d", &type, &id, &car_time) != 3)
        {
            printf("输入格式错误，程序退出。\n");
            break;
        }
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
        else
        {
            printf("无效的操作类型：%c\n", type);
        }
    }
    fclose(fp);
    calc();
    free_resources();
    return 0;
}