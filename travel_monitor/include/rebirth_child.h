#ifndef REBIRTH_CHILD_H
#define REBIRTH_CHILD_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "bloom_filter.h"
#include "read_write_pipe.h"
#include "country_list.h"
#include "exchange_messages_with_monitor.h"


int initialize_child_after_terminate(child_info *child_array, int index, int buffer_size, int bloom_size, char *input_dir);


#endif