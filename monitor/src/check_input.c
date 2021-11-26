#include "../include/check_input.h"


int check_arguments_monitor(int argc, char **argv, char **read_child_pipe, char **write_child_pipe){

    if(argc!=3){

        printf("Please enter the required arguments (-c <citizenRecordsFile> -b <bloomSize>)\nExit.\n");
        exit(1);
    }

    //Edit the parameters given at program

    if(argc > 1 && --argc) 
        *read_child_pipe = *++argv;

    if(argc > 1 && --argc) 
        *write_child_pipe = *++argv;

    return 0;
}




int count_lines(char *filename){

    FILE *fp=NULL;
    char ch;
    int lines=0;

    if ((fp=fopen(filename,"r"))==NULL){

        //if cannot open the file exit
        printf("Could not open file %s\n", filename);
        exit(1);
    }

    while(!feof(fp)){

        ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
        }
    }
    fclose(fp);

    return lines+1;
}



int scan_id_and_age(char *token){

    const char s[4] = " \n";

    if(token!=NULL){

        token = strtok(NULL, s);

        if(!check_if_contain_only_numbers_and_not_letters(token)){

            return -1;
        }

        return atoi(token);
    }
    return atoi(token);
}




int scan_file_date(char *token){

    const char s[5] = " \n-";

    if(token!=NULL){

        token = strtok(NULL, s);

        if(!check_if_contain_only_numbers_and_not_letters(token)){

            return -1;
        }
        
        return atoi(token);
    }
    return atoi(token);
}




char *scan_file_char(char *token){

    const char s[5] = " \n-";

    if(token!=NULL){

        token = strtok(NULL, s);

        if(!contains_only_letters(token)){

            return "-1";
        }

        return token;
    }
    return "-1";
}



char *scan_file_char_virus_name(char *token){

    int i = 0, cnt=0;
    const char c='-';
    const char s[4] = " \n";

    if(token!=NULL){

        token = strtok(NULL, s);

        //check if virus_name has more than one dash symbol (-)
        while(token[i] != '\0'){

            if(token[i]==c){
                cnt++;
            }

            i++;
        }

        if(cnt>1){

            printf("The virus format for '%s' is inconsistent.\n", token);
            return "-1";
        }

        return token;
    }

    return "-1";
}




int count_viruses(int lines){
        
    int cnt=sqrt(lines)*10;

    if(lines<cnt){
        return lines;
    }
    else{
        return cnt;
    }
}




int count_arguments(char *buffer){

    char *token;
    char s[5]=" \n-";
    int cnt=1;

    token = strtok(buffer, s);

    while(token!=NULL){

        token=strtok(NULL, s);
        cnt++;
    }
    
    cnt--;

    return cnt;
}





int count_arguments_without_dash_symbol(char *buffer){

    char *token;
    char s[4]=" \n";
    int cnt=1;

    //char *buf2=calloc(30, sizeof(char));
    //strcpy(buf2, buffer);

    token = strtok(buffer, s);

    while(token!=NULL){

        token=strtok(NULL, s);
        cnt++;
    }
    
    cnt--;
    
    return cnt;
}




int correct_format_record(char *buffer){

    char *token;
    char s[5]=" \n-";
    char s2[4]=" \n";
    int cnt=1;

    token = strtok(buffer, s);

    while(token!=NULL){

        if(cnt==5){
            token=strtok(NULL, s2);
        }
        else{
            token=strtok(NULL, s);
        }
        cnt++;
    }

    cnt--;

    if(cnt==7 || cnt==10)
        return cnt;
    else
        return false;
}



int value_range(int value, int minimum_limit, int maximum_limit){

    if(value>=minimum_limit && value<=maximum_limit)
        return true;
    else
        return false;
}



int check_yes_or_no(char *buf){

    if(buf==NULL){

        return -1;
    }

    if(!strcmp(buf, "YES") || !strcmp(buf, "NO")){

        return true;
    }
    else{

        return false;
    }
}



int correct_date(int day, int month, int year){

    if(day>=1 && day<=31 && month>=1 && month<=12 && year>0)
        return true;
    else
        return false;
}




int check_if_contain_only_numbers_and_not_letters(char *citizen_id_str){

    char *ptr;
    strtol(citizen_id_str, &ptr, 10);

    if (*ptr!='\0' || ptr==citizen_id_str){

        return false;
    } 
    else{

        return true;
    }
}




int contains_only_letters(char *str){

    int i=0;

    if(str!=NULL){

        while(str[i] != '\0'){

            if(!((str[i]>='A' && str[i]<='Z') || (str[i]>='a' && str[i]<='z'))){
                
                return false;
            }

            i++;
        }
    }
    else{
        
        return false;
    }

    return true;
}


void print_error_record(char *record){

    printf("ERROR IN RECORD: %s\n", record);
}


void print_error_input(char *record){

    printf("INPUT ERROR: %s\n", record);
}