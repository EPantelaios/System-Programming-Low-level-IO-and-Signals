#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <stdio.h>
#include <stdlib.h>
#include "skip_list.h"


typedef struct travel_requests{

    int accepted;
    int rejected;
}travel_requests;


typedef struct travel_requests_monitor{

    int accepted;
    int rejected;
}travel_requests_monitor;


int initialize_aux_vars(char **buffer, char **tmp_buf, char **aux_buf, char **citizen_id_str, 
                    char **country, char **virus_name, char **yes_or_no, int buffersize, int size);

int initialize_bloom_filter(unsigned char ***bloom_filter_array, int size);

int initialize_skip_list(skip_list ***skip_lists, int size);

int initialize_viruses_array(char ***viruses_array, int cnt_of_viruses, int size);

int clean_str(char *buf, int buffer_size);


#endif