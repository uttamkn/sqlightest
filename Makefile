.DEFAULT_GOAL := build

.PHONY: help configure build test run valgrind memcheck clean

help:
	@echo "Targets:"
	@echo "  make              Configure and build the project"
	@echo "  make test         Build and run tests"
	@echo "  make run ARGS='path/to/database.db \".tables\"'"
	@echo "  make valgrind ARGS='path/to/database.db \".tables\"'"
	@echo "  make memcheck     Run tests under Valgrind"
	@echo "  make clean        Remove generated build files"

configure:
	cmake --preset dev

build: configure
	cmake --build --preset dev --parallel

test: build
	ctest --preset dev

run: build
	./build/sqlite $(ARGS)

valgrind: build
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all \
		./build/sqlite $(ARGS)

memcheck: build
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all \
		./build/sqlite_tests

clean:
	cmake -E remove_directory build
