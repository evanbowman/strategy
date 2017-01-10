CC = clang++
CFLAGS = -std=c++11 -O2
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cc=.o)

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS)

%.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o
