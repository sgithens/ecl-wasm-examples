#!/bin/bash

function build () {
    echo "Building..."
    emcc -O2 \
         -sASYNCIFY -sASYNCIFY_STACK_SIZE=1048576 \
         -sSTACK_SIZE=1048576 \
         -s TOTAL_MEMORY=197656576 \
         -sERROR_ON_UNDEFINED_SYMBOLS=0 \
         -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,stringToNewUTF8,UTF8ToString \
         -sUSE_GLFW=3 \
          --embed-file "quicklisp" \
         --embed-file "asdf.lisp" --embed-file "help.doc"\
         -I/home/sgithens/code/ecl/ecl-emscripten/ \
         ../libecl{,gc,gmp}.a hello.c -I./ -o hello.html
}

build

