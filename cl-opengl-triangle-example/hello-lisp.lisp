
(defmacro without-fpe-traps (&body body)
  `(let ((bits (si:trap-fpe 'cl:last t)))
     (unwind-protect
          (progn
            (si:trap-fpe t nil)
            ,@body)
       (si:trap-fpe bits t))))

(defconstant :triangles #x4)
(defconstant :array-buffer #x8892)
(defconstant :static-draw #x88E4)
(defconstant :fragment-shader #x8B30)
(defconstant :vertex-shader #x8B31)
(defconstant :gl-float #x1406)
(defconstant :gl-false #x0)

(defvar vertexSource "attribute vec4 position;
    varying vec3 color;
    void main()
    {
        gl_Position = vec4(position.xyz, 1.0);
        color = gl_Position.xyz + vec3(0.5);
    }")

(defvar fragmentSource "precision mediump float;
  varying vec3 color;
  void main()
  {
      gl_FragColor = vec4 ( color, 1.0 );
  }")

(defun setup-opengl-program ()
  (let ((vao (gl:gen-vertex-array))
        (vbo (gl:gen-buffer)))
    (gl:bind-vertex-array vao)
    (gl:bind-buffer :array-buffer vbo))

  (let* ((vertices  #(0.0 0.5 0.5 -0.5 -0.5 -0.5))
         (arr (gl:alloc-gl-array :float (length vertices))))
      (dotimes (i (length vertices))
        (setf (gl:glaref arr i) (aref vertices i)))
    (gl:buffer-data :array-buffer :static-draw arr))

  (let ((vertex-shader (gl:create-shader :vertex-shader))
        (fragment-shader (gl:create-shader :fragment-shader))
        (shader-program (gl:create-program))
        (pos-attrib nil))

    (gl:shader-source vertex-shader vertexSource)

    (gl:compile-shader vertex-shader)

    (gl:shader-source fragment-shader fragmentSource)
    (gl:compile-shader fragment-shader)

    (gl:attach-shader shader-program vertex-shader)
    (gl:attach-shader shader-program fragment-shader)
    (gl:link-program shader-program)
    (gl:use-program shader-program)

    (setf pos-attrib (gl:get-attrib-location shader-program "position"))
    (gl:enable-vertex-attrib-array pos-attrib)
    (gl:vertex-attrib-pointer pos-attrib 2 :gl-float :gl-false 0 0))
  "Done setting up opengl shaders/program/etc.")

(defun render-triangle-from-lisp ()
  (gl:clear-color 0.0 1.0 0.0 1.0)
  (gl:clear)

  (gl:draw-arrays :triangles 0 3))
