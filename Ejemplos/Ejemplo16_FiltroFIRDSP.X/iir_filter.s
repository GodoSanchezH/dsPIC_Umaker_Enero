; File iir_filter.s
.equ iir_filterNumSections, 1
; Allocate and initialize filter coefficients
; These coefficients have been designed for use in the Transpose filter only
.section .xdata,data,xmemory
;.align 256
iir_filterCoefs:
 ;b0 b1 a1 b2 a2     
 ;'7fff'0b
 ;'7fff'
 ;'7fff'
 ;'7fff'
 ;'8000'
 ;'751e'

.hword 0x7fff,0x7fff, 0x8000, 0x7fff , 0x751e; b( 1,0)/2 ; a( 1,1)/2; b( 1,2)/2; a( 1,2)/2

; ..............................................................................
; Allocate states buffers in (uninitialized) Y data space
.section .ybss, bss, ymemory
iir_filterStates1:
.space iir_filterNumSections*2
iir_filterStates2:
.space iir_filterNumSections*2
; ..............................................................................
; Allocate and intialize filter structure
.section .data
.global _Filter1
_Filter1:
.hword iir_filterNumSections-1
.hword iir_filterCoefs
.hword 0xFF00
.hword iir_filterStates1
.hword iir_filterStates2
.hword 0x0000