CC = gcc
CFLAGS = -Wall -Wextra
TARGET = gic-p
SOURCE = main.c

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin

clean:
	rm -f $(TARGET)

.PHONY: install clean
