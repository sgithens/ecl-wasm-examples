(defpackage :gl
  (:use :cl)
  (:export
   :alloc-gl-array
   :bind-buffer
   :bind-vertex-array
   :buffer-data
   :clear
   :clear-color
   :create-shader
   :create-program
   :compile-shader
   :draw-arrays
   :gen-buffer
   :gen-vertex-array
   :gl-array
   :glaref
   :shader-source
   :attach-shader
   :link-program
   :use-program
   :get-attrib-location
   :enable-vertex-attrib-array
   :vertex-attrib-pointer
))

(in-package :gl)

(defun inc-pointer (ptr offset)
  "Return a pointer OFFSET bytes past PTR."
  (ffi:make-pointer (+ (ffi:pointer-address ptr) offset) :void))

(defun foreign-type-size (type)
  (ffi:size-of-foreign-type type))

(defstruct (gl-array (:copier nil))
  "Pointer to C array with size and type information attached."
  (pointer) ; (null-pointer))
  (size 0 :type unsigned-byte)
  (type nil :type (or symbol cons)))

(declaim (inline gl-array-pointer-offset))
(defun gl-array-pointer-offset (array offset)
  (inc-pointer (gl-array-pointer array)
               (* (foreign-type-size (gl-array-type array)) offset)))

(declaim (inline gl-array-byte-size))
(defun gl-array-byte-size (array)
  (* (gl-array-size array) (foreign-type-size (gl-array-type array))))

;; was definline
(defun buffer-data (target usage array &key (offset 0)
                               (size (gl-array-byte-size array)))
  (gl-buffer-data target size (gl-array-pointer-offset array offset) usage))

(defun mem-ref (ptr type &optional (offset 0))
  "Dereference an object of TYPE at OFFSET bytes from PTR."
  (let* ((type-size (ffi:size-of-foreign-type type)))
    (si:foreign-data-ref-elt
     (si:foreign-data-recast ptr (+ offset type-size) :void) offset type)))

(defun mem-set (value ptr type &optional (offset 0))
  "Set an object of TYPE at OFFSET bytes from PTR."
  (let* ((type-size (ffi:size-of-foreign-type type)))
    (si:foreign-data-set-elt
     (si:foreign-data-recast ptr (+ offset type-size) :void)
     offset type value)))

(defun mem-aref (ptr type &optional (index 0))
  "Like MEM-REF except for accessing 1d arrays."
  (mem-ref ptr type (* index (foreign-type-size type))))

(defun (setf mem-aref) (value ptr type &optional (index 0))
  "Like MEM-REF except for accessing 1d arrays."
  (mem-set value ptr type (* index (foreign-type-size type))))

(defun foreign-alloc (type &key count)
  (si:allocate-foreign-data :void (* count (ffi:size-of-foreign-type type))))

(defun glaref (array index)
  "Returns the INDEX-th component of ARRAY. If COMPONENT is
supplied and ARRAY is of a compound type the component named
COMPONENT is returned."
  (mem-aref (gl-array-pointer array) (gl-array-type array) index))

(declaim (inline (setf glaref)))
(defun (setf glaref) (value array index)
  "Sets the place (GLAREF ARRAY INDEX [COMPONENT]) to VALUE."
  (setf (mem-aref (gl-array-pointer array) (gl-array-type array) index)
        value))

(defun alloc-gl-array (type count)
;;   (if (get (bare-type type) 'vertex-array-allocate)
;;       (funcall (get (bare-type type) 'vertex-array-allocate)
;;                count)
  (make-gl-array :pointer (foreign-alloc type :count count)
                  :size count :type type)
                    ;;  )
                     )
