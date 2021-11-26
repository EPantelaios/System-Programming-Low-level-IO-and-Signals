#ifndef INFO_CITIZEN_LIST_H
#define INFO_CITIZEN_LIST_H

#include "country_list.h"
#include "date_list.h"


typedef struct info_citizen_node{

    char *name;
    char *surname;
    int age;
    struct country_node *country_list;
    struct info_citizen_node *next;

}info_citizen_node;


info_citizen_node *insert_citizen(info_citizen_node **head, char *name, char *surname, country_node *country, int age);

int delete_first_node_citizen(info_citizen_node **head);

int delete_list_citizen(info_citizen_node **head);

int search_citizen(info_citizen_node *head, char *name, char *surname, char *country, int age);

info_citizen_node *find_citizen(info_citizen_node *head, char *name, char *surname, char *country, int age);

void print_list_citizen(info_citizen_node *head);


#endif