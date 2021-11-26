#ifndef CHECK_INPUT_H
#define CHECK_INPUT_H

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include <dirent.h>
#include <errno.h>


int check_arguments(int argc, char **argv, int *num_monitors, int *buffer_size, int *bloom_size, char **input_dir);

int check_number_of_counties(int *num_monitors, char *input_dir);

int count_lines(char *filename);

int scan_id_and_age(char *token);

int scan_file_date(char * token);

char *scan_file_char(char * token);

char *scan_file_char_virus_name(char *token);

int count_viruses(int lines);

int correct_format_record(char *buffer);

int count_arguments(char *buffer);

int count_arguments_without_dash_symbol(char *buffer);

int value_range(int value, int minimum_limit, int maximum_limit);

void print_error_record(char *record);

void print_error_input(char *record);

int check_if_contain_only_numbers_and_not_letters(char *citizen_id_str);

int correct_date(int day, int month, int year);

int contains_only_letters(char *str);

int check_yes_or_no(char *buf);


#endif