#include "../include/signal_handler.h"


int set_up_sigusr1(){

    struct sigaction setup_action;
    sigset_t signal_mask;

    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGUSR1);

    setup_action.sa_mask = signal_mask;
    setup_action.sa_handler = sigusr1_handler;
    setup_action.sa_flags = 0;

    sigaction (SIGUSR1, &setup_action, NULL);

    return 0;
}


int set_up_sigint(){

    struct sigaction setup_action;
    sigset_t signal_mask;

    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGINT);

    setup_action.sa_mask = signal_mask;
    setup_action.sa_handler = sigint_handler;
    setup_action.sa_flags = 0;

    sigaction (SIGINT, &setup_action, NULL);

    return 0;
}


int set_up_sigquit(){

    struct sigaction setup_action;
    sigset_t signal_mask;

    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGQUIT);

    setup_action.sa_mask = signal_mask;
    setup_action.sa_handler = sigquit_handler;
    setup_action.sa_flags = 0;

    sigaction (SIGQUIT, &setup_action, NULL);

    return 0;
}



void sigusr1_handler(int signal){

    if(signal!=SIGUSR1){

        assert(1==0);
    }
    flag_sigusr1=1;
}



void sigint_handler(int signal){

    if(signal!=SIGINT){

        assert(1==0);
    }
    flag_sigint=1;
}



void sigquit_handler(int signal){

    if(signal!=SIGQUIT){

        assert(1==0);
    }
    flag_sigquit=1;
}



int update_structures_sigusr1_signal(skip_list **skip_lists, char **viruses_array, int cnt_of_viruses, unsigned char **bloom_filter_array, 
                          int bloom_size, country_node **head_country, info_citizen_node **head_citizen, 
                          vaccination_date_node **head_date, int max_layer, int stored_cnt_files, int fd_write, int buffer_size){

    FILE *fp=NULL;
    char path_country[100];
    int number_of_args=0, lines=0, ret_val=0;
    struct dirent **dir_list;
    int cnt_files=0;
    size_t buffersize=1000;
    const int size=1000;
    char *buffer=calloc(size, sizeof(char));
    char *tmp_buf=calloc(size, sizeof(char));
    char *aux_buf=calloc(size, sizeof(char));

    country_node *current = *head_country;

    while(current != NULL){

        clean_str(path_country, 100);
        strcpy(path_country, "input_dir/");
        strcat(path_country, current->country);

        DIR *dir=opendir(path_country);
        if(dir!=NULL){  //directory exists.

            cnt_files=scandir(path_country, &dir_list, NULL, alphasort);

            if(cnt_files!=stored_cnt_files){

                break;
            }

        }
        else if(ENOENT == errno){ //directory does not exist.

            printf("Directory '%s' does not exist. Exit...\n", path_country);
            return 1;
        }
        else{  //opendir() failed for some other reason.

            printf("Failed to open directory '%s'\n", path_country);
            return 2;
        }

        current=current->next;
    }

    for (int i=stored_cnt_files;i<cnt_files;i++){

        clean_str(path_country, 100);

        strcpy(path_country, "input_dir/");
        strcat(path_country, current->country);
        strcat(path_country, "/");
        strcat(path_country, dir_list[i]->d_name);

        lines =count_lines(path_country);

        if((fp=fopen(path_country, "r"))!=NULL){

            for(int j=0;j<lines;j++){

                if(getline(&buffer, &buffersize, fp)!=(EOF)){
                    
                    //cnt_lines++;

                    strcpy(tmp_buf ,buffer);
                    strcpy(aux_buf, buffer);

                    if(buffer==NULL || buffer[0]=='\n'){

                        continue;
                    }

                    //check the format and the number of tokens of the record
                    number_of_args=correct_format_record(aux_buf);
                    if(number_of_args==false){

                        print_error_record(aux_buf);
                        continue;
                    }

                    
                    ret_val = insert_entry(skip_lists, viruses_array, cnt_of_viruses, bloom_filter_array, bloom_size, 
                                           head_country, head_citizen, head_date, max_layer, buffer, tmp_buf, number_of_args);

                    if(ret_val<0){

                        continue;
                    }
                }
            }
        }

        fclose(fp);
    }

    int index_virus_array = max_index_virus_array(viruses_array, cnt_of_viruses);

    clean_str(buffer, size);
    sprintf(buffer, "%d", index_virus_array);
    write_to_pipe(fd_write, buffer, (int)strlen(buffer), buffer_size, 10);

    for(int i=0;i<index_virus_array;i++){

        write_to_pipe(fd_write, viruses_array[i], (int)strlen(viruses_array[i]), buffer_size, 11);
    }

    for(int i=0;i<index_virus_array;i++){
       
        write_to_pipe(fd_write, (char *)bloom_filter_array[i], bloom_size, buffer_size, 12);
    }

    free(buffer);
    free(tmp_buf);
    free(aux_buf);

    return 0;
}