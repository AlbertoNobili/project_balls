#---------------------------------------------
# Target file to be compiled by default
#---------------------------------------------
MAIN = balls
#---------------------------------------------
# CC is the compiler to be used
#---------------------------------------------
CC = gcc
#---------------------------------------------
# CFLAGS are the options passed to the compiler
#---------------------------------------------
CFLAGS = -Wall 
#---------------------------------------------
# OBJS are the object files to be linked
#---------------------------------------------
OBJ1 = ptask
OBJ2 = kbfunc
OBJS = $(MAIN).o $(OBJ1).o $(OBJ2).o
#---------------------------------------------
# LIBS are the external libraries to be used
#--------------------------------------------
LIBS = -pthread -lrt -lm `allegro-config --libs`
#---------------------------------------------
# Dependencies
#--------------------------------------------
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

$(MAIN).o: $(MAIN).c constant.h
	$(CC) $(CFLAGS) -c $(MAIN).c

$(OBJ1).o: $(OBJ1).c 
	$(CC) $(CFLAGS) -c $(OBJ1).c

$(OBJ2).o: $(OBJ2).c
	$(CC) $(CFLAGS) -c $(OBJ2).c
	

#-------------------------------------------
# Command that can be specified inline: make clean
#-------------------------------------------
clean: 
	rm -rf *.o $(MAIN)
