CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = file2_1.c file2_2.c
OBJS = $(SRCS:.c=.o)
LIB = libdir2.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIB)
