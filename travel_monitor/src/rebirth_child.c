#include "../include/rebirth_child.h"



int initialize_child_after_terminate(child_info *child_array, int index, int buffer_size, int bloom_size, char *input_dir){

    const int size=1000;
    //parent process
    char *buf = (char *)calloc(size, sizeof(char));

    while((child_array[index].fd_parent_write = open(child_array[index].fifo_parent_write, O_WRONLY|O_CREAT)) == -1){

        perror("open named pipe parent");
        sleep(1);
    }

    while((child_array[index].fd_parent_read = open(child_array[index].fifo_parent_read, O_RDONLY)) == -1){

        perror("open named pipe parent");
    }

    const int size_virus=100;


    clean_str(buf, size);
    sprintf(buf, "%d", buffer_size);
    write_to_pipe(child_array[index].fd_parent_write, buf, (int)strlen(buf), buffer_size, 1);

    clean_str(buf, size);
    sprintf(buf, "%d", bloom_size);
    write_to_pipe(child_array[index].fd_parent_write, buf, (int)strlen(buf), buffer_size, 1);


    //distribute with round-robin the countries that will manage each process
    int cnt=0;
    country_node *current = child_array[index].head_country;

    while(current != NULL){

        cnt++;
        current = current->next;
    }

    //send to child process the number of subdirectories which will be managed
    clean_str(buf, size);
    sprintf(buf, "%d", cnt);
    write_to_pipe(child_array[index].fd_parent_write, buf, (int)strlen(buf), buffer_size, 1);

    current = child_array[index].head_country;
    
    while(current != NULL){

        write_to_pipe(child_array[index].fd_parent_write, current->country, (int)strlen(current->country), buffer_size, 1);

        current = current->next;
    }


    clean_str(buf, size);

    read_from_pipe(child_array[index].fd_parent_read, buf, buffer_size);
    child_array[index].cnt_of_viruses=atoi(buf);

    assert(!initialize_viruses_array(&child_array[index].viruses_array, child_array[index].cnt_of_viruses, size_virus));

    for(int j=0;j<child_array[index].cnt_of_viruses;j++){

        clean_str(buf, size);
        read_from_pipe(child_array[index].fd_parent_read, buf, buffer_size);
        strcpy(child_array[index].viruses_array[j], buf);
    }

    assert(!initialize_bloom_filter(&child_array[index].bloom_filter_array, child_array[index].cnt_of_viruses));

    for(int j=0;j<child_array[index].cnt_of_viruses;j++){

        create_bloom_filter(&child_array[index].bloom_filter_array[j], bloom_size);
        read_from_pipe(child_array[index].fd_parent_read, (char *)child_array[index].bloom_filter_array[j], buffer_size);
    }

    clean_str(buf, size);
    read_from_pipe(child_array[index].fd_parent_read, buf, buffer_size);

    if(!strcmp(buf, "*I am ready to serve requests*")){

        printf("\nMonitor Process with pid: %d is ready to serve requests!\n", child_array[index].child_pid);
    }

    return 0;
}