CC=g++-11
CFLAGS=-std=c++20 -Werror -Wall -Wextra
IFLAGS=-I $(INCLUDE) -I/usr/local/include
LFLAGS=-L/usr/local/lib -lfmt

TARGET=gps-playground
ROOT=src/app
INCLUDE=src/include
BIN=src/bin

OBJS=$(BIN)/main.o

$(BIN)/%.o: $(INCLUDE)/%.cpp
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(IFLAGS) -c -MD $< -o $@

$(BIN)/%.o: $(ROOT)/%.cpp
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(IFLAGS) -c -MD $< -o $@

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(OBJS) -o $(BIN)/$(TARGET)

-include $(BIN)/*.d

start:
	@$(BIN)/$(TARGET)

.PHONY: clean

clean:
	rm -r $(BIN)
