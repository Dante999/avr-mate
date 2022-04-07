#include "fifo.h"

#include <errno.h> // errno
#include <stdio.h>
#include <stdlib.h>    // constants like EXIT_FAILURE
#include <string.h>    // strncpy()
#include <sys/stat.h>  // mkfifo()
#include <sys/types.h> // mkfifo()
#include <unistd.h>

#include "cartridge/haw_logger.h"

void fifo_init(struct fifo_pipe *f, char *path, int size)
{
	strncpy(f->path, path, MAX_FIFO_PATH - 1);

	f->size = size;
}

int fifo_open(struct fifo_pipe *f, int mode)
{
	umask(0);

	if ((mkfifo(f->path, 0666)) == -1) {

		int error = errno;

		if (error == EEXIST) {
			char msg[100];
			snprintf(msg, 100, "pipe %s already exists", f->path);
			LOG_INFO(msg);
		}
		else {
			char msg[100];
			snprintf(msg, 100, "error creating pipe %s: %s",
			         f->path, strerror(error));

			exit(EXIT_FAILURE);
		}
	}

	char msg[100];
	snprintf(msg, 100, "trying to open pipe %s", f->path);
	LOG_DEBUG(msg);

	/* FIFO zum Lesen und Schreiben öffnen */
	if ((f->handle = open(f->path, mode)) == -1) {
		char msg[100];
		snprintf(msg, 100, "error opening pipe %s: %s", f->path,
		         strerror(errno));
		exit(EXIT_FAILURE);
	}
	else {
		char msg[100];
		snprintf(msg, 100, "opening pipe %s successful", f->path);
		LOG_INFO(msg);
	}

	return 0;
}

int fifo_close(struct fifo_pipe *f)
{
	int ret   = close(f->handle);
	f->handle = 0;
	return ret;
}

int fifo_write(const struct fifo_pipe *f, const char *data)
{
	return write(f->handle, data, f->size);
}

int fifo_read(const struct fifo_pipe *f, char *data)
{
	return read(f->handle, data, f->size);
}
