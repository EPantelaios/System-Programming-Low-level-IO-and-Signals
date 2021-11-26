#include "../include/exchange_messages_with_monitor.h"


int communicate_with_monitor_process(child_info *child_array, int num_monitors, int buffer_size, int bloom_size, char *input_dir){

    struct dirent **dir_fifo_list;
    int cnt_dirs=0;
    const int size=1000;
    char str_fifo_dir[20];
    strcpy(str_fifo_dir, "fifo_files");

    DIR *dir_named_pipes=opendir(str_fifo_dir);
    if(dir_named_pipes!=NULL){  //directory exists.

        cnt_dirs=scandir(str_fifo_dir, &dir_fifo_list, NULL, alphasort);

        char filepath[size];
        for (int i=2;i<cnt_dirs;i++){

            sprintf(filepath, "%s/%s", str_fifo_dir, dir_fifo_list[i]->d_name);
            unlink(filepath);
        }

        closedir(dir_named_pipes);
        rmdir(str_fifo_dir);

        //free auto-generated array from function scandir()
        for (int i=0;i<cnt_dirs;i++){
                
            free(dir_fifo_list[i]);
        }
        free(dir_fifo_list);

    }
    else if(ENOENT == errno){ //directory does not exist.

       printf("Directory '%s' does not exist. A new directory has just been created\n", str_fifo_dir);
    }
    else{  //opendir() failed for some other reason.

        printf("Failed to open directory '%s'\n", str_fifo_dir);
    }

    mkdir(str_fifo_dir, 0777);


    //Create named pipes
    for(int i=0;i<num_monitors;i++){

        sprintf(child_array[i].fifo_parent_read, "%s/fifo_read_%d", str_fifo_dir, i);

        if(mkfifo(child_array[i].fifo_parent_read, 0666) == -1){

            if(errno != EEXIST){

                perror("Make fifo: ");
            }
        }


        sprintf(child_array[i].fifo_parent_write, "%s/fifo_write_%d", str_fifo_dir, i);

        if(mkfifo(child_array[i].fifo_parent_write, 0666) == -1){

            if(errno != EEXIST){

                perror("Make fifo: ");
            }
        }
    }



    for(int i=0;i<num_monitors;i++){

        child_array[i].child_pid=fork();
        if(child_array[i].child_pid<0){

            perror("fork error");
            exit(1);
        }

        //for the child process
        if(child_array[i].child_pid==0){

            execl("./Monitor", "Monitor", child_array[i].fifo_parent_write, child_array[i].fifo_parent_read, NULL);
            exit(0);
        }
    }


    //parent process
    char *buf = (char *)calloc(size, sizeof(char));

    for(int i=0;i<num_monitors;i++){
                                                                                        //O_WRONLY|O_CREAT|O_NONBLOCK
        while((child_array[i].fd_parent_write = open(child_array[i].fifo_parent_write, O_WRONLY|O_CREAT)) == -1){

            perror("open named pipe parent");
            //exit(2);
            sleep(1);
        }


        while((child_array[i].fd_parent_read = open(child_array[i].fifo_parent_read, O_RDONLY)) == -1){

            perror("open named pipe parent");
            //exit(2);
            //sleep(1);
        }

    }


    struct dirent **dir_list;
    cnt_dirs=0;
    const int size_virus=100;


    for(int i=0;i<num_monitors;i++){

        clean_str(buf, size);
        sprintf(buf, "%d", buffer_size);
        write_to_pipe(child_array[i].fd_parent_write, buf, (int)strlen(buf), buffer_size, 1);

        clean_str(buf, size);
        sprintf(buf, "%d", bloom_size);
        write_to_pipe(child_array[i].fd_parent_write, buf, (int)strlen(buf), buffer_size, 1);
    }


    DIR *dir=opendir(input_dir);
    if(dir!=NULL){  //directory exists.

        cnt_dirs=scandir(input_dir, &dir_list, NULL, alphasort);

        //dont count "." and ".." directories
        cnt_dirs = cnt_dirs - 2;

        closedir(dir);
    }
    else if(ENOENT == errno){ //directory does not exist.

       printf("Directory '%s' does not exist. Exit...\n", input_dir);
       return -1;
    }
    else{  //opendir() failed for some other reason.

        printf("Failed to open directory '%s'\n", input_dir);
        return -2;
    }

    for(int i=0;i<cnt_dirs;i++){

        insert_country(&child_array[(i%num_monitors)].head_country, dir_list[i+2]->d_name);
    }

    //distribute with round-robin the countries that will manage each process
    for(int i=0;i<num_monitors;i++){

        int cnt=0;

        country_node *current = child_array[i].head_country;

        while(current != NULL){

            cnt++;
            current = current->next;
        }

        //send to child process the number of subdirectories which will be managed
        clean_str(buf, size);
        sprintf(buf, "%d", cnt);
        write_to_pipe(child_array[i].fd_parent_write, buf, (int)strlen(buf), buffer_size, 1);

        current = child_array[i].head_country;
        
        while(current != NULL){

            write_to_pipe(child_array[i].fd_parent_write, current->country, (int)strlen(current->country), buffer_size, 1);

            current = current->next;
        }
    }


    clean_str(buf, size);

    for(int i=0;i<num_monitors;i++){

        clean_str(buf, size);
        read_from_pipe(child_array[i].fd_parent_read, buf, buffer_size);
        child_array[i].cnt_of_viruses=atoi(buf);
        assert(!initialize_viruses_array(&child_array[i].viruses_array, child_array[i].cnt_of_viruses, size_virus));
    }


    for(int i=0;i<num_monitors;i++){

        for(int j=0;j<child_array[i].cnt_of_viruses;j++){

            clean_str(buf, size);
            read_from_pipe(child_array[i].fd_parent_read, buf, buffer_size);
            strcpy(child_array[i].viruses_array[j], buf);
        }
    }


    for(int i=0;i<num_monitors;i++){

        assert(!initialize_bloom_filter(&child_array[i].bloom_filter_array, child_array[i].cnt_of_viruses));
        
        for(int j=0;j<child_array[i].cnt_of_viruses;j++){

            create_bloom_filter(&child_array[i].bloom_filter_array[j], bloom_size);
            read_from_pipe(child_array[i].fd_parent_read, (char *)child_array[i].bloom_filter_array[j], buffer_size);
        }
    }


    for(int i=0;i<num_monitors;i++){

        clean_str(buf, size);
        read_from_pipe(child_array[i].fd_parent_read, buf, buffer_size);

        if(!strcmp(buf, "*I am ready to serve requests*")){

            printf("\nMonitor Process with pid: %d is ready to serve requests!\n", child_array[i].child_pid);
        }
    }


    //free auto-generated array from function scandir()
    for (int i=0;i<cnt_dirs+2;i++){

        free(dir_list[i]);
    }
    free(dir_list);
    free(buf);


    return 0;

}



void print_monitors_info(child_info *child_array, int num_monitors){

    FILE *fp=NULL;

    remove("Monitor_info");

    if((fp=fopen("Monitor_info", "w"))!=NULL){

        for(int i=0;i<num_monitors;i++){

            fprintf(fp, "%d. Monitor PID: %d\n", i+1, child_array[i].child_pid);

            //print all countries for current monitor process
            country_node *current = child_array[i].head_country;

            if(current==NULL){

                fprintf(fp, "List is empty\n");
            }
            
            while(current != NULL){

                fprintf(fp, "%s\n", current->country);
                current = current->next;
            }

            fprintf(fp, "\n");
        }

        fclose(fp);
    }
}