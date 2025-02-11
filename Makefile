CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2
TARGET = proccloner
SRC = proccloner.c writer.c reader.c load_linux.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)