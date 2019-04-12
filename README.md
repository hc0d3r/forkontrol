# forkontrol

This is my code for make the use of parallel processes a bit easier

## example

```c
#include <stdio.h>
#include <unistd.h>
#include "forkontrol.h"

int do_sleep(void *arg){
    int seconds = (int)(long)arg;
    printf("[%d] sleeping %d seconds\n", getpid(), seconds);
    sleep(seconds);
    printf("[%d] finish\n", getpid());
    return 0;
}

int main(void){
    struct forkontrol f;
    size_t i;

    /* 10 = max process number */
    if(forkontrol_init(&f, 10)){
        perror("forkontrol_init()");
        return 1;
    }

    for(i=0; i<20; i++){
        forkontrol_run(&f, do_sleep, (void *)i);
    }

    forkontrol_wait_all(&f);
    forkontrol_free(&f);

    return 0;
}
```
