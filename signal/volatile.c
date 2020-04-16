#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile long long a = 1;

void sigcb(int no)
{
    a = 0;
    printf("a = %d\n", a);
}

int main()
{
    signal(SIGINT, sigcb);

    while (a)
    {
        sleep(1);
    }
    printf("exited a = %d\n", a);

    return 0;
}
