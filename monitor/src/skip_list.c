#include "../include/skip_list.h"


int assignment_values(skip_list_node *node, int citizenID, info_citizen_node *citizen_node, vaccination_date_node *date){
    
    node->citizenID=citizenID;
    node->info_citizen=citizen_node;
    node->date=date;
    node->lower_layer=NULL;

    return 0;
}


int flip_coin(float possibility){

    possibility=possibility*100;
    float random_number=(float)rand()/(float)(RAND_MAX/100);

    if(random_number<=possibility){

        return true;
    }
    else{

        return false;
    }
}


skip_list *create_skip_list(skip_list *skip_list_array, int max_layer, float possibility, char *virus_name){

    skip_list_array = (skip_list *)calloc(1, sizeof(skip_list));
    skip_list_array->max_layer=max_layer;

    skip_list_array->head = (skip_list_node **)calloc(skip_list_array->max_layer, sizeof(skip_list_node *));
    for(int i=0;i<skip_list_array->max_layer;i++){
        
        skip_list_array->head[i]=NULL;
    }

    skip_list_array->current_max_layer=0;
    skip_list_array->possibility_flip_coin=possibility;

    skip_list_array->virus_name=(char *)calloc(strlen(virus_name)+1, sizeof(char));
    strncpy(skip_list_array->virus_name, virus_name, strlen(virus_name));
    memset(skip_list_array->virus_name+strlen(virus_name), '\0', 1);

    return skip_list_array;
}



skip_list_node *insert_first_node(skip_list_node **head, int citizenID, info_citizen_node *citizen_node, vaccination_date_node *date){

    skip_list_node *new_node = (skip_list_node *)calloc(1, sizeof(skip_list_node));
    assignment_values(new_node, citizenID, citizen_node, date);
    
    new_node->next = *head;
    *head = new_node;

    return new_node;
}



skip_list_node *insert_skip_list_node(skip_list_node **head, int citizenID, info_citizen_node *citizen_node, vaccination_date_node *date){

    skip_list_node *current = *head;

    //if the list is empty
    if(current == NULL || citizenID<current->citizenID){

        current=insert_first_node(head, citizenID, citizen_node, date);
        return current;
    }

    while(current->next != NULL){

        if(citizenID > current->citizenID && current->next!=NULL && citizenID < current->next->citizenID){

            skip_list_node *tmp = current->next;
            current->next = (skip_list_node *)calloc(1, sizeof(skip_list_node));
            assignment_values(current->next, citizenID, citizen_node, date);
            current->next->next = tmp;
            return current->next;
        }

        current = current->next;
    }

    //now we can add a new node
    current->next = (skip_list_node *)calloc(1, sizeof(skip_list_node));
    assignment_values(current->next, citizenID, citizen_node, date);
    current->next->next = NULL;

    return current->next;
}



skip_list_node *insert_skip_list_node_upper_layer(skip_list_node **head, int citizenID, skip_list_node *pointer_node){

    skip_list_node *current = *head;

    //if the list is empty
    if(current == NULL || citizenID<current->citizenID){

        skip_list_node *new_node = (skip_list_node *)calloc(1, sizeof(skip_list_node));
        new_node->citizenID=citizenID;
        new_node->lower_layer=pointer_node;
        new_node->next = *head;
        *head = new_node;

        return new_node;
    }

    while(current->next != NULL){

        if(citizenID > current->citizenID && current->next!=NULL && citizenID < current->next->citizenID){

            skip_list_node *tmp = current->next;
            current->next = (skip_list_node *)calloc(1, sizeof(skip_list_node));
            current->next->citizenID=citizenID;
            current->next->lower_layer=pointer_node;
            current->next->next = tmp;
            return current->next;
        }

        current = current->next;
    }

    //now we can add a new variable
    current->next = (skip_list_node *)calloc(1, sizeof(skip_list_node));
    current->next->citizenID=citizenID;
    current->next->lower_layer=pointer_node;
    current->next->next = NULL;

    return current->next;
}



