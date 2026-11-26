# jpacUtils

Methods with common and otherwise useful methods for JPAC analyses. 

Originally these were internal modules of [jpacPhoto](https://github.com/dwinney/jpacPhoto) and [iterateKT](https://github.com/dwinney/iterateKT) but better place them as a unified external library to avoid copy and pasting into each new project.

## INSTALLATION
Simply clone normally and install with cmake. Compilation requires only ROOT (tested with versions 6.17 and 6.24).
```bash
cd jpacUtilities
mkdir build && cd build
cmake ..
cmake --build . --target install
```

To make the library searchable by cmake in other projects define an environment variable `$JPACUTILITIES` pointing to the top-level installation directiory.

# References
+ [1] [JPAC Website](https://www.jpac-physics.org/)

<p align="center">
  <img width="300" src="./doc/JPAClogo.png">
</p>