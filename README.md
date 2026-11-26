# jpacUtils

Collection of common and otherwise useful methods for JPAC analyses. 

These fall into a few categories:
- [math](./jpacUtils/math.hpp) - Mathematical functions such as angular polynomials, complex numerical derivatives, and useful global constants.
- [plotter](./jpacUtils/plotter.hpp) - Make plots easily within ROOT with JPAC colors and style.
- [data](./jpacUtils/data.hpp) - Simple interface for working with data sets in plots and fitters. 
- [print](./jpacUtils/print.hpp) - Methods to print messages to the commandline without having to write all boilerplate of C++.
- [timer](./jpacUtils/timer.hpp) - Class that provides an easy way to time processes in a human readable way. 


Originally these were internal modules of [jpacPhoto](https://github.com/dwinney/jpacPhoto) and [iterateKT](https://github.com/dwinney/iterateKT) but better place them as a unified external library to avoid copy and pasting into each new project.

## INSTALLATION
Simply clone normally and install with cmake. Compilation requires only ROOT (tested with versions 6.17 and 6.24).
```bash
cd jpacUtils
mkdir build && cd build
cmake ..
cmake --build . --target install
```

To make the library searchable by cmake in other projects define an environment variable `$JPACUTILS` pointing to the top-level installation directiory.

# References
+ [1] [JPAC Website](https://www.jpac-physics.org/)

<p align="center">
  <img width="300" src="./doc/JPAClogo.png">
</p>