#include "../include/initialize_travelMonitor.h"



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



int initialize_viruses_array(char ***viruses_array, int cnt_of_viruses, int size){

    *viruses_array = (char **)calloc(cnt_of_viruses, sizeof(char *));
    
    for(int i=0;i<cnt_of_viruses;i++){

        (*viruses_array)[i]=(char *)calloc(size, sizeof(char));
    }

    return 0;
}



int max_index_virus_array(char **viruses_array, int array_size){

    for(int i=0;i<array_size;i++){

        if(!strcmp(viruses_array[i], "")){

            return i-1;
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


int find_max_integer_from_array(int *int_array, int size){

    int max_int = INT_MIN;

    for(int i=0;i<size;i++){

        if(int_array[i]>max_int){

            max_int=int_array[i];
        }
    }

    return max_int;
}



void print_commands(){

    printf("\n\n--->  Enter one of the following commands  <---\n\n");
    printf("/travelRequest citizenID date countryFrom countryTo virusName\n");
    printf("/travelStats virusName date1 date2 [country]\n");
    printf("/addVaccinationRecords country\n");
    printf("/searchVaccinationStatus citizenID\n");
    printf("/exit\n\n");
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