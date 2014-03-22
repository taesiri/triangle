#lang racket
 
(require racket/class)
(require racket/gui/base)
(require sgl)
(require sgl/gl-vectors)
 
(define c%
  (class canvas%
    (inherit refresh with-gl-context swap-gl-buffers)
 
    (super-new)
  
    (define rotation-x 0.0)
    (define rotation-y 0.0)
 
    (define/public (STEP)
  
      (set! rotation-y (+ rotation-y 2))
  
      (refresh)
      (sleep/yield 1/60)
      (queue-callback (lambda _ (send this STEP)) #f))
 
    (define/override (on-paint)
      (with-gl-context
       (lambda ()
         (gl-clear-color 0.0 0.0 0.0 0.0)
         (gl-clear 'color-buffer-bit 'depth-buffer-bit)
 
         (gl-push-matrix)
         (gl-rotate rotation-x 1.0 0.0 0.0)
         (gl-rotate  rotation-y 0.0 1.0 0.0)
 
         (gl-begin 'triangles)
         (gl-vertex 0 1 0)
         (gl-vertex 1 -1 0)
         (gl-vertex -1 -1 0)
         (gl-end)
 
         (gl-pop-matrix)
         (swap-gl-buffers)
         (gl-flush))))
 
    (define/override (on-char e)
      (when (is-a? e key-event%)
        (when (eq? (send e get-key-code) 'escape)
          (exit 0))))
 
    (define/override (on-size width height)
      (with-gl-context
       (lambda ()
         (gl-viewport 0 0 width height)
 
         (gl-matrix-mode 'projection)
         (gl-load-identity)
         (let ((h (/ height width)))
           (gl-frustum -1.0 1.0 (- h) h 5.0 60.0))

         (gl-matrix-mode 'modelview)
         (gl-load-identity)
         (gl-translate 0.0 0.0 -10.0)
 
         (gl-enable 'depth-test)
 
         (gl-color-material 'front 'ambient-and-diffuse)
         (gl-enable 'color-material)
         ))
      (refresh))))
 
(module+ main
  (define-values (W H) (values 400 400))
  (define f (new frame%
                 [style '(no-resize-border
                          no-caption
                          no-system-menu
                          hide-menu-bar)]
                 [label "Traingle"]
                 [width W]
                 [height H]))
  (define c (new c%
                 [parent f]
                 [style '(gl)]))
  (send f show #t)
  (send f center 'both)
 
  (send c STEP))