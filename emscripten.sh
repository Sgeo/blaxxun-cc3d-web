emcc -gsource-map --source-map-base "http://localhost:8000/" -s EXIT_RUNTIME=1 -s ALLOW_MEMORY_GROWTH -s SAFE_HEAP -s FULL_ES2=1 -Igl4es/include -s gl4es/lib/libGL.a GLU/.libs/libGLU.a  -IlibCC3D libCC3d/stdafx.cpp libCC3d/cc3dglut.cpp libCC3d/gnavigation.cpp -s LLD_REPORT_UNDEFINED -Isrc/core src/core/*.cpp -Isrc/vrml/include src/vrml/src/*.cpp -Isrc/core/gl src/core/gl/*.cpp -Isrc/core/collision/rapid src/core/collision/rapid/*.cpp -Isrc/hgnt/ src/hgnt/image.c src/hgnt/png_read.c src/hgnt/jpegread.c src/hgnt/targaio.c -s USE_LIBJPEG=1 -s USE_LIBPNG -s USE_ZLIB -Isrc/vrmlscript src/vrmlscript/*.cpp -D_OGL=1 -Wno-deprecated-register -Wno-extra-tokens --preload-file worlds@/ --pre-js pre.js -o blaxxun-cc3d-web.html