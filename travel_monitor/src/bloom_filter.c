#include "../include/bloom_filter.h"



int create_bloom_filter(unsigned char **bloom_filter_array, unsigned long int total_bytes){

    *bloom_filter_array = (unsigned char *)calloc(total_bytes, sizeof(unsigned char));
    if(*bloom_filter_array==NULL){

        printf("Sorry, cannot allocate memory\n");
        return -1;
    }

    return 0;
}


int insert_bloom_filter(unsigned char *bloom_filter_array, unsigned long int total_bytes, char *citizenID){

    unsigned long int index=0;
    int current_bit=0, M=16;
    unsigned long int total_bits = total_bytes*8;

    for(int i=0;i<M;i++){
        
        //have 8 bits with value: 00000001
        char tmp=0x01;

        index=hash_i((unsigned char *)citizenID, i) % total_bits;

        current_bit=index%8;
        index=index/8;

        tmp = tmp << 7;
        tmp = tmp >> current_bit;
       
        bloom_filter_array[index] = bloom_filter_array[index] | tmp;
    }

    return 0;
}



int search_bloom_filter(unsigned char *bloom_filter_array, unsigned long int total_bytes, char *citizenID){

    unsigned long int index=0;
    int current_bit=0, M=16;

    unsigned long int total_bits = total_bytes*8;

    for(int i=0;i<M;i++){

        //have 8 bits with value: 00000001
        char tmp=0x01;

        index=hash_i((unsigned char *)citizenID, i) % total_bits;

        current_bit=index%8;
        index=index/8;

        tmp = tmp << 7;

        tmp = tmp >> current_bit;

        tmp = bloom_filter_array[index] & tmp;

        if(tmp==0){
            return false;
        }
    }

    return true;
}


int delete_bloom_filter(unsigned char **bloom_filter_array, int array_size){

    for(int i=0;i<array_size;i++){

        if(bloom_filter_array[i]!=NULL){

            free(bloom_filter_array[i]);
        }
    }

    free(bloom_filter_array);
    return 0;
}