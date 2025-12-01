# Programowanie grafiki 3D w OpenGL

semestr zimowy 2023/2024

## Instalacja bibliotek (Linux)

```
sudo apt-get install freeglut3-dev
sudo apt-get install libglew-dev
sudo apt-get install libglm-dev
```

### Uruchamianie

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Tekstury

Poniższe tekstury zostały pobrane ze strony <https://www.freepik.com/>:

- **chessboard.jpg** - Designed by aranjuezmedina
- **grass.jpg** - Designed by awesomecontent

Tesktura skuybox'a została pobrana ze strony <https://www.keithlantz.net/2011/10/rendering-a-skybox-using-a-cube-map-with-opengl-and-glsl/>

**Uwaga**: Niektóre z użytych tekstur zostały przeskalowane lub zostało zmienione rozrzeszenie plików
