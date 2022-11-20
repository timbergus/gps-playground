CXX=clang++
STD=c++2b

TARGET=gps-playground

ROOT=src
APP=$(ROOT)/app
INCLUDE=$(ROOT)/include
BIN=bin

ifeq ($(OS),Windows_NT)
FMT_INCLUDE=???
else
FMT_INCLUDE=/opt/homebrew/Cellar/fmt/9.1.0/include
endif

ifeq ($(OS),Windows_NT)
IFLAGS=-I $(INCLUDE)
else
IFLAGS=-I $(INCLUDE) -I $(FMT_INCLUDE)
endif

CFLAGS=-std=$(STD) -Werror -Wall -Wextra

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
