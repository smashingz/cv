CC              := g++
CFLAGS          := -I/usr/include/opencv -I/usr/include/opencv2 -L/usr/local/lib -Wall -g
OBJECTS         := 
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui

.PHONY: all clean

all: test

test: 
	$(CC) $(CFLAGS) -o test.out main.c $(LIBRARIES)
        
clean:
	rm -f *.o *.out
