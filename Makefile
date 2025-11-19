CC = gcc
CFLAGS = -O2 -Wall -Wextra -march=native
LDFLAGS = -lpthread
SRCS = main.c matmul.c utils.c
OBJS = $(SRCS:.c=.o)
TARGET = matrix_mult

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) results.csv
