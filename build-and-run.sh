echo

mkdir -p ./out

# lGL = openGL, lglfw = GLFW
g++ ./src/main.cpp -o ./out/main -lglfw -lGL

if [ $? -eq 0 ] 
then 
  echo "BUILD SUCCESSFUL - Executable created at ./out/main\n" 
  ./out/main
else 
  echo "BUILD FAILED - exit code: $?"
fi

echo