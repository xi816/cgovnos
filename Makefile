CC=gcc
CFLAGS=-lcurses
TARGET=govnos
SRC=src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
    $(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
    rm -f $(TARGET)
