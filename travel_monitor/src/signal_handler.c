#include "../include/signal_handler.h"



int set_up_sigchld(){

    struct sigaction setup_action;
    sigset_t signal_mask;

    sigemptyset (&signal_mask);
    sigaddset (&signal_mask, SIGCHLD);

    setup_action.sa_mask = signal_mask;
    setup_action.sa_handler = sigchld_handler;
    setup_action.sa_flags = 0;

    sigaction (SIGCHLD, &setup_action, NULL);

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


void sigchld_handler(int signal){

    if(signal!=SIGCHLD){

        assert(1==0);
    }

    flag_sigchld=1;
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