#ifndef FIFO_H
#define FIFO_H

#include <fcntl.h> // constants for fifo mode (e.g. O_RWONLY)

#define MAX_FIFO_PATH 40

struct fifo_pipe {
	char path[40];
	int  size;
	int  handle;
};

void fifo_init(struct fifo_pipe *f, char *path, int size);
int  fifo_open(struct fifo_pipe *f, int mode);
int  fifo_close(struct fifo_pipe *f);
int  fifo_write(const struct fifo_pipe *f, const char *data);
int  fifo_read(const struct fifo_pipe *f, char *data);

#endif // FIFO_H
