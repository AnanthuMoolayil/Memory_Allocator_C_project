CC = clang

CFLAGS = -std=c17 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-Wshadow \
	-Wconversion \
	-Wsign-conversion \
	-Wformat=2 \
	-g \
	-fsanitize=address,undefined

SRC_DIR = 1_source_files

SRC = $(wildcard $(SRC_DIR)/*.c)	

TARGET = mem

all:
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)