[![Build and Test on windows, ubuntu, macos](https://github.com/nachooya/ISOBMFF_Reader/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/nachooya/ISOBMFF_Reader/actions/workflows/cmake-multi-platform.yml)

# ISOBMFF Reader

This is a simple ISOBMFF reader written in C++. It reads the ISOBMFF file and prints the contents of the file to the console.

## Usage

To use the ISOBMFF reader, you need to provide the path to the ISOBMFF file as an argument when running the program. For example:

```
./Mp4Reader tests/assets/text0.mp4
```

This will read the file `tests/assets/text0.mp4` and print its contents to the console.

Image extraction is also supported when built with pugixml. To extract images from the ISOBMFF file, you need to provide the path to the ISOBMFF file as an argument when running the program. For example:

```
./Mp4Reader --extract-images tests/assets/text0.mp4
```

This will read the file `tests/assets/text0.mp4` and extract images from the file and save them to the current directory.

## Features

The ISOBMFF reader supports the following features:

- Reading the contents of the ISOBMFF file and printing them to the console.
- Extracting images from the ISOBMFF file and saving them to a directory.

## Building

To build the ISOBMFF reader, you need to have the following dependencies installed:

- A C++ compiler (e.g., GCC or Clang)
- CMake (version 3.14 or later)

Once you have the dependencies installed, you can build the ISOBMFF reader by running the following commands:

```
mkdir build
cd build
cmake ..
make
```

To disable pugixml support (which is enabled by default and implements image extraction), you can use the following command:

```
cmake -DENABLE_PUGIXML=OFF ..
``` 

This will create a `Mp4Reader` executable in the `build` directory.

## Running the Tests

To run the tests, you need to have the following dependencies installed:

- A C++ compiler (e.g., GCC or Clang)
- CMake (version 3.14 or later)
- CppUTest (version 4.0 or later) automatically installed when you build the project with CMake

Once you have the dependencies installed, you can run the tests by running the following commands:

```
mkdir build_test
cd build_test
cmake -DENABLE_PUGIXML=ON -DCMAKE_BUILD_TYPE=Debug ..
make Mp4ReaderTests
./Mp4ReaderTests
```

This will run the tests and display the results.

# Dev guidelines
- Explore already available solutions in public software repositories (ie: github).
- Try to minimize external dependencies (reduce solution complexity, more knowledge about the solution, less incertitude.
- Configure some test suit to check functionality and performance.
- Abstract the problems
- Check it works on different OSs
- Ask AI for solutions

# TODO

1. Add linter configuration (ie: clang-tidy)
2. Define code style by too (ie: clang-format)
3. Code coverage