int insert_skip_list(skip_list **skip_lists, int index, int citizenID, info_citizen_node *citizen_node, vaccination_date_node *date){

    skip_list_node *store_node=NULL, *tmp=NULL;
    int current_layer=0;


    //Only for layer zero
    if(skip_lists[index]->head[current_layer]==NULL){
        
        store_node=insert_skip_list_node(&skip_lists[index]->head[current_layer], citizenID, citizen_node, date);
        return 0;
    }
    else{

        store_node=search_node_to_insert_skip_list(skip_lists, index, citizenID);
        
        if(citizenID < store_node->citizenID){

            store_node=insert_first_node(&skip_lists[index]->head[current_layer], citizenID, citizen_node, date);
        }
        else{

            tmp=store_node->next;
            store_node->next=(skip_list_node *)calloc(1, sizeof(skip_list_node));
            assignment_values(store_node->next, citizenID, citizen_node, date);
            store_node->next->next=tmp;
        }
    }

    current_layer++;

    //check if the new node will be inserted at higher layers
    while(current_layer < skip_lists[index]->max_layer && flip_coin(skip_lists[index]->possibility_flip_coin)==true){

        store_node=insert_skip_list_node_upper_layer(&skip_lists[index]->head[current_layer], citizenID, store_node);

        if(current_layer>skip_lists[index]->current_max_layer){

            skip_lists[index]->current_max_layer=current_layer;
        }

        current_layer++;
    }

    return 0;
}




int delete_first_node(skip_list_node **head){

    int ret_val = -1;
    skip_list_node *next_node = NULL;

    //if list is empty
    if(*head == NULL){

        return ret_val;
    }
    
    next_node = (*head)->next;
    ret_val = (*head)->citizenID;
    free(*head);
    *head = next_node;

    return ret_val;
}



int delete_last_node(skip_list_node *head){

    int ret_val = -1;

    //if list is empty
    if(head == NULL){
        
        return ret_val;
    }

    //if there is only one item in the list, delete it
    if(head->next == NULL){

        ret_val = head->citizenID;
        free(head);
        head=NULL;
        return ret_val;
    }

    //get to the second from last node in the list
    skip_list_node *current = head;
    while(current->next->next != NULL){

        current = current->next;
    }

    ret_val = current->next->citizenID;
    free(current->next);
    current->next = NULL;
    return ret_val;
}




int delete_list_node(skip_list_node **head, int citizenID){

    int ret_val = -1;
    skip_list_node *prev_node = NULL;
    skip_list_node *current = *head;

    //if list is empty
    if(*head == NULL){
        
        return ret_val;
    }

    //if there is only one item in the list, delete it
    if((*head)->next == NULL && (*head)->citizenID==citizenID){

        ret_val = (*head)->citizenID;
        free(*head);
        *head=NULL;
        return ret_val;
    }

    //if the first node must be delete
    if((*head)->citizenID==citizenID){

        ret_val = delete_first_node(head);
        return ret_val;
    }


    while(current != NULL){

        if(current->citizenID==citizenID){

            if(current->next!=NULL){

                skip_list_node *next_node = current->next;
                ret_val=current->citizenID;
                free(current);
                prev_node->next=next_node;
            }
            else{ //if the deleted node was the last in the list

                ret_val=delete_last_node(*head);
            }

            return ret_val;
        }

        prev_node=current;
        current = current->next;
    }

    return -1;
}




int delete_list(skip_list_node **head){

    int ret_val=0;

    while(ret_val != -1){

        ret_val=delete_first_node(head);
    }

    return 0;
}




int delete_node_skip_list(skip_list **skip_lists, int index, int citizenID){

    int deleted_element=0, max_layer=0;

    max_layer=skip_lists[index]->current_max_layer;

    for(int i=max_layer;i>=0;i--){

        deleted_element=delete_list_node(&skip_lists[index]->head[i], citizenID);

        if(skip_lists[index]->head[i]==NULL){

            skip_lists[index]->current_max_layer = skip_lists[index]->current_max_layer - 1;
        }
    }

    return deleted_element;
}




