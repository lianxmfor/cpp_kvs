.PHONY: build test

build:
	@mkdir -p build
	cd build && cmake .. && make && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 .. && mv compile_commands.json ..

test: build
	@cd build && ctest --output-on-failure

format:
	@find src/ -iname '*.h' -o  -iname '*cpp' | xargs clang-format -style Webkit -i

clean:
	@rm -rf build

