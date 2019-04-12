#ifndef __FORKONTROL_H__
#define __FORKONTROL_H__

#include <unistd.h>

struct forkontrol {
    size_t size;
    size_t count;
    pid_t *pids;
    void (*start_cb)(void *);
    void *start_cb_arg;
};

int forkontrol_init(struct forkontrol *f, size_t len);
void forkontrol_free(struct forkontrol *f);
pid_t forkontrol_run(struct forkontrol *f, int (*callback)(void *), void *arg);
void forkontrol_wait_first(struct forkontrol *f);
void forkontrol_wait_all(struct forkontrol *f);

#endif
