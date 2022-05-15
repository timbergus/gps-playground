CXX=clang++
C_VERSION=c++20

TARGET=gps-playground

ROOT=src
APP=$(ROOT)/app
INCLUDE=$(ROOT)/include
BIN=bin

ifeq ($(OS),Windows_NT)
FMT_INCLUDE=???
else
FMT_INCLUDE=/opt/homebrew/Cellar/fmt/8.1.1_1/include
endif

ifeq ($(OS),Windows_NT)
IFLAGS=-I $(INCLUDE)
else
IFLAGS=-I $(INCLUDE) -I $(FMT_INCLUDE)
endif

CFLAGS=-std=$(C_VERSION) -Werror -Wall -Wextra

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
