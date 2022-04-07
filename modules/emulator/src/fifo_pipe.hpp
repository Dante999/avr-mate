#ifndef FIFO_PIPE_H
#define FIFO_PIPE_H

#include <string>

#include <fcntl.h> // fifo
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h> // fifo write() read()

#include <spdlog/spdlog.h>

class fifo_pipe_c {

      public:
	enum class mode_e { READ, WRITE };

      private:
	const std::string m_filepath;
	int               m_fhandle;

      public:
	fifo_pipe_c(const std::string &path) : m_filepath(path){};

	int open_pipe(mode_e mode)
	{
		umask(0);

		if ((mkfifo(m_filepath.c_str(), 0666)) == -1) {

			int error = errno;

			if (error == EEXIST) {
				spdlog::info("pipe {} does already exist",
				             m_filepath);
			}
			else {
				spdlog::error(
				    "failed to create fifo pipe {} : {}",
				    m_filepath, strerror(error));
				exit(EXIT_FAILURE);
			}
		}

		const int flags = (mode == mode_e::READ ? O_RDONLY : O_WRONLY);

		/* FIFO zum Lesen und Schreiben öffnen */
		if ((m_fhandle = open(m_filepath.c_str(), flags)) == -1) {
			spdlog::error("failed to open fifo pipe {} : {}",
			              m_filepath, strerror(errno));
			exit(EXIT_FAILURE);
		}

		return 0;
	}

	int close_pipe()
	{
		return close(m_fhandle);
	}

	void write_bytes(const uint8_t *data, size_t length)
	{
		if (write(m_fhandle, data, length) < 0) {
			spdlog::error("failed to write to pipe {} : {}",
			              m_filepath, strerror(errno));
		}
	}

	void read_bytes(uint8_t *data, size_t length)
	{
		if (read(m_fhandle, data, length) < 0) {
			spdlog::error("failed to read from pipe {} : {}",
			              m_filepath, strerror(errno));
		}
	}
};

#endif /* FIFO_PIPE_H */
