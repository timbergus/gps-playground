BUILD=build

init:
	conan install . -of $(BUILD) --build=fmt/10.1.1 && cmake -B $(BUILD)

build_project:
	cmake --build $(BUILD)

start:
	./$(BUILD)/gps_playground

clean:
	rm -rf $(BUILD)
