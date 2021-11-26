#ifndef EXCHANGE_MESSAGES_WITH_MONITOR_H
#define EXCHANGE_MESSAGES_WITH_MONITOR_H


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


typedef struct child_info{

    pid_t child_pid;
    int fd_parent_read;
    int fd_parent_write;
    char *fifo_parent_read;
    char *fifo_parent_write;

    country_node *head_country;
    unsigned char **bloom_filter_array;
    int cnt_of_viruses;
    char **viruses_array;

}child_info;


int communicate_with_monitor_process(child_info *child_array, int num_monitors, int buffer_size, int bloom_size, char *input_dir);


void print_monitors_info(child_info *child_array, int num_monitors);

#endif