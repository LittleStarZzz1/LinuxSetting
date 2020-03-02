#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char buf[10] = { 0 };
    int i;
    for (i = 0; i < 10; ++i)
    {
        buf[i] = '-';
        printf("\r%s", buf);
        fflush(stdout);
        usleep(10000);
    }


    return 0;
}
