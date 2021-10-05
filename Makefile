CXX=clang++

TARGET=gps-playground

ROOT=src
APP=$(ROOT)/app
INCLUDE=${ROOT}/include
BIN=bin

ifeq ($(OS),Windows_NT)
	IFLAGS=-I $(INCLUDE)
else
	IFLAGS=-I $(INCLUDE) -I /usr/local/include
	LFLAGS=-L /usr/local/lib -lfmt
endif

CFLAGS=-std=c++2a -Werror -Wall -Wextra

OBJS=$(BIN)/main.o

$(BIN)/%.o: $(INCLUDE)/%.cpp
ifeq ($(OS),Windows_NT)
	@mkdir $(BIN)
else
	@mkdir -p $(BIN)
endif
	$(CXX) $(CFLAGS) $(IFLAGS) -c -MD $< -o $@

$(BIN)/%.o: $(APP)/%.cpp
ifeq ($(OS),Windows_NT)
	@mkdir $(BIN)
else
	@mkdir -p $(BIN)
endif
	$(CXX) $(CFLAGS) $(IFLAGS) -c -MD $< -o $@

$(TARGET): $(OBJS)
	@$(CXX) $(CFLAGS) $(IFLAGS) $(LFLAGS) $(OBJS) -o $(BIN)/$(TARGET)

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
