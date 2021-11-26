#include "../include/skip_list.h"
#include "../include/bloom_filter.h"
#include "../include/info_citizen_list.h"
#include "../include/country_list.h"
#include "../include/date_list.h"
#include "../include/check_input.h"
#include "../include/insert_entry.h"
#include "../include/initialize.h"
#include "../include/read_write_pipe.h"
#include "../include/signal_handler.h"



int flag_sigusr1, flag_sigint, flag_sigquit;



int main(int argc, char *argv[]){

    FILE *fp=NULL;
    int ret_val=0, cnt_of_viruses=0, number_of_args=0, stored_cnt_files=0;
    const int size=1000;
    const int size_virus=100;
    int num_l=25;
    flag_sigusr1=0;
    flag_sigint=0;
    flag_sigquit=0;
    unsigned long long int current_population_earth=7000000000;
    const int max_layer=(int)log2(current_population_earth);
    size_t buffersize=1000;
    char *buffer, *tmp_buf, *aux_buf, *citizen_id_str, *country, *virus_name, *yes_or_no;
    char *read_child_pipe;
    char *write_child_pipe;
    int lines=0;

    travel_requests_monitor travel_requests_stats;
    travel_requests_stats.accepted=0;
    travel_requests_stats.rejected=0;

    struct dirent **dir_list;
    int cnt_files=0;

    //to generate pseudo-random numbers
    srand(time(NULL));

    //head pointers for citizen, country and date list
    info_citizen_node *head_citizen=NULL;
    country_node *head_country=NULL;
    vaccination_date_node *head_date=NULL;

    check_arguments_monitor(argc, argv,  &read_child_pipe, &write_child_pipe);

    assert(!initialize_aux_vars(&buffer, &tmp_buf, &aux_buf, &citizen_id_str, &country, &virus_name, 
                    &yes_or_no, buffersize, size_virus));

    int special_code=0;
    int fd_read, fd_write;
    char *buf = (char *)calloc(size, sizeof(char));

    //setup signals handler
    assert(!set_up_sigusr1());
    assert(!set_up_sigint());
    assert(!set_up_sigquit());

    while((fd_read = open(read_child_pipe, O_RDONLY)) == -1){

        perror("open named pipe child");
        //exit(1);
    }

    while((fd_write = open(write_child_pipe, O_WRONLY|O_CREAT)) == -1){

        perror("open named pipe child");
        sleep(1);
    }

    clean_str(buf, size);
    special_code=read_from_pipe(fd_read, buf, 0); //Get buffer size
    const int buffer_size = atoi(buf);

    clean_str(buf, size);
    special_code=read_from_pipe(fd_read, buf, buffer_size); //Get buffer size
    const int bloom_size = atoi(buf);

    clean_str(buf, size);
    special_code=read_from_pipe(fd_read, buf, buffer_size); //Get buffer size
    int cnt_countries = atoi(buf);


    for(int i=0;i<cnt_countries;i++){

        clean_str(buf, size);
        special_code=read_from_pipe(fd_read, buf, buffer_size); //Get buffer size
        insert_country(&head_country, buf);
    }

    char *path_country=calloc(size_virus, sizeof(char));
    
    cnt_of_viruses=count_viruses(num_l);
    int tmp_viruses=cnt_of_viruses*2;

    char **viruses_array;
    assert(!initialize_viruses_array(&viruses_array, cnt_of_viruses, size_virus));

    unsigned char **bloom_filter_array;
    assert(!initialize_bloom_filter(&bloom_filter_array, cnt_of_viruses));

    skip_list **skip_lists;
    assert(!initialize_skip_list(&skip_lists, tmp_viruses));


    //access the list of countries
    country_node *current = head_country;

    while(current != NULL){

        clean_str(path_country, size_virus);
        //maybe "../input_dir/"
        strcpy(path_country, "input_dir/");
        strcat(path_country, current->country);


        DIR *dir=opendir(path_country);
        if(dir!=NULL){  //directory exists.

            cnt_files=scandir(path_country, &dir_list, NULL, alphasort);

            //store the number of files in case they are added new files
            stored_cnt_files=cnt_files;
        }
        else if(ENOENT == errno){ //directory does not exist.

            printf("Directory '%s' does not exist. Exit...\n", path_country);
            exit(1);
        }
        else{  //opendir() failed for some other reason.

            printf("Failed to open directory '%s'\n", path_country);
            exit(2);
        }


        for (int i=2;i<cnt_files;i++){

            clean_str(path_country, size_virus);
            //maybe "../input_dir/"
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

                        ret_val = insert_entry(skip_lists, viruses_array, cnt_of_viruses, bloom_filter_array, bloom_size, &head_country, &head_citizen,
                                            &head_date, max_layer, buffer, tmp_buf, number_of_args);

                        if(ret_val<0){

                            continue;
                        }
                    }
                }
            }

            fclose(fp);
        }

        //free auto-generated array from function scandir()
        for (int i=0;i<cnt_files;i++){

            free(dir_list[i]);
        }
        free(dir_list);

        current = current->next;
    }


    int index_virus_array = max_index_virus_array(viruses_array, cnt_of_viruses);

    clean_str(buf, size);
    sprintf(buf, "%d", index_virus_array);
    write_to_pipe(fd_write, buf, (int)strlen(buf), buffer_size, 2);

    for(int i=0;i<index_virus_array;i++){

        write_to_pipe(fd_write, viruses_array[i], (int)strlen(viruses_array[i]), buffer_size, 2);
    }

    for(int i=0;i<index_virus_array;i++){
       
        write_to_pipe(fd_write, (char *)bloom_filter_array[i], bloom_size, buffer_size, 2);
    }

    clean_str(buf, size);
    strcpy(buf, "*I am ready to serve requests*");
    write_to_pipe(fd_write, buf, (int)strlen(buf), buffer_size, 2);


    //Child process wait for commands from parent.
    int citizen_id;
    char date[10];
    while(1){

        while(1){

            int cnt_fds=0;
            fd_set current_fds, ready_fds;

            FD_ZERO(&current_fds);

            FD_SET(fd_read, &current_fds);

            struct timeval time_out;
            time_out.tv_sec = 0;
            time_out.tv_usec = 100000; // 0.1 second

            ready_fds = current_fds;
            
            cnt_fds=select(fd_read+1, &ready_fds, NULL, NULL, &time_out);

            //if SIGUSR1 happens
            if(flag_sigusr1==1){

                printf("\nSIGUSR1 Signal!\n");

                assert(!update_structures_sigusr1_signal(skip_lists, viruses_array, cnt_of_viruses, bloom_filter_array, bloom_size,
                        &head_country, &head_citizen, &head_date, max_layer, stored_cnt_files, fd_write, buffer_size));

                flag_sigusr1=0;
                fflush(NULL);
                continue;
            }


            //if SIGINT or SIGQUIT happens
            if(flag_sigint==1 || flag_sigquit==1){

                if(flag_sigint==1)
                    printf("\nSIGINT Signal!\n");
                else
                    printf("\nSIGQUIT Signal!\n");

                //Create "log_file" with countries and requests
                FILE *fp=NULL;
                clean_str(buf, size);
                sprintf(buf, "log_file.%d", getpid());

                if((fp=fopen(buf, "w"))!=NULL){

                    //print all countries for current monitor process
                    country_node *current = head_country;

                    if(current==NULL){

                        fprintf(fp, "List is empty\n");
                    }
                    
                    while(current != NULL){

                        fprintf(fp, "%s\n", current->country);
                        current = current->next;
                    }

                    fprintf(fp, "\n");

                    fprintf(fp, "TOTAL TRAVEL REQUESTS %d\n", travel_requests_stats.accepted + travel_requests_stats.rejected);
                    fprintf(fp, "ACCEPTED %d\n", travel_requests_stats.accepted);
                    fprintf(fp, "REJECTED %d\n", travel_requests_stats.rejected);

                    fclose(fp);
                }

                delete_list_date(&head_date);
                delete_list_citizen(&head_citizen);
                delete_list_country(&head_country);
                delete_skip_list_structure(skip_lists, cnt_of_viruses*2);
                delete_bloom_filter(bloom_filter_array, cnt_of_viruses);

                for(int i=0;i<cnt_of_viruses;i++){

                    free(viruses_array[i]);
                }
                free(viruses_array);

                free(path_country);
                free(buf);
                free(buffer);
                free(aux_buf);
                free(tmp_buf);
                free(citizen_id_str);
                free(country);
                free(virus_name);
                free(yes_or_no);

                flag_sigint=0;
                flag_sigquit=0;

                exit(0);
            }


            if(cnt_fds < 0){

                perror("Select() error");
                exit(-1);
            }
            else if(cnt_fds > 0){

                if(FD_ISSET(fd_read, &ready_fds)){

                    clean_str(buf, size);
                    special_code=read_from_pipe(fd_read, buf, buffer_size);
                    if(special_code==-1){
                        
                        int status=0;

                        pid_t wait_ret = waitpid(getppid(), &status, WNOHANG);
                        if(wait_ret>0){

                            printf("\nParent process has died\n");
                            close(fd_write);
                            close(fd_read);
                            exit(1);
                        }
                    }
                    else{

                        break;
                    }
                }
            
            }
        }


        if(special_code==3){ //travelRequest

            citizen_id=atoi(buf);

            clean_str(buf, size);
            special_code=read_from_pipe(fd_read, buf, buffer_size);
            strcpy(virus_name, buf);

            clean_str(buf, size);
            special_code=read_from_pipe(fd_read, buf, buffer_size);
            strcpy(country, buf);


            int index=find_index_skip_list(skip_lists, cnt_of_viruses, virus_name);

            skip_list_node *node=find_entry_skip_list(skip_lists, index, citizen_id);
            if(node!=NULL && !strcmp(node->info_citizen->country_list->country, country)){

                clean_str(buf, size);
                sprintf(date, "%d-%d-%d", node->date->day, node->date->month, node->date->year);
                strcpy(buf, date);

                write_to_pipe(fd_write, buf, (int)strlen(buf), buffer_size, 4);
                travel_requests_stats.accepted++;
            }
            else{
                
                clean_str(buf, size);
                write_to_pipe(fd_write, buf, 0, buffer_size, 5);
                travel_requests_stats.rejected++;
            }

        }
        else if(special_code==6){ //searchVaccinationStatus citizenID

            int cnt_nodes=0;
            char skip_list_buf[size_virus];

            citizen_id=atoi(buf);
            
            int max_index = max_index_skip_lists(viruses_array, cnt_of_viruses); 

            skip_list_node *node;

            for(int i=0;i<max_index+1;i++){

                if((node=find_entry_skip_list(skip_lists, i, citizen_id))!=NULL){
                    
                    //send at parent process the personal data of the citizen only the first time
                    if(cnt_nodes==0){
                        
                        clean_str(skip_list_buf, size_virus);
                        sprintf(skip_list_buf, "%d %s %s %s", node->citizenID, node->info_citizen->name, node->info_citizen->surname,
                                node->info_citizen->country_list->country);

                        write_to_pipe(fd_write, skip_list_buf, (int)strlen(skip_list_buf), buffer_size, 7);

                        clean_str(skip_list_buf, size_virus);
                        sprintf(skip_list_buf, "AGE %d", node->info_citizen->age); 
                        write_to_pipe(fd_write, skip_list_buf, (int)strlen(skip_list_buf), buffer_size, 7);

                        cnt_nodes++;
                    }

                    clean_str(skip_list_buf, size_virus);
                    if(i%2==0){   //For vaccinated skip_list
                        
                        clean_str(skip_list_buf, size_virus);
                        sprintf(skip_list_buf, "%s VACCINATED ON %d-%d-%d", skip_lists[i]->virus_name, node->date->day, node->date->month, node->date->year);
                        write_to_pipe(fd_write, skip_list_buf, (int)strlen(skip_list_buf), buffer_size, 8);
                    }
                    else{   //For non-vaccinated skip list

                        clean_str(skip_list_buf, size_virus);
                        sprintf(skip_list_buf, "%s NOT YET VACCINATED", skip_lists[i]->virus_name);
                        write_to_pipe(fd_write, skip_list_buf, (int)strlen(skip_list_buf), buffer_size, 8);
                    }
                }
            }

            if(cnt_nodes==1){
                
                //send terminated messages with special code: 9
                clean_str(skip_list_buf, size_virus);
                write_to_pipe(fd_write, skip_list_buf, 0, buffer_size, 9);
            }

        }
        else if(special_code==-1){

            printf("Parent pid = -%d-\n", getppid());
            printf("Parent process has terminated\n");
            close(fd_read);
            close(fd_write);
            exit(0);
        }

    }


    delete_list_date(&head_date);
    delete_list_citizen(&head_citizen);
    delete_list_country(&head_country);

    delete_skip_list_structure(skip_lists, cnt_of_viruses*2);

    delete_bloom_filter(bloom_filter_array, cnt_of_viruses);

    for(int i=0;i<cnt_of_viruses;i++){

        free(viruses_array[i]);
    }
    free(viruses_array);

    free(buffer);
    free(aux_buf);
    free(tmp_buf);
    free(citizen_id_str);
    free(country);
    free(virus_name);
    free(yes_or_no);

    exit(0);
}