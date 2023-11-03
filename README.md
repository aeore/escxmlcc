escxmlcc
========

C++ console code generator and editor with GUI for state machines.
This project is on the proof-of-concept stage.

##### License

This Source Code Form is subject to the terms of the GPLv3.
The generated code is covered by MIT License.

##### Build Instructions for Linux
###### Dependencies

- A C++11 enabled compiler is needed
- escxmlcc uses CMake as buildsystem
- escxmlcc uses Boost >= 1.45.0

###### Compilation

For compilation call:

```bash
mkdir build
cd build
cmake ..
make
optional: sudo make install
```

To compile with unit tests:
```bash
mkdir build
cd build
cmake -DWITH_TESTS=TRUE ..
make
```

To specify a installation directory (like `--prefix=` if you're used to autotools) call cmake like:
```bash
cmake -DCMAKE_INSTALL_PREFIX:PATH=$YOUR_PATH ..
make
make install
```
