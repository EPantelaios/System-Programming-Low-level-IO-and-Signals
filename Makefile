CC 		= gcc
FLAGS	= -std=gnu99 -Wall -c


#For Monitor Program
SRC_MONITOR	= monitor/src
INC_MONITOR	= monitor/include

OBJS_MONITOR	= monitor.o skip_list.o bloom_filter.o hash_functions.o check_input.o initialize.o \
				  insert_entry.o info_citizen_list.o country_list.o date_list.o \
				  read_write_pipe.o signal_handler.o

OUT_MONITOR = Monitor

$(OUT_MONITOR): $(OBJS_MONITOR)
	$(CC) $(OBJS_MONITOR) -o $@ -lm



#For travelMonitor Program
SRC_TRAVEL	= travel_monitor/src
INC_TRAVEL	= travel_monitor/include

OBJS_TRAVEL_MONITOR	= travelMonitor.o country_list_travel.o bloom_filter_travel.o hash_functions_travel.o \
					  check_input_travel.o initialize_travelMonitor.o exchange_messages_with_monitor.o \
					  read_write_pipe_travel.o rebirth_child.o signal_handler_travel.o travel_stats.o

OUT_TRAVEL_MONITOR = travelMonitor

$(OUT_TRAVEL_MONITOR): $(OBJS_TRAVEL_MONITOR)
	$(CC) $(OBJS_TRAVEL_MONITOR) -o $@ -lm




#Type 'make' instead of 'make all'
.DEFAULT_GOAL = all
all: $(OUT_MONITOR) $(OUT_TRAVEL_MONITOR)





#Compile Monitor Program
monitor.o: $(SRC_MONITOR)/monitor.c
	$(CC) $(FLAGS) $(SRC_MONITOR)/monitor.c

skip_list.o: $(SRC_MONITOR)/skip_list.c $(INC_MONITOR)/skip_list.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/skip_list.c

bloom_filter.o: $(SRC_MONITOR)/bloom_filter.c $(INC_MONITOR)/bloom_filter.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/bloom_filter.c

hash_functions.o: $(SRC_MONITOR)/hash_functions.c $(INC_MONITOR)/hash_functions.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/hash_functions.c

check_input.o: $(SRC_MONITOR)/check_input.c $(INC_MONITOR)/check_input.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/check_input.c

initialize.o: $(SRC_MONITOR)/initialize.c $(INC_MONITOR)/initialize.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/initialize.c

insert_entry.o: $(SRC_MONITOR)/insert_entry.c $(INC_MONITOR)/insert_entry.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/insert_entry.c

info_citizen_list.o: $(SRC_MONITOR)/info_citizen_list.c $(INC_MONITOR)/info_citizen_list.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/info_citizen_list.c

country_list.o: $(SRC_MONITOR)/country_list.c $(INC_MONITOR)/country_list.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/country_list.c

date_list.o: $(SRC_MONITOR)/date_list.c $(INC_MONITOR)/date_list.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/date_list.c

read_write_pipe.o: $(SRC_MONITOR)/read_write_pipe.c $(INC_MONITOR)/read_write_pipe.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/read_write_pipe.c

signal_handler.o: $(SRC_MONITOR)/signal_handler.c $(INC_MONITOR)/signal_handler.h
	$(CC) $(FLAGS) $(SRC_MONITOR)/signal_handler.c






#Compile travelMonitor Program
travelMonitor.o: $(SRC_TRAVEL)/main.c
	$(CC) $(FLAGS) -o travelMonitor.o $(SRC_TRAVEL)/main.c

country_list_travel.o: $(SRC_TRAVEL)/country_list.c $(INC_TRAVEL)/country_list.h
	$(CC) $(FLAGS) -o country_list_travel.o $(SRC_TRAVEL)/country_list.c

bloom_filter_travel.o: $(SRC_TRAVEL)/bloom_filter.c $(INC_TRAVEL)/bloom_filter.h
	$(CC) $(FLAGS) -o bloom_filter_travel.o $(SRC_TRAVEL)/bloom_filter.c

hash_functions_travel.o: $(SRC_TRAVEL)/hash_functions.c $(INC_TRAVEL)/hash_functions.h
	$(CC) $(FLAGS) -o hash_functions_travel.o $(SRC_TRAVEL)/hash_functions.c

check_input_travel.o: $(SRC_TRAVEL)/check_input.c $(INC_TRAVEL)/check_input.h
	$(CC) $(FLAGS) -o check_input_travel.o $(SRC_TRAVEL)/check_input.c

initialize_travelMonitor.o: $(SRC_TRAVEL)/initialize_travelMonitor.c $(INC_TRAVEL)/initialize_travelMonitor.h
	$(CC) $(FLAGS) $(SRC_TRAVEL)/initialize_travelMonitor.c

exchange_messages_with_monitor.o: $(SRC_TRAVEL)/exchange_messages_with_monitor.c $(INC_TRAVEL)/exchange_messages_with_monitor.h
	$(CC) $(FLAGS) $(SRC_TRAVEL)/exchange_messages_with_monitor.c

read_write_pipe_travel.o: $(SRC_TRAVEL)/read_write_pipe.c $(INC_TRAVEL)/read_write_pipe.h
	$(CC) $(FLAGS) -o read_write_pipe_travel.o $(SRC_TRAVEL)/read_write_pipe.c

rebirth_child.o: $(SRC_TRAVEL)/rebirth_child.c $(INC_TRAVEL)/rebirth_child.h
	$(CC) $(FLAGS) $(SRC_TRAVEL)/rebirth_child.c

signal_handler_travel.o: $(SRC_TRAVEL)/signal_handler.c $(INC_TRAVEL)/signal_handler.h
	$(CC) $(FLAGS) -o signal_handler_travel.o $(SRC_TRAVEL)/signal_handler.c

travel_stats.o: $(SRC_TRAVEL)/travel_stats.c $(INC_TRAVEL)/travel_stats.h
	$(CC) $(FLAGS) $(SRC_TRAVEL)/travel_stats.c





clean:
	rm -f $(OBJS_TRAVEL_MONITOR) $(OUT_TRAVEL_MONITOR) $(OBJS_MONITOR) $(OUT_MONITOR)



.PHONY:
	all