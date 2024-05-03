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

For using by Hyperscan also need boost
<pre>
sudo apt install libboost-all-dev
</pre>

For using by Google RE2 also need install `abseil-cpp` from https://github.com/abseil/abseil-cpp
(see README for instructions)
<pre>
git clone https://github.com/abseil/abseil-cpp abseil-cpp && cd abseil-cpp && mkdir -p bin && cd bin && cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_POSITION_INDEPENDENT_CODE=ON .. && cmake --build . && sudo cmake --install .
</pre>

## Build

<pre>
make
</pre>

## Run tests

<pre>
make check
</pre>

## Show results after check

All iterations of checks placed in separated files at directory: `results`

<pre>
ls -1 results
</pre>

## Cleaning

<pre>
make clean
</pre>

Also deleting all loaded git repository.