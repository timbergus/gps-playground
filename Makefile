ifeq ($(OS),Windows_NT) # is Windows_NT on XP, 2000, 7, Vista, 10...
    CC=clang++
	IFLAGS=-I $(INCLUDE)
	BIN=src\bin
else
    CC=g++-11
	IFLAGS=-I $(INCLUDE) -I /usr/local/include
	LFLAGS=-L /usr/local/lib -lfmt
	BIN=src/bin
endif

CFLAGS=-std=c++20 -Werror -Wall -Wextra

TARGET=gps-playground
ROOT=src/app
INCLUDE=src/include

OBJS=$(BIN)/main.o

$(BIN)/%.o: $(INCLUDE)/%.cpp
ifeq ($(OS),Windows_NT)
	@mkdir src\bin
else
	@mkdir -p $(BIN)
endif
	$(CC) $(CFLAGS) $(IFLAGS) -c -MD $< -o $@

$(BIN)/%.o: $(ROOT)/%.cpp
ifeq ($(OS),Windows_NT)
	@mkdir $(BIN)
else
	@mkdir -p $(BIN)
endif
	$(CC) $(CFLAGS) $(IFLAGS) -c -MD $< -o $@

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(OBJS) -o $(BIN)/$(TARGET)

-include $(BIN)/*.d

start:
	@$(BIN)/$(TARGET)

.PHONY: clean

clean:
ifeq ($(OS),Windows_NT)
	@rmdir /S/Q $(BIN)
else
	@rm -r $(BIN)
endif
