# SoADesign

## Introduction
This is a simple test suite designed to work in a local machine that supports ROOT with RNTuple. This was tested in ANL HEPLOGIN Machine.


## Dependencies

1. ROOT (Version 6.30 or larger)
3. cmake (Version 3.20 or larger)

All these packages are available either in Conda or homebrew which is the easy and recommended way of getting these packages. If you are building this in LCRC, these anaconda is already available in LCRC machine. 

## Creating Conda Environment (One time Only)
In the LCRC (heplogin) machine:

```
$ module load anaconda3/2021.05
$ conda create -n rootenv python=3.8 root -c conda-forge
$ conda install cmake -c conda-forge
```
We have created a conda environment called rootenv, installed `ROOT` and `cmake`. 

## Loading the Conda Environment
Now load the conda environment. This is something you have to do each time you want to run this package.

```
$ module load anaconda3/2021.05
$ conda activate rootenv
```

## Building Code

Download the Code (lcrc branche) from github.
```
$ git clone -b feature/lcrc https://github.com/physnerds/SoADesign.git 
$ cd SoADesign
$ mkdir build
$ cmake ..
$ make -j16
```

## Test codes:
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
