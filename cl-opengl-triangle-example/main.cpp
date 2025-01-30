#include <stdio.h>
#include <ecl/ecl.h>

#include <exception>
#include <functional>

#define GL_GLEXT_PROTOTYPES 1

#include <emscripten.h>
#include <SDL/SDL.h>
#include <SDL_opengles2.h>

#include <iostream>
using namespace std;

std::function<void()> loop;
void main_loop() { loop(); }

cl_object lisp_gl_clear_color(cl_object r, cl_object g, cl_object b, cl_object a) {
    glClearColor(ecl_single_float(r), ecl_single_float(g), ecl_single_float(b), ecl_single_float(a));
    return ECL_NIL;
}

cl_object lisp_gl_clear() {
    glClear(GL_COLOR_BUFFER_BIT);
    return ECL_NIL;
}

cl_object lisp_gl_draw_arrays(cl_object mode, cl_object first, cl_object count) {
    glDrawArrays(ecl_fixnum(mode), ecl_fixnum(first), ecl_fixnum(count));
    return ECL_NIL;
}

cl_object lisp_gl_gen_vertex_array() {
    GLuint vao;
    glGenVertexArraysOES(1, &vao);
    return ecl_make_fixnum(vao);
}

cl_object lisp_gl_bind_vertex_array(cl_object vao) {
    glBindVertexArrayOES(ecl_fixnum(vao));
    return ECL_NIL;
}

cl_object lisp_gl_gen_buffer() {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    return ecl_make_fixnum(vbo);
}

cl_object lisp_gl_bind_buffer(cl_object mode, cl_object vbo) {
    glBindBuffer(ecl_fixnum(mode), ecl_fixnum(vbo));
    return ECL_NIL;
}

cl_object lisp_gl_buffer_data(cl_object buffer_mode, cl_object size, cl_object vertices, cl_object draw_mode) {
    glBufferData(ecl_fixnum(buffer_mode), ecl_fixnum(size),
        (void*) ecl_foreign_data_pointer_safe(vertices), ecl_fixnum(draw_mode));
    return ECL_NIL;
}

cl_object lisp_gl_create_shader(cl_object mode) {
    GLuint shader;
    shader = glCreateShader(ecl_fixnum(mode));
    return ecl_make_fixnum(shader);
}

cl_object lisp_gl_shader_source(cl_object shader, cl_object source) {
    char * ptr = ecl_base_string_pointer_safe (ecl_null_terminated_base_string(source));
    glShaderSource(ecl_fixnum(shader), 1, (const char *const *) &ptr, NULL);
    return ECL_NIL;
}

cl_object lisp_gl_compile_shader(cl_object shader) {
    glCompileShader(ecl_fixnum(shader));
    return ECL_NIL;
}

cl_object lisp_gl_create_program() {
    GLuint program;
    program = glCreateProgram();
    return ecl_make_fixnum(program);
}

cl_object lisp_gl_attach_shader(cl_object program, cl_object shader) {
    glAttachShader(ecl_fixnum(program), ecl_fixnum(shader));
    return ECL_NIL;
}

cl_object lisp_gl_link_program(cl_object program) {
    glLinkProgram(ecl_fixnum(program));
    return ECL_NIL;
}

cl_object lisp_gl_use_program(cl_object program) {
    glUseProgram(ecl_fixnum(program));
    return ECL_NIL;
}

cl_object lisp_gl_get_attrib_location(cl_object program, cl_object index) {
    // TODO the second arg needs to be passed in
    char * ptr = ecl_base_string_pointer_safe (ecl_null_terminated_base_string(index));
    GLint posAttrib = glGetAttribLocation(ecl_fixnum(program), ptr);
    return ecl_make_fixnum(posAttrib);
}

cl_object lisp_gl_enable_vertex_attrib_array(cl_object attrib) {
    glEnableVertexAttribArray(ecl_fixnum(attrib));
    return ECL_NIL;
}

cl_object lisp_gl_vertex_attrib_pointer(cl_object index, cl_object size, cl_object type,
    cl_object normalized, cl_object stride, cl_object pointer) {
    glVertexAttribPointer(
        ecl_fixnum(index),
        ecl_fixnum(size),
        ecl_fixnum(type),
        ecl_fixnum(normalized), 0, 0);
    return ECL_NIL;
}

void setup_cl_opengl() {
    cl_object aux;
    aux = cl_eval(c_string_to_object("(load \"ecl-opengl.lisp\")"));
    ecl_print(aux, ECL_T);

    aux = ecl_make_symbol("CLEAR-COLOR", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_clear_color, 4);

    aux = ecl_make_symbol("CLEAR", "GL");
    ecl_def_c_function(aux, lisp_gl_clear, 0);

    aux = ecl_make_symbol("DRAW-ARRAYS", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_draw_arrays, 3);

    aux = ecl_make_symbol("GEN-VERTEX-ARRAY", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_gen_vertex_array, 0);

    aux = ecl_make_symbol("BIND-VERTEX-ARRAY", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_bind_vertex_array, 1);

    aux = ecl_make_symbol("GEN-BUFFER", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_gen_buffer, 0);

    aux = ecl_make_symbol("BIND-BUFFER", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_bind_buffer, 2);

    aux = ecl_make_symbol("GL-BUFFER-DATA", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_buffer_data, 4);

    aux = ecl_make_symbol("CREATE-SHADER", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_create_shader, 1);

    aux = ecl_make_symbol("SHADER-SOURCE", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_shader_source, 2);

    aux = ecl_make_symbol("COMPILE-SHADER", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_compile_shader, 1);

    aux = ecl_make_symbol("CREATE-PROGRAM", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_create_program, 0);

    aux = ecl_make_symbol("ATTACH-SHADER", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_attach_shader, 2);

    aux = ecl_make_symbol("LINK-PROGRAM", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_link_program, 1);

    aux = ecl_make_symbol("USE-PROGRAM", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_use_program, 1);

    aux = ecl_make_symbol("GET-ATTRIB-LOCATION", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_get_attrib_location, 2);

    aux = ecl_make_symbol("ENABLE-VERTEX-ATTRIB-ARRAY", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_enable_vertex_attrib_array, 1);

    aux = ecl_make_symbol("VERTEX-ATTRIB-POINTER", "GL");
    ecl_def_c_function(aux, (cl_objectfn_fixed) lisp_gl_vertex_attrib_pointer, 6);
}

int main(int argc, char** argv)
{
    cl_object result;
    cl_boot(argc, argv);

    setup_cl_opengl();

    result = cl_eval(c_string_to_object("(load \"hello-lisp.lisp\")"));
    ecl_print(result, ECL_T);

    result = cl_eval(c_string_to_object("(format nil \"Hello Triangle\")"));
    ecl_print(result, ECL_T);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    auto wnd(
        SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));

    auto glc = SDL_GL_CreateContext(wnd);

    SDL_GL_SetSwapInterval(0);

    auto rdr = SDL_CreateRenderer(
        wnd, -1, SDL_RENDERER_ACCELERATED);

    result = cl_eval(c_string_to_object("(setup-opengl-program)"));
    ecl_print(result, ECL_T);

    loop = [&]
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) std::terminate();
        }

        result=cl_eval(c_string_to_object("(render-triangle-from-lisp)"));
        // ecl_print(result, ECL_T);

        SDL_GL_SwapWindow(wnd);
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, true);
#else
    while(true) main_loop();
#endif

    return 0;
}
