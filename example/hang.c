#include <stdio.h>
#include <unistd.h>

int main(void) {
    char c;
    printf("pid: %d, press any key to exit ... ", getpid());
    scanf("%c", &c);
    return 0;
}