BUILD=build
PROJECT=gps-playground

init:
	cmake -B $(BUILD)

project:
	cmake --build $(BUILD)

start:
	./$(BUILD)/$(PROJECT)

test:
	ctest --test-dir $(BUILD)

clean:
	rm -rf $(BUILD)
