# debugfs

This directory contains a kernel module, which uses 
debugfs feature.

The goal is to use debugfs feature to export
internal state of the kernel module to the user space.

# How to

```sh
# install kernel modules
cd mod1 
make install
cd ../mod2 
make install

# show debugfs log messages
sudo cat /sys/kernel/debug/module1-log
sudo cat /sys/kernel/debug/module2-log

# uninstall kernel modules
make uninstall
cd ../mod1
make uninstall
```
