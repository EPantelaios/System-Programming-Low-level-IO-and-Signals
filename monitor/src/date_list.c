#include "../include/date_list.h"


vaccination_date_node *insert_date(vaccination_date_node **head, int day, int month, int year){

    vaccination_date_node *new_node = (vaccination_date_node *)calloc(1, sizeof(vaccination_date_node));

    new_node->day=day;
    new_node->month=month;
    new_node->year=year;
    //new_node->cnt_of_vaccinations=cnt_of_vaccinations;

    new_node->next = *head;
    *head = new_node;

    return new_node;
}


int delete_first_node_date(vaccination_date_node **head){

    int ret_val = 0;
    vaccination_date_node *next_node = NULL;

    if(*head == NULL){
        
        return -1;
    }
    
    next_node = (*head)->next;
    free(*head);
    *head = next_node;

    return ret_val;
}



int delete_list_date(vaccination_date_node **head){

    int ret_val=0;
    
    while(ret_val != -1){

        ret_val=delete_first_node_date(head);
    }

    return 0;
}



vaccination_date_node *search_date(vaccination_date_node *head, int day, int month, int year){

    vaccination_date_node *current = head;

    while(current != NULL){

        if(current->day==day && current->month==month && current->year==year){

            return current;
        }

        current = current->next;
    }

    return NULL;
}



void print_list_date(vaccination_date_node *head){

    vaccination_date_node *current = head;

    if(current==NULL){

        printf("List is empty\n");

    }
    
    while(current != NULL){

        printf("%d-%d-%d\n", current->day, current->month, current->year);
        current = current->next;
    }

    printf("\n");
}