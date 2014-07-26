#/bin/bash

cd guichan; make clean; rm -r CMakeFiles; rm CMakeCache.txt
cd ../box2d/; make clean; rm -r CMakeFiles; rm CMakeCache.txt
cd ../fann/; make clean;  rm -r CMakeFiles; rm CMakeCache.txt
make -C ../kmlocal clean
make -C ../json_spirit clean
make -C ../LuaJIT-2.0.3 clean

cd ../guichan; cmake  -DENABLE_OPENGL=OFF -DBUILD_GUICHAN_OPENGL_SHARED=OFF  .; make
cd ../box2d; cmake -DBOX2D_BUILD_SHARED=ON -DBOX2D_BUILD_STATIC=OFF -DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL=OFF .; make 
cd ../fann; cmake .; make
make -C ../kmlocal 
make -C ../json_spirit
make -C ../LuaJIT-2.0.3
