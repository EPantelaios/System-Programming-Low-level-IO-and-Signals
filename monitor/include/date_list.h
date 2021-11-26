#ifndef DATE_LIST_H
#define DATE_LIST_H

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct vaccination_date_node{

    int day;
    int month;
    int year;
    //int cnt_of_vaccinations;
    struct vaccination_date_node *next;

}vaccination_date_node;


vaccination_date_node *insert_date(vaccination_date_node **head, int day, int month, int year);

int delete_first_node_date(vaccination_date_node **head);

int delete_list_date(vaccination_date_node **head);

vaccination_date_node *search_date(vaccination_date_node *head, int day, int month, int year);

void print_list_date(vaccination_date_node *head);



#endif