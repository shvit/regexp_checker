# Testing suite for some regexp library

Developed and tested on Ubuntu 20.04.
Used C++17 (why? I like it).

Used only library/application with source code.
For all externall library will clone repository and build checker app.

Regexp in directory: `test/regexp`

Data chunks in directory: `test/data`

## Included library for testing

| Alias | Description | URL of repository |
| --- | --- | --- |
| stdlib | C++ standard library | - | 
| tiny-regex-c | C regexp code | https://github.com/kokke/tiny-regex-c |
| ximtech | C library | https://github.com/ximtech/Regex |
| boost | Regex from C++ library | https://github.com/boostorg/regex.git |
| hyperscan | Intel regex library | https://github.com/intel/hyperscan |
| google-re2 | Google C++ regex library | https://github.com/google/re2 |

# Build and run

## Installing dependencies

<pre>
sudo apt install build-essential cmake ragel git
</pre>

Also need install `abseil-cpp` from https://github.com/abseil/abseil-cpp
(see README for instructions)

## Build

<pre>
make
</pre>

## Run tests

<pre>
make check
</pre>

## Show results after check

<pre>
ls -1 results
</pre>

All iterations of checks placed in seperetaed files at directory: `results`

## Cleaning

<pre>
make clean
</pre>
