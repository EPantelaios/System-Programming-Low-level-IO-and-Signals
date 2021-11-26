#ifndef SIGNAL_HANDLER_MONITOR_H
#define SIGNAL_HANDLER_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "skip_list.h"
#include "country_list.h"
#include "bloom_filter.h"
#include "initialize.h"
#include "check_input.h"
#include "insert_entry.h"
#include "read_write_pipe.h"


extern int flag_sigusr1, flag_sigint, flag_sigquit;


int set_up_sigusr1();

int set_up_sigint();

int set_up_sigquit();


void sigusr1_handler(int signal);

void sigint_handler(int signal);

void sigquit_handler(int signal);


int update_structures_sigusr1_signal(skip_list **skip_lists, char **viruses_array, int cnt_of_viruses, unsigned char **bloom_filter_array, 
                          int bloom_size, country_node **head_country, info_citizen_node **head_citizen, 
                          vaccination_date_node **head_date, int max_layer, int stored_cnt_files, int fd_write, int buffer_size);


#endif