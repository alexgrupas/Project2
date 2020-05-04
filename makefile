CC = gcc
CFLAGS = -I. -g
TARGET1 = p2
TARGET2 = user
OBJS1 = main.o Options.o
OBJS2 = user.o
.SUFFIXES: .c .o

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1)
	$(CC) $(CFLAGS) -o $@ $(OBJS1)

$(TARGET2): $(OBJS2)
	$(CC) $(CFLAGS) -o $@ $(OBJS2)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -f *.o $(TARGET1) $(TARGET2)
