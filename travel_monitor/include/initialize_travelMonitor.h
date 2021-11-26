#ifndef INITIALIZE_TRAVEL_MONITOR_H
#define INITIALIZE_TRAVEL_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include "../../monitor/include/skip_list.h"


int initialize_aux_vars(char **buffer, char **tmp_buf, char **aux_buf, char **citizen_id_str, 
                    char **country, char **virus_name, char **yes_or_no, int buffersize, int size);

int initialize_bloom_filter(unsigned char ***bloom_filter_array, int size);

int initialize_viruses_array(char ***viruses_array, int cnt_of_viruses, int size);


int max_index_virus_array(char **viruses_array, int array_size);

int find_index_virus_name(char **viruses_array, int array_size, char *virus_name);

int is_new_virus(char **viruses_array, int array_size, char *virus_name);

int find_max_integer_from_array(int *int_array, int size);

void print_commands();

int clean_str(char *buf, int buffer_size);


#endif