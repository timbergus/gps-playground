# GPS Playground

Simple playground to experiment with GPS data.

It has a main program that runs the example, and a GPS module that deals with the GPS data.

- `app/main.cpp`
- `include/gps.h`

This module uses different modules to parse the GPS data, according to the NMEA standards, and a utils library for minor tasks.

- `include/rmc.h`
- `include/utils.h`

I'm building this project to use it to parse GPS data from a GPS unit attached to a BeagleBone Blue in real time, and learn C++ along the way.

## Clean

```bash
make clean
```

## Build & Run

```bash
make && make start
```

## Data

It uses randomly generated data stored in `src/data/samples.txt`. It's a file with a sample per line.

## Libraries

To use [`format`](https://github.com/fmtlib/fmt), you need to install it with `brew` (maybe cloning it on Windows, or using something like [Conan](https://conan.io/)).
