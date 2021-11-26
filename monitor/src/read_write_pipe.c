#include "../include/read_write_pipe.h"


//return the number code of action to do
int read_from_pipe(int fd, char *buf, unsigned int buffer_size){
    
    int n_bytes=0;
    int cnt_bytes=0;
    int special_code=0;
    int bytes_to_read=0;
    char *tmp_buf=calloc(buffer_size, sizeof(char));

    //In the first message child process does not know the size of buffer
    if(buffer_size==0){

        buffer_size=sizeof(unsigned int);
    }

    //read 4 bytes to take the special code
    n_bytes = read(fd, &special_code, sizeof(int));
    if(n_bytes!=sizeof(unsigned int)){

        return -1;
    }

    //read next 4 bytes to take the number of bytes to read
    clean_str(tmp_buf, buffer_size);
    cnt_bytes = read(fd, &bytes_to_read, sizeof(int));
    if(cnt_bytes!=4){

        assert(1==0);
        return -1;
    }

    if(buffer_size>bytes_to_read){

        buffer_size=bytes_to_read;
    }

    int total_bytes=0;
    //read the message
    while(bytes_to_read>0){

        clean_str(tmp_buf, buffer_size);
        n_bytes = read(fd, tmp_buf, buffer_size);

        memmove(buf+total_bytes, tmp_buf, n_bytes);
        total_bytes+=n_bytes;

        bytes_to_read -= n_bytes;

        if(buffer_size>bytes_to_read){

            buffer_size=bytes_to_read;
        }
    }

    free(tmp_buf);

    return special_code;
}





//return the number code of action to do
int write_to_pipe(int fd, char *buf, int bytes_to_write, unsigned int buffer_size, int special_code){

    int n_bytes=0;
    int cnt_bytes=0;
    char *tmp_buf=calloc(buffer_size, sizeof(char));

    //write 4 bytes the special code
    sprintf(tmp_buf, "%d", special_code);

    n_bytes = write(fd, &special_code, sizeof(int));
    if(n_bytes!=sizeof(unsigned int)){

        assert(1==0);
    }

    cnt_bytes = write(fd, &bytes_to_write, sizeof(int));
    if(cnt_bytes!=4){

        assert(1==0);
    }


    if(buffer_size > bytes_to_write){

        buffer_size = bytes_to_write;
    }


    //write the message
    while(bytes_to_write>0){

        
        n_bytes = write(fd, buf, buffer_size);

        buf=buf+n_bytes;
        bytes_to_write -= n_bytes;

        if(buffer_size > bytes_to_write){

            buffer_size = bytes_to_write;
        }
    }

    free(tmp_buf);
    return 0;
}