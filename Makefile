OBJS = door.o irc.o
ifndef NOTPI
CXX = g++-4.7
CC = gcc-4.7
LIBS = -lwiringPi -lircclient -lpthread
INCLUDE =	-IwiringPi/wiringPi
CXXFLAGS = -O3 -std=c++11 $(INCLUDE)
CFLAGS = -O3 -std=c++11 $(INCLUDE)
else
CXX = g++
CC = gcc
LIBS = -lircclient -lpthread
INCLUDE =
CXXFLAGS = -O3 -std=c++11 $(INCLUDE) -DNOTPI
CFLAGS = -O3 -std=c++11 $(INCLUDE) -DNOTPI 
endif


TARGET =	door

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
