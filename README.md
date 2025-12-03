# pine
Physics 3D engine

## Requirements
1. OS: Tested only on Debian
2. Global Debian libraries: `sudo apt-get install freeglut3-dev libglew-dev libglm-dev`
3. Download these libraries to `libs` folder:
    - [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)

## Installation

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```
