#ifndef READ_WRITE_PIPE_H
#define READ_WRITE_PIPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#include "../include/initialize.h"

int read_from_pipe(int fd, char *buf, unsigned int buffer_size);

int write_to_pipe(int fd, char *buf, int bytes_to_write, unsigned int buffer_size, int special_code);


#endif