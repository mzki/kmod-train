# independent-module

This directory contains a kernel module, which has 
dependency on other kernel modules. 
The dependent kernel module is in `dep_timer` subdirectory.

The goal is building the kernel module depending on the others and unit testing it.

# How to

To build kernel module, just type 
```sh
$ make
```

To register the built module to the kernel:
```sh
$ make install
```

To test this module:
```sh
$ make test
```

Testing module is in `testing` subdirectory.
