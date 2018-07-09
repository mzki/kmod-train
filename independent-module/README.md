# independent-module

This directory contains a independent kernel module, which has 
no dependency on other kernel modules. 

The goal is building simple kernel module and unit testing it.

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
