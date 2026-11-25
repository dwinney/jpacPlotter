# jpacPlotter

Methods to easily make plots within ROOT. Originally this was an internal module of [jpacPhoto](https://github.com/dwinney/jpacPhoto) but now justifies being its own module to be added into other projects as well. 


## INSTALLATION
Simply clone normally and install with cmake. Compilation requires only ROOT (tested with versions 6.17 and 6.24).
```bash
cd jpacPhoto
mkdir build && cd build
cmake ..
cmake --build . --target install
```

To make the library searchable by cmake in other projects define an environment variable `$JPACPLOTTER` pointing to the top-level installation directiory.