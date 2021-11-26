#ifndef SIGNAL_HANDLER_TRAVEL_MONITOR_H
#define SIGNAL_HANDLER_TRAVEL_MONITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>


extern int flag_sigchld, flag_sigint, flag_sigquit;


int set_up_sigchld();

int set_up_sigint();

int set_up_sigquit();


void sigchld_handler(int signal);

void sigint_handler(int signal);

void sigquit_handler(int signal);


#endif