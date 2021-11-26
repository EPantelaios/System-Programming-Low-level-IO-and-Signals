#ifndef INSERT_ENTRY_H
#define INSERT_ENTRY_H

#include <unistd.h>
#include "check_input.h"
#include "skip_list.h"
#include "bloom_filter.h" 
#include "info_citizen_list.h"
#include "country_list.h"
#include "date_list.h"


int insert_entry(skip_list **skip_lists, char **viruses_array, int cnt_of_viruses, unsigned char **bloom_filter_array,
                 int bloom_size, country_node **head_country, info_citizen_node **head_citizen, 
                 vaccination_date_node **head_date, int max_layer, char *buffer, char *tmp_buf, int number_of_args);


int insert_citizen_record(skip_list **skip_lists, char **viruses_array, int cnt_of_viruses, unsigned char **bloom_filter_array,
                 int bloom_size, country_node **head_country, info_citizen_node **head_citizen, 
                 vaccination_date_node **head_date, int max_layer, char *buffer, char *tmp_buf, int number_of_args);


int vaccinate_now(skip_list **skip_lists, char **viruses_array, int cnt_of_viruses, unsigned char **bloom_filter_array,
                 int bloom_size, country_node **head_country, info_citizen_node **head_citizen, 
                 vaccination_date_node **head_date, int max_layer, char *buffer, char *tmp_buf, int number_of_args);


int free_heap_insert_entry(info_citizen_node **citizen_node, vaccination_date_node **date_node, char **citizen_id_str, 
                           char **virus_name, char **yes_or_no);


#endif