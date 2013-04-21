CXX = g++
CC = gcc
OBJS = door.o
LIBS = -lwiringPi
INCLUDE =	-IwiringPi/wiringPi
CXXFLAGS = -O3 -std=c++11 $(INCLUDE)
CFLAGS = -O3 -std=c11 $(INCLUDE)

TARGET =	door

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
