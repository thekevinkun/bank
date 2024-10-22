# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CXX = g++
CFLAGS  = -g -std=c++11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
CPPFLAGS = -lsqlite3 -lcrypto

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: main

# To create the executable file count we need the object files
# all *.o files
#
main: account.o database.o encrypt.o main.o 
	$(CXX) $(CFLAGS) account.o database.o encrypt.o main.o -o main $(CPPFLAGS)

# To create the object file account.o, we need the source
# Including the header in source files
#
account.o: account.cpp account.h database.h encrypt.h
	$(CXX) $(CFLAGS) -c account.cpp

# To create the object file database.o, we need the source files
# Including the header in source files
#
database.o: database.cpp database.h
	$(CXX) $(CFLAGS) -c database.cpp

# To create the object file encrypt.o, we need the source files
# Including the header in source files
#
encrypt.o: encrypt.cpp encrypt.h
	$(CXX) $(CFLAGS) -c encrypt.cpp

# To create the object file main.o, we need the source files
# Including the header in source files
#
main.o: main.cpp database.h account.h
	$(CXX) $(CFLAGS) -c main.cpp

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) main *.o *~