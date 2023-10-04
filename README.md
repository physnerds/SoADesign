# SoADesign

## Introduction
This is a simple test suite designed to work in a local machine that supports ROOT with RNTuple. This was tested in M2 chip Macbook.
Detailed version of this test suite that includes HDF5 related tests and GPU offloading is here [https://github.com/physnerds/rntuple_test.git] But is only tested in Perlmutter.
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
test_trigger_rntuple [1,2,3]
```
Writes (1), Reads (2) and compares with TTree ```created with test_trigger_tree ``` (3).
***********************************
```
test_trigger_tree [1,2]
```
Writes SoA of vectors (1)in tree and Reads them back (2).
***********************************
```
test_trigger_rntuple_arrSoa [1,2,3]
```
Writes SoA of arrays (1), Reads (2) and Compares with those written with TTree (3)
***********************************
```
test_trigger_tree_arrSoa [1,2]
```
Writes SoA of arrays (1)in tree and Reads them back (2).
