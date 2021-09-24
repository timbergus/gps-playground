CC=gcc-11
CFLAGS=-std=c++20 -Werror -Wall -Wextra -I $(LIBRARY)/include

TARGET=gps-playground
ROOT=src/app
LIBRARY=src/lib
BIN=src/bin

OBJS=	$(BIN)/main.o		\
		$(BIN)/gps.o

$(BIN)/%.o: $(LIBRARY)/%.cpp
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c -MD $< -o $@

$(BIN)/%.o: $(ROOT)/%.cpp
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -c -MD $< -o $@

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(BIN)/$(TARGET)

-include $(BIN)/*.d

start:
	$(BIN)/$(TARGET)

.PHONY: clean

clean:
	rm -r $(BIN)
