emcc -s FULL_ES2=1 -Igl4es/include GLU/.libs/libGLU.a gl4es/lib/libGL.a -IlibCC3D libCC3d/stdafx.cpp libCC3d/cc3dglut.cpp -s LLD_REPORT_UNDEFINED -Isrc/core src/core/*.cpp -Isrc/vrml/include src/vrml/src/*.cpp -Isrc/core/gl src/core/gl/*.cpp -Isrc/core/collision/rapid src/core/collision/rapid/*.cpp -Isrc/jpeghgnt/ src/jpeghgnt/image.c src/jpeghgnt/png_read.c src/jpeghgnt/jpegread.c src/jpeghgnt/targaio.c -s USE_LIBJPEG=1 -s USE_LIBPNG -s USE_ZLIB -Isrc/vrmlscript src/vrmlscript/*.cpp -D_OGL=1 -Wno-deprecated-register -Wno-extra-tokens 