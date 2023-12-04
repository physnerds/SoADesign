# SoADesign
Doxygen pages are found here: "https://physnerds.github.io/SoADesign/"

## Introduction
This is a simple test suite designed to work in a local machine that supports ROOT with RNTuple. This was tested in M2 chip Macbook.
Detailed version of this test suite that includes HDF5 related tests and GPU offloading is here [https://github.com/physnerds/rntuple_test.git] But is only tested in Perlmutter.

## Dependencies
1. ROOT (preferably >=6.30 but at least greater than 6.28)
2. HDF5 (Version Greater than 1.14). Currently HDF5 is not needed but later modules will be added.
3. BOOST: Required for Pre-processing macro generation.
4. CMake (Version Greater than 3.20)

Almost all the dependencies are available via package manager like Conda or Brew. For ROOT with version 6.30 or greater, source installation is required.



## Building Code

1. Download the Code from github.
```
$ git clone https://github.com/physnerds/SoADesign.git
$ cd SoADesign
$ mkdir build
$ cmake ..
$ make -j16
```

Test codes:
Test Codes produce the data models similar to that of ProtoDUNE Trigger Data in the SoA format and write them in TTree or RNTuple
*********************************
```
./test_trigger_rntuple [1,2,3]
```
Writes (1), Reads (2) and compares with TTree ```created with test_trigger_tree ``` (3).
***********************************
```
./test_trigger_tree [1,2]
```
Writes SoA of vectors (1)in tree and Reads them back (2).
***********************************
```
./test_trigger_rntuple_arrSoa [1,2,3]
```
Writes SoA of arrays (1), Reads (2) and Compares with those written with TTree (3)
***********************************
```
./test_trigger_tree_arrSoa [1,2]
```
Writes SoA of arrays (1)in tree and Reads them back (2).
