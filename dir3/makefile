CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = file3_1.c file3_2.c
OBJS = $(SRCS:.c=.o)
LIB = libdir3.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)
