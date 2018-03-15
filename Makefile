CC              := g++
CFLAGS          := -I/usr/include/opencv -L/usr/local/lib
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui

.PHONY: all clean

all: test

test: 
	$(CC) $(CFLAGS) -o test.out main.cpp $(LIBRARIES)
        
clean:
	rm -f *.o *.out
