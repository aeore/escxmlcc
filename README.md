escxmlcc
========

FSM Generator for C++ with included graphical editor

##### License

This Source Code Form is subject to the terms of the GPLv3
The generated code is covered by MIT License

##### Build Instructions for Linux
###### Dependencies

- A C++11 enabled compiler is needed
- escxmlcc uses CMake as buildsystem.
- escxmlcc uses Boost >= 1.45.0

###### Compilation

For compilation call:

```bash
mkdir build
cd build
cmake ..
make
```

To compile with unit tests:
```bash
cmake -DWITH_TESTS=TRUE ..
make
```

To specify a installation directory (like `--prefix=` if you're used to autotools) call cmake like:
```bash
cmake -DCMAKE_INSTALL_PREFIX:PATH=$YOUR_PATH ..
make
make install
```
