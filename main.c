#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int create_sleeping_child();
int random_range(int min, int max);
unsigned int xor_true_rand();

int main(void)
{
    int child_a, child_a_status;
    int child_b, child_b_status;

    child_a = create_sleeping_child();
    child_b = create_sleeping_child();

    waitpid(child_a, &child_a_status, 0);

    printf("[PARENT] Child Process A [%d] has finished! They slept for %d seconds.\n", child_a, WEXITSTATUS(child_a_status));

    waitpid(child_b, &child_b_status, 0);
    printf("[PARENT] Child Process B [%d] has finished! They slept for %d seconds.\n", child_b, WEXITSTATUS(child_b_status));
}

int create_sleeping_child()
{
    int parent_pid;
    int child_pid;
    int sleep_time;

    // Sleep time is calculated before fork to
    // Prevent both forks from having identical values
    sleep_time = random_range(2, 5);

    parent_pid = getpid();
    child_pid = fork();

    // Return Parent Process 
    if (child_pid) return child_pid;

    // Continue as child
    else
    {
        child_pid = getpid();

        printf("[PROCESS %d]: I was Created! I'm going to sleep...\n", child_pid);

        sleep(sleep_time);
        
        printf("[PROCESS %d]: Finished Sleeping! Returning to %d\n", child_pid, parent_pid);
        
        exit(sleep_time);
    }
}

int random_range(int min, int max)
{
    return min + xor_true_rand() % (max - min);
}

unsigned int xor_true_rand()
{
    unsigned int pool = 0;
    int i;

    for(i = 0; i < 16; ++i)
    {
        pool ^= (unsigned int)(clock() + time(0) + rand());
        pool ^= pool << 13; pool ^= pool >> 5; pool ^= pool << 7;
    }

    return pool;
}