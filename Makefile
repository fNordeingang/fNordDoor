CXX = clang++
CC = clang
OBJS = door.o irc.o
ifndef NOTPI
LIBS = -lwiringPi -lircclient -lpthread
INCLUDE =	-IwiringPi/wiringPi
CXXFLAGS = -O3 -std=c++11 $(INCLUDE)
CFLAGS = -O3 -std=c++11 $(INCLUDE)
else
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
