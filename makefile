CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c file1.c file2.c
OBJS = $(SRCS:.c=.o)
EXEC = alex

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
