CXX = g++
CC = gcc
OBJS = door.o
ifndef NOTPI
LIBS = -lwiringPi
INCLUDE =	-IwiringPi/wiringPi
CXXFLAGS = -O3 -std=c++11 $(INCLUDE)
CFLAGS = -O3 -std=gnu11 $(INCLUDE)
else
LIBS = 
INCLUDE =
CXXFLAGS = -O3 -std=c++11 $(INCLUDE) -DNOTPI
CFLAGS = -O3 -std=gnu11 $(INCLUDE) -DNOTPI 
endif


TARGET =	door

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
