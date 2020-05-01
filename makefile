CC = gcc
CFLAGS = -I. -g
TARGET = p2
OBJS = main.o Options.o
.SUFFIXES: .c .o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f *.o $(TARGET)
