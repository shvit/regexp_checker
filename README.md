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
| stdlib-compiled | C++ standard library with pre compiled rules | - | 
| tiny-regex-c | C regexp code | https://github.com/kokke/tiny-regex-c |
| ximtech | C library | https://github.com/ximtech/Regex |

# Build

<pre>
make
</pre>

# Run tests

<pre>
make check
</pre>

Show results after check

<pre>
ls -1 results
</pre>

All iterations of checks placed in seperetaed files at directory: `results`
