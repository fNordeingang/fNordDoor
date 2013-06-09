OBJS = door.o irc.o
UNAME := $(shell uname -m)
ifneq ($(UNAME), x86_64)
CXX = g++-4.7
CC = gcc-4.7
LIBS = -lwiringPi -lircclient -lpthread -lboost_system
INCLUDE = -IwiringPi/wiringPi
CXXFLAGS = -O3 -std=c++11 $(INCLUDE)
CFLAGS = -O3 -std=c++11 $(INCLUDE)
else
CXX = clang++
CC = clang
LIBS = -lircclient -lpthread -lboost_thread -lboost_system
INCLUDE =
CXXFLAGS = -g -std=c++11 $(INCLUDE)
CFLAGS = -g -std=c++11 $(INCLUDE) 
endif

CXXFLAGS += -D$(UNAME)

TARGET =	door

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
