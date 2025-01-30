# ECL Web Assembly Examples

Examples of doing various lisp things in ECL/Emscripten/webassembly. This is
very much a work in progress with various levels of experimentation as we
figure out how to do things in this new-ish environment.

These examples assume you have mostly followed the ECL Install instructions for use
with webassembly and Emscripten.

Most of these examples include a `help.doc` file, this was necessary to work around some
issues with asdf.lisp loading. Additionally, asdf.lisp was copied over from the ECL source.
Using the other ECL compile options for including asdf are currently causing errors (at least
for me).

Addtionally, so these are turn key examples I'm including `libecl.a`, `libeclgc.a`, and
`libeclgmp.a`. Ideally you would bring these in from your own webassembly target build of ECL.

## OpenGL / SDL

## Quicklisp

This example shows copying in your quicklisp directory wholesale, bundling it, loading asdf and
quicklisp, and then quickloading a simple library (in this case Alexandra.)

```sh
cd quicklisp-asdf-example
./make.sh
python3 -m http.server # or other local http server
# load http://localhost:8000/hello.html
```

