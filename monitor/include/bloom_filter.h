#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hash_functions.h"

int create_bloom_filter(unsigned char **bloom_filter_array, unsigned long int total_bytes);

int insert_bloom_filter(unsigned char *bloom_filter_array, unsigned long int total_bytes, char *citizenID);

int search_bloom_filter(unsigned char *bloom_filter_array, unsigned long int total_bytes, char *citizenID);

int delete_bloom_filter(unsigned char **bloom_filter_array, int array_size);


#endif