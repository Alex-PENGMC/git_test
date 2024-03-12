CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = file1_1.c file1_2.c
OBJS = $(SRCS:.c=.o)
LIB = libdir1.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)
