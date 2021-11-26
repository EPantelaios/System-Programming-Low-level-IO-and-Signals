#include "../include/info_citizen_list.h"


info_citizen_node *insert_citizen(info_citizen_node **head, char *name, char *surname, country_node *country, int age){

    info_citizen_node *new_node = (info_citizen_node *)calloc(1, sizeof(info_citizen_node));

    new_node->name=(char *)calloc(strlen(name)+1, sizeof(char));
    strncpy(new_node->name, name, strlen(name));
    memset(new_node->name+strlen(name), '\0', 1);

    new_node->surname=(char *)calloc(strlen(surname)+1, sizeof(char));
    strncpy(new_node->surname, surname, strlen(surname));
    memset(new_node->surname+strlen(surname), '\0', 1);

    //new_node->country_list=(country_node *)calloc(1, sizeof(country_node));
   
    new_node->country_list=country;
    
    new_node->age=age;

    new_node->next = *head;
    *head = new_node;

    return new_node;
}



int delete_first_node_citizen(info_citizen_node **head){

    int ret_val = 0;
    info_citizen_node *next_node = NULL;

    //if list is empty
    if(*head == NULL){

        return -1;
    }
    
    next_node = (*head)->next;
    free((*head)->name);
    free((*head)->surname);
    free(*head);
    *head = next_node;

    return ret_val;
}



int delete_list_citizen(info_citizen_node **head){

    int ret_val=0;
    
    while(ret_val != -1){

        ret_val=delete_first_node_citizen(head);
    }

    return 0;
}



int search_citizen(info_citizen_node *head, char *name, char *surname, char *country, int age){

    info_citizen_node *current = head;

    while(current != NULL){

        if(!strcmp(current->name, name) && !strcmp(current->surname, surname) && !strcmp(current->country_list->country, country) && current->age==age){

            return true;
        }

        current = current->next;
    }

    return false;
}



info_citizen_node *find_citizen(info_citizen_node *head, char *name, char *surname, char *country, int age){

    info_citizen_node *current = head;

    while(current != NULL){

        if(!strcmp(current->name, name) && !strcmp(current->surname, surname) && !strcmp(current->country_list->country, country) && current->age==age){

            return current;
        }

        current = current->next;
    }

    return NULL;
}



void print_list_citizen(info_citizen_node *head){

    info_citizen_node *current = head;

    if(current==NULL){

        printf("List is empty\n");
    }
    
    while(current != NULL){

        printf("%s %s %s %d\n", current->name, current->surname, current->country_list->country, current->age);
        
        current = current->next;
    }

    printf("\n");
}