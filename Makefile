CC=gcc  #compiler
CFLAGS=-Wall -Wextra -I.
LIBS = -lm

TARGET=perceptron #target file name

build-run: *.c #*.h
	$(CC) $(CFLAGS) $(LIBS) main.c -o $(TARGET) && ./$(TARGET)


clean:
	rm $(TARGET) #*.o
