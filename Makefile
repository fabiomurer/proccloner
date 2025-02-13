CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -static -Wl,-Ttext-segment,0x70000000
TARGET = proccloner
SRC = proccloner.c writer.c reader.c load_linux.c utility.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)