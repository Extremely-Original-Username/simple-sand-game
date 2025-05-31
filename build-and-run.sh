echo

mkdir -p ./out

# lGL = openGL, lglfw = GLFW
g++ ./src/main.cpp -o ./out/main -lglfw -lGL

./out/main

echo