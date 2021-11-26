#include "../include/country_list.h"


country_node *insert_country(country_node **head, char *country){

    country_node *new_node = (country_node *)calloc(1, sizeof(country_node));

    new_node->country=(char *)calloc(strlen(country)+1, sizeof(char));
    strncpy(new_node->country, country, strlen(country));
    memset(new_node->country+strlen(country), '\0', 1);

    new_node->next = *head;
    *head = new_node;

    return new_node;
}


int delete_first_node_country(country_node **head){

    int ret_val = 0;
    country_node *next_node = NULL;

    if(*head == NULL){

        return -1;
    }
    
    next_node = (*head)->next;
    free((*head)->country);
    free(*head);
    *head = next_node;

    return ret_val;
}


int delete_list_country(country_node **head){

    int ret_val=0;
    
    while(ret_val != -1){

        ret_val=delete_first_node_country(head);
    }

    return 0;
}


int there_is_this_country(country_node *head, char *country){

    country_node *current = head;

    while(current != NULL){
        
        if(!strcmp(current->country, country)){

            return true;
        }

        current = current->next;
    }
    return false;
}


country_node *search_country(country_node *head, char *country){

    country_node *current = head;

    while(current != NULL){
        
        if(!strcmp(current->country, country)){

            return current;
        }

        current = current->next;
    }
    return NULL;
}


void print_list_country(country_node *head){

    country_node *current = head;

    if(current==NULL){

        printf("List is empty\n");
    }
    
    while(current != NULL){
        printf("%s\n", current->country);
        current = current->next;
    }

    printf("\n");
}