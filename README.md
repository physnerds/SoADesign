# SoADesign

## Introduction
This is a simple test suite designed to work in a local machine that supports ROOT with RNTuple. This was tested in M2 chip Macbook.
Detailed version of this test suite that includes HDF5 related tests and GPU offloading is here [https://github.com/physnerds/rntuple_test.git] But is only tested in Perlmutter.

## Dependencies

1. ROOT (Version 6.28 or larger)
2. HDF5 (Version 1.12 or larger)
3. cmake (Version 3.20 or larger)

All these packages are available either in Conda or homebrew which is the easy and recommended way of getting these packages. 

Here is the link to conda installation in MAC [https://docs.conda.io/projects/conda/en/latest/user-guide/install/macos.html].
Here is the link to brew installation in MAC [https://docs.brew.sh/Installation].

For Homebrew installation check for required installations in the webpage. 

Once you have one of those package managers, you can follow the instructions online (or google) to have those packages to build them locally. 

## Building Code

1. Download the Code from github.
```
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
