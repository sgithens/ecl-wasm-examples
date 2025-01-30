#!/bin/bash

# -sINITIAL_MEMORY=67108864

function build-sdl-triangle () {
    echo "Building..."
    emcc -O2 \
         -sUSE_SDL=2 -sFULL_ES2=1 \
         -sASYNCIFY -sASYNCIFY_STACK_SIZE=1048576 \
         -sSTACK_SIZE=1048576 \
         -s TOTAL_MEMORY=197656576 \
         -sERROR_ON_UNDEFINED_SYMBOLS=0 \
         -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,stringToNewUTF8,UTF8ToString \
         --embed-file "ecl-opengl.lisp" \
         --embed-file "hello-lisp.lisp" \
         --embed-file "help.doc"\
         -I/home/sgithens/code/ecl/ecl-emscripten/ \
         ../libecl{,gc,gmp}.a main.cpp -I./ -o main-triangle.html
}

# build
build-sdl-triangle

# -sASYNCIFY_IMPORTS=[cl_eval,js_test,js_read] \
# --embed-file "boxerdata"
#          -sUSE_GLFW=3 \  --embed-file "asdf.lisp"
#