int delete_skip_list_structure(skip_list **skip_lists, int index){

    for(int i=0;i<index;i++){

        if(skip_lists[i]==NULL){

            break;
        }

        if(skip_lists[i]!=NULL){

            free(skip_lists[i]->virus_name);

            for(int j=0;j<=skip_lists[i]->current_max_layer;j++){

                if(skip_lists[i]->head[j]!=NULL){

                    delete_list(&skip_lists[i]->head[j]);
                }
            }

            free(skip_lists[i]->head);
        }

        free(skip_lists[i]);
    }

    free(skip_lists);

    return 0;
}



int max_index_skip_lists(char **viruses_array, int array_size){

    for(int i=0;i<array_size;i++){

        if(!strcmp(viruses_array[i], "")){

            return (i-1)*2+1;
        }
    }

    return -1;
}



int max_index_virus_array(char **viruses_array, int array_size){

    for(int i=0;i<array_size;i++){

        if(!strcmp(viruses_array[i], "")){

            return i;
        }
    }

    //if array is full
    return -1;
}



int find_index_virus_name(char **viruses_array, int array_size, char *virus_name){

    for(int i=0;i<array_size;i++){

        if(!strcmp(viruses_array[i], virus_name)){

            return i;
        }
    }

    return -1;
}



int is_new_virus(char **viruses_array, int array_size, char *virus_name){

    for(int i=0;i<array_size;i++){

        if(!strcmp(viruses_array[i], virus_name)){
            
            return false;
        }
    }

    return true;
}



int initialize_skip_lists_for_new_virus(skip_list **skip_lists, int max_layer, char **viruses_array, int array_size, char *virus_name, float possibility){

    int index=-1;

    for(int i=0;i<array_size;i++){

        if(!strcmp(viruses_array[i], "")){

            index=i;
            strncpy(viruses_array[index], virus_name, strlen(virus_name));
            
            memset(viruses_array[index]+strlen(virus_name), '\0', 1);

            break;
        }
    }

    if(skip_lists[index*2]==NULL && skip_lists[index*2+1]==NULL){

        skip_lists[index*2]=create_skip_list(skip_lists[index*2], max_layer, possibility, virus_name);
        skip_lists[index*2+1]=create_skip_list(skip_lists[index*2+1], max_layer, possibility, virus_name);

        return 0;
    }

    return -1;

}



int find_index_skip_list(skip_list **skip_lists, int cnt_of_viruses, char *virus_name){

    int x = cnt_of_viruses*2;
    for(int i=0;i<x;i+=2){
        
        if(!strcmp(skip_lists[i]->virus_name, virus_name)){

            return i;
        }
    }

    return -1;
}




skip_list_node *find_entry_skip_list(skip_list **skip_lists, int index, int citizenID){

    int current_level=skip_lists[index]->current_max_layer;

    skip_list_node *current=skip_lists[index]->head[current_level];

    while(current_level>0){

        while(current->next!=NULL && citizenID>=current->next->citizenID){

            current=current->next;
        }

        if(citizenID<current->citizenID){

            current_level = current_level - 1;
            current=skip_lists[index]->head[current_level];
        }
        else if(current->next!=NULL && citizenID>=current->citizenID && citizenID<current->next->citizenID){

            current_level = current_level - 1;
            current=current->lower_layer;
        }
        else if(current->next==NULL && citizenID>=current->citizenID){
            
            current_level = current_level - 1;
            current=current->lower_layer;
        }
    }

    while(current!=NULL){

        if(citizenID==current->citizenID){

            return current;
        }

        current = current->next;
    }


    //if the element is not at the skip list
    return NULL;
}




int there_is_that_entry_in_skip_list(skip_list **skip_lists, int index, int citizenID){

    int current_level=skip_lists[index]->current_max_layer;

    skip_list_node *current=skip_lists[index]->head[current_level];

    while(current_level>0){

        while(current->next!=NULL && citizenID>=current->next->citizenID){

            current=current->next;
        }

        if(citizenID<current->citizenID){

            current_level = current_level - 1;
            current=skip_lists[index]->head[current_level];
        }
        else if(current->next!=NULL && citizenID>=current->citizenID && citizenID<current->next->citizenID){

            current_level = current_level - 1;
            current=current->lower_layer;
        }
        else if(current->next==NULL && citizenID>=current->citizenID){
            
            current_level = current_level - 1;
            current=current->lower_layer;
        }
    }

    while(current!=NULL){

        if(citizenID==current->citizenID){

            return true;
        }

        current = current->next;
    }


    //if the element is not at the skip list
    return false;
}



skip_list_node *search_node_to_insert_skip_list(skip_list **skip_lists, int index, int citizenID){

    int current_level=skip_lists[index]->current_max_layer;

    skip_list_node *current=skip_lists[index]->head[current_level];

    while(current_level>0){

        while(current->next!=NULL && citizenID>=current->next->citizenID){

            current=current->next;
        }

        if(citizenID<current->citizenID){

            current_level = current_level - 1;
            current=skip_lists[index]->head[current_level];
        }
        else if(current->next!=NULL && citizenID>=current->citizenID && citizenID<current->next->citizenID){

            current_level = current_level - 1;
            current=current->lower_layer;
        }
        else if(current->next==NULL && citizenID>=current->citizenID){
            
            current_level = current_level - 1;
            current=current->lower_layer;
        }
    }


    while(current!=NULL){

        if(citizenID>=current->citizenID && current->next!=NULL && citizenID<current->next->citizenID){

            return current;
        }
        else if(citizenID<current->citizenID){

            return current;
        }
        else if(current->next==NULL && citizenID>=current->citizenID){

            return current;
        }

        current = current->next;
    }


    //if the element is not at the skip list
    return NULL;
}



void print_layer_zero_non_vaccinated(skip_list **skip_lists, int index){

    skip_list_node *current = skip_lists[index]->head[0];

    while(current != NULL){

        printf("%d %s %s %s %d\n", current->citizenID, current->info_citizen->name, current->info_citizen->surname,
                                            current->info_citizen->country_list->country, current->info_citizen->age);

        current = current->next;
    }
}



void print_layer_zero_all_info(skip_list **skip_lists, int index){

    skip_list_node *current = skip_lists[index]->head[0];

    char str[4];

    if(index%2==0)
        strcpy(str, "YES");
    else
        strcpy(str, "NO");

    while(current != NULL){

        if(!(strcmp(str, "YES"))){

            printf("%d %s %s %s %d %s %s %d-%d-%d\n", current->citizenID, current->info_citizen->name, current->info_citizen->surname,
                                                  current->info_citizen->country_list->country, current->info_citizen->age, 
                                                  skip_lists[index]->virus_name, str, current->date->day, 
                                                  current->date->month, current->date->year);
        }
        else{

            printf("%d %s %s %s %d %s %s\n", current->citizenID, current->info_citizen->name, current->info_citizen->surname,
                                                current->info_citizen->country_list->country, current->info_citizen->age, 
                                                skip_lists[index]->virus_name, str);

        }


        current = current->next;
    }
}




void print_sub_skip_list(skip_list_node *head){

    skip_list_node *current = head;

    printf("> ");
    while(current != NULL){

        printf("%d\t", current->citizenID);
        current = current->next;
    }
}



void print_skip_list(skip_list **skip_lists, int index){


    for(int i=skip_lists[index]->current_max_layer;i>=0;i--){
        
        print_sub_skip_list(skip_lists[index]->head[i]);
        printf("\n");
    }
}