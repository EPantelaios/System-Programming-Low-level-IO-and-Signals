#ifndef COUNTRY_LIST_H
#define COUNTRY_LIST_H

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct country_node{

    char *country;
    struct country_node *next;

}country_node;


country_node *insert_country(country_node **head, char *country);

int delete_first_node_country(country_node **head);

int delete_last_node_country(country_node *head);

int delete_node_country(country_node **head, char *country);

int delete_list_country(country_node **head);

int there_is_this_country(country_node *head, char *country);

country_node *search_country(country_node *head, char *country);

void print_list_country(country_node *head);


#endif