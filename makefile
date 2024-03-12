CC = gcc
CFLAGS = -Wall -Wextra -g

SRCDIRS = dir1 dir2 dir3
LIBS = $(foreach dir,$(SRCDIRS),$(dir)/lib$(notdir $(dir)).a)
EXEC = alex

all: $(EXEC)

$(EXEC): $(LIBS) main.o
	$(CC) $(CFLAGS) -o $@ main.o $(LIBS)

$(LIBS): 
	$(MAKE) -C $(@D)

clean:
	rm -f $(EXEC) main.o
	$(foreach dir,$(SRCDIRS),$(MAKE) -C $(dir) clean;)
