# Sqlite implementation in C
## Development

You need a C23 compiler, CMake 3.21 or newer, and Make. Valgrind is only
required for the memory-checking commands.

```sh
make help
make                  # Configure and build
make test             # Build and run the test suite
make run ARGS='path/to/database.db ".tables"'
make valgrind ARGS='path/to/database.db ".tables"'
make memcheck         # Run the tests under Valgrind
make clean
```

`build.sh` remains as a compatibility wrapper around Make, so `./build.sh`
and `./build.sh test` work too. It no longer runs the executable or Valgrind
as an implicit side effect of building.

The Makefile is a convenience interface; CMake remains the build system. The
equivalent direct commands are:

```sh
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

## Notes
![image](https://github.com/user-attachments/assets/eadeeec3-b8a5-42a2-abb4-f974414c190f)
![image](https://github.com/user-attachments/assets/cd0887f1-834a-4537-a4e1-550632bc2f1c)

## Storage engine
![image](https://github.com/user-attachments/assets/0a8472a1-e020-4a4b-a323-d459a42dd561)
