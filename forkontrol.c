#include "forkontrol.h"
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

int forkontrol_init(struct forkontrol *f, size_t maxprocs){
    int ret;
    memset(f, 0, sizeof(struct forkontrol));

    f->size = maxprocs;
    f->pids = calloc(maxprocs, sizeof(pid_t));

    if(f->pids == NULL)
        ret = 1;
    else
        ret = 0;


    return ret;
}

void forkontrol_free(struct forkontrol *f){
    free(f->pids);
}

pid_t forkontrol_run(struct forkontrol *f, int (*callback)(void *), void *arg){
    pid_t ret = 0;
    size_t i;

    if(f->count >= f->size)
        forkontrol_wait_first(f);

    for(i=0; i<f->size; i++){
        if(!f->pids[i]){
            ret = fork();
            if(ret == 0){
                if(f->start_cb){
                    f->start_cb(f->start_cb_arg);
                }

                _exit(callback(arg));
            }

            f->pids[i] = ret;
            f->count++;

            break;
        }
    }

    return ret;
}

void forkontrol_wait_all(struct forkontrol *f){
    size_t i;
    int status;

    while(f->count){
        pid_t aux = waitpid(-1, &status, 0);
        if(aux == -1){
            err(1, "waitpid error");
        }

        for(i=0; i<f->size; i++){
            if(aux != f->pids[i])
                continue;

            if(!WIFEXITED(status) && !WIFSIGNALED(status))
                continue;

            f->pids[i] = 0x0;
            f->count--;

            break;
        }
    }
}

void forkontrol_wait_first(struct forkontrol *f){
    int status;
    pid_t aux;
    size_t i;

    while(f->count == f->size){
        aux = waitpid(-1, &status, 0);
        if(aux == -1){
            err(1, "waitpid error");
        }

        if(!WIFEXITED(status) && !WIFSIGNALED(status))
            continue;

        for(i=0; i<f->size; i++){
            if(aux == f->pids[i]){
                f->pids[i] = 0;
                f->count--;
                break;
            }
        }
    }
}
