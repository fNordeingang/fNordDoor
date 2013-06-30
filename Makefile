OBJS = door.o doorlock.o
UNAME := $(shell uname -m)
ifneq ($(UNAME), x86_64)
CXX = g++-4.7
CC = gcc-4.7
LIBS = -lwiringPi -lboost_program_options
INCLUDE = -IwiringPi/wiringPi
CXXFLAGS = -O3 -std=c++11 $(INCLUDE)
CFLAGS = -O3 -std=c++11 $(INCLUDE)
else
CXX = clang++
CC = clang
LIBS = -lboost_program_options
INCLUDE =
CXXFLAGS = -O3 -std=c++11 $(INCLUDE)
CFLAGS = -O3 -std=c++11 $(INCLUDE) 
endif

CXXFLAGS += -D$(UNAME)

TARGET =	doorlock

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
