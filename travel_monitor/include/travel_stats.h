#ifndef TRAVEL_STATS_H
#define TRAVEL_STATS_H

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct travel_requests{

    int accepted;
    int rejected;
}travel_requests;



typedef struct date_node{

    int day;
    int month;
    int year;

}date_node;



typedef struct travel_stats{

    char *country_to;
    char *virus_name;
    struct date_node date;
    int approved_request;
    struct travel_stats *next;

}travel_stats;



travel_stats *insert_travel_stats(travel_stats **head, travel_stats stats);

int delete_first_node_travel_stats(travel_stats **head);

int delete_list_travel_stats(travel_stats **head);

travel_requests traverse_travel_stats_list(travel_stats *head, char *country, char *virus_name, date_node date1, date_node date2);

travel_requests traverse_travel_stats_list_without_country(travel_stats *head, char *virus_name, date_node date1, date_node date2);

void print_list_travel_stats(travel_stats *head);

int compare_date(date_node date1, date_node date2, date_node cur_date);

int compare_date_six_months(date_node travel_date, date_node vaccination_date);


#endif