#include "../include/initialize.h"


int initialize_aux_vars(char **buffer, char **tmp_buf, char **aux_buf, char **citizen_id_str, 
                    char **country, char **virus_name, char **yes_or_no, int buffersize, int size){


    *buffer=(char *)calloc(buffersize, sizeof(char));
    *tmp_buf=(char *)calloc(buffersize, sizeof(char));
    *aux_buf=(char *)calloc(buffersize, sizeof(char));

    *citizen_id_str=(char *)calloc(size, sizeof(char));
    *country=(char *)calloc(size, sizeof(char));
    *virus_name=(char *)calloc(size, sizeof(char));
    *yes_or_no=(char *)calloc(size, sizeof(char));

    return 0;
}


int initialize_bloom_filter(unsigned char ***bloom_filter_array, int size){

    *bloom_filter_array = (unsigned char **)calloc(size, sizeof(unsigned char *));
    for(int i=0;i<size;i++){

        (*bloom_filter_array)[i]=NULL;
    }

    return 0;
}


int initialize_skip_list(skip_list ***skip_lists, int size){

    *skip_lists = (skip_list **)calloc(size, sizeof(skip_list *));

    for(int i=0;i<size;i++){

        (*skip_lists)[i]=NULL;
    }

    return 0;
}


int initialize_viruses_array(char ***viruses_array, int cnt_of_viruses, int size){

    *viruses_array = (char **)calloc(cnt_of_viruses, sizeof(char *));
    
    for(int i=0;i<cnt_of_viruses;i++){

        (*viruses_array)[i]=(char *)calloc(size, sizeof(char));
    }

    return 0;
}


int clean_str(char *buf, int buffer_size){
        
    if(buf==NULL){

        printf("String is NULL\n");
        assert(1==0);
        return -1;
    }

    for(int i=0;i<buffer_size;i++){

        buf[i]='\0';
    }

    return 0;
}