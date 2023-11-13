#include "pengmc.h"

typedef void (*TaskCallback)(int);

#define MAX_TASKS 10

typedef struct Task 
{
    TaskCallback callback;
    int param;
} Task_t;

Task_t taskList[MAX_TASKS];
int taskCount = 0;

void registerTask(TaskCallback callback, int param) 
{
    if (taskCount < MAX_TASKS) 
    {
        taskList[taskCount].callback = callback;
        taskList[taskCount].param = param;
        taskCount++;
    } 
    else
        printf("Task list is full!\n");
}


void runTasks() 
{
    for (int i = 0; i < taskCount; i++) 
        taskList[i].callback(taskList[i].param);
}

void task1(int param) 
{
    printf("Running task 1 with param %d\n", param);
}

void task2(int param) 
{
    printf("Running task 2 with param %d\n", param);
}

int main() 
{
    registerTask(task1, 100);
    registerTask(task2, 200);

    sleep(5);

    runTasks();

    return 0;
}

