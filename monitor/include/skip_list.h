#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <assert.h>
#include <time.h>
#include "info_citizen_list.h"
#include "country_list.h"
#include "date_list.h"



typedef struct skip_list_node{

    int citizenID;
    struct info_citizen_node *info_citizen;
    struct vaccination_date_node *date;
    struct skip_list_node *lower_layer;
    struct skip_list_node *next;

}skip_list_node;



typedef struct skip_list{

    skip_list_node **head;
    int max_layer;
    int current_max_layer;
    char *virus_name;
    float possibility_flip_coin; //possibility a node will be promoted to the next higher level

}skip_list;



int assignment_values(skip_list_node *node, int citizenID, info_citizen_node *info_citizen, vaccination_date_node *date);

int flip_coin(float possibility);

skip_list *create_skip_list(skip_list *skip_list_array, int max_layer, float possibility, char *virus_name);

skip_list_node *insert_first_node(skip_list_node **head, int citizenID, info_citizen_node *info_citizen, vaccination_date_node *date);

skip_list_node *insert_skip_list_node(skip_list_node **head, int citizenID, info_citizen_node *info_citizen, vaccination_date_node *date);

skip_list_node *insert_skip_list_node_upper_layer(skip_list_node **head, int citizenID, skip_list_node *pointer_node);

int insert_skip_list(skip_list **skip_lists, int index, int citizenID, info_citizen_node *info_citizen, vaccination_date_node *date);

int delete_first_node(skip_list_node **head);

int delete_last_node(skip_list_node *head);

int delete_list_node(skip_list_node **head, int citizenID);

int delete_list(skip_list_node **head);

int delete_node_skip_list(skip_list **skip_lists, int index, int citizenID);

int delete_skip_list_structure(skip_list **skip_lists, int index);

int max_index_skip_lists(char **viruses_array, int array_size);

int max_index_virus_array(char **viruses_array, int array_size);

int find_index_virus_name(char **viruses_array, int array_size, char *virus_name);

int is_new_virus(char **viruses_array, int array_size, char *virus_name);

int initialize_skip_lists_for_new_virus(skip_list **skip_lists, int max_layer, char **viruses_array, 
                                        int array_size, char *virus_name, float possibility);

int find_index_skip_list(skip_list **skip_lists, int cnt_of_viruses, char *virus_name);

skip_list_node *find_entry_skip_list(skip_list **skip_lists, int index, int citizenID);

int there_is_that_entry_in_skip_list(skip_list **skip_lists, int index, int citizenID);

skip_list_node *search_node_to_insert_skip_list(skip_list **skip_lists, int index, int citizenID);

void print_layer_zero_all_info(skip_list **skip_lists, int index);

void print_layer_zero_non_vaccinated(skip_list **skip_lists, int index);

void print_sub_skip_list(skip_list_node *head);

void print_skip_list(skip_list **skip_lists, int index);



#endif