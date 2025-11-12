CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDFLAGS :=

# Add your four source files here
SRCS := main.c st_ana.c next_state.c task04.c task12.c
OBJS := $(SRCS:.c=.o)

TARGET := tele

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)