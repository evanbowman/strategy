CC = clang++
CFLAGS = -std=c++11 -O2
LDFLAGS = -framework sfml-graphics -framework sfml-window -framework sfml-system
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o
