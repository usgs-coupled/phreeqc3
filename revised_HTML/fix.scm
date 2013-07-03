(define (script-fu-fix-files)
  (fix-top-line "phreeqc3-1.png")
)

(define (fix-top-line filename)

   (let* (
	  (image (car (gimp-file-load RUN-NONINTERACTIVE filename filename)))
          (layer (car (gimp-image-get-active-layer image)))
          (display (car (gimp-display-new image)))
	  (width (car (gimp-drawable-width layer)))
	  (new_width(/ width 2))
	  (height (car (gimp-drawable-height layer)))
	  (new_height(/ height 2))
	  (gimp-layer-scale layer new_width new_height FALSE)
	  )
   )
)

( script-fu-register
 "script-fu-fix-files"
 "<Image>/Xtns/Script-Fu/Scott/Fix Files"
 "Removes black line from top of JPEG file"
 "Scott Charlton"
 "copyright 1999, Scott Charlton"
 "December 9, 1999"
 ""
)
