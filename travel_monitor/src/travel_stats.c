#include "../include/travel_stats.h"


travel_stats *insert_travel_stats(travel_stats **head, travel_stats stats){

    travel_stats *new_node = (travel_stats *)calloc(1, sizeof(travel_stats));

    new_node->country_to=(char *)calloc(strlen(stats.country_to)+1, sizeof(char));
    strncpy(new_node->country_to, stats.country_to, strlen(stats.country_to));
    memset(new_node->country_to+strlen(stats.country_to), '\0', 1);

    new_node->virus_name=(char *)calloc(strlen(stats.virus_name)+1, sizeof(char));
    strncpy(new_node->virus_name, stats.virus_name, strlen(stats.virus_name));
    memset(new_node->virus_name+strlen(stats.virus_name), '\0', 1);

    new_node->date.day=stats.date.day;
    new_node->date.month=stats.date.month;
    new_node->date.year=stats.date.year;

    new_node->approved_request=stats.approved_request;

    new_node->next = *head;
    *head = new_node;

    return new_node;
}


int delete_first_node_travel_stats(travel_stats **head){

    int ret_val = 0;
    travel_stats *next_node = NULL;

    if(*head == NULL){

        return -1;
    }
    
    next_node = (*head)->next;
    free((*head)->virus_name);
    free((*head)->country_to);
    free(*head);
    *head = next_node;

    return ret_val;
}


int delete_list_travel_stats(travel_stats **head){

    int ret_val=0;
    
    while(ret_val != -1){

        ret_val=delete_first_node_travel_stats(head);
    }

    return 0;
}


travel_requests traverse_travel_stats_list(travel_stats *head, char *country, char *virus_name, date_node date1, date_node date2){

    travel_requests requests;
    requests.accepted=0;
    requests.rejected=0;
    travel_stats *current = head;

    while(current != NULL){
        
        if(!strcmp(current->country_to, country) && !strcmp(current->virus_name, virus_name)){

            if(compare_date(date1, date2, current->date)==true){

                if(current->approved_request==1){

                    requests.accepted++;
                }
                else{

                    requests.rejected++;
                }
            }
        }

        current = current->next;
    }

    return requests;
}



travel_requests traverse_travel_stats_list_without_country(travel_stats *head, char *virus_name, date_node date1, date_node date2){

    travel_requests requests;
    requests.accepted=0;
    requests.rejected=0;
    travel_stats *current = head;

    while(current != NULL){
        
        if(!strcmp(current->virus_name, virus_name)){

            if(compare_date(date1, date2, current->date)==true){

                if(current->approved_request==1){

                    requests.accepted++;
                }
                else{

                    requests.rejected++;
                }
            }
        }

        current = current->next;
    }

    return requests;
}



void print_list_travel_stats_list(travel_stats *head){

    travel_stats *current = head;

    while(current != NULL){
        
        printf("| %s %s %d-%d-%d  Approved=%d |\n", current->country_to, current->virus_name,
                current->date.day, current->date.month, current->date.year, current->approved_request);
       
        current = current->next;
    }

    printf("\n");
}



int compare_date(date_node date1, date_node date2, date_node cur_date){

    int cnt=0;

    if(date1.year<cur_date.year)
        cnt++;
    else if(date1.year==cur_date.year && date1.month<cur_date.month)
        cnt++;
    else if(date1.year==cur_date.year && date1.month==cur_date.month && date1.day<=cur_date.day)
        cnt++;

    if(date2.year>cur_date.year)
        cnt++;
    else if(date2.year==cur_date.year && date2.month>cur_date.month)
        cnt++;
    else if(date2.year==cur_date.year && date2.month==cur_date.month && date2.day>=cur_date.day)
        cnt++;

    if(cnt==2)
        return true;
    else
        return false;
}




int compare_date_six_months(date_node travel_date, date_node vaccination_date){

    //Must travel_date be at most six months before vaccination_date.
    //For this reason convert dates to distinct days that have passed since 1970.

    int travel_date_in_days_from_1970 = (travel_date.year - 1970)*360 + travel_date.month*30 + travel_date.day;
    int vaccination_date_in_days_from_1970 = (vaccination_date.year - 1970)*360 + vaccination_date.month*30 + vaccination_date.day;

    int remaining_days = travel_date_in_days_from_1970 - vaccination_date_in_days_from_1970;

    if(remaining_days >= 0 && remaining_days < 180){

        return true;
    }
    else{

        return false;
    }
}