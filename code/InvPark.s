

;******************************************************************************
;                                                                             *
; Filename:  invpark.s							                              *
;                                                                             *
;-----------------------------------------------------------------------------*
; Date           Revision   Author    Desrcription                            *
; (mmddyy)       (MM.mm)                                                      *
;-----------------------------------------------------------------------------*
; 09/20/07        5.00	  	M91260			Ported code from 30F to 33F	      *
;																			  *
;******************************************************************************
;      Code Description														  *
;*******************************************************************
; InvPark
;  
;Description:        
;  Calculate the inverse Park transform. Assumes the Cos and Sin values 
;  are in the ParkParm structure.
;
;         Valpha =  Vd*cos(Angle) - Vq*sin(Angle)
;         Vbeta  =  Vd*sin(Angle) + Vq*cos(Angle)
;
;  This routine works the same for both integer scaling and 1.15 scaling.
;
;Functional prototype:
; 
; void InvPark( void )
;
;On Entry:   The ParkParm structure must contain qCos, qSin, qVd and qVq.
;
;On Exit:    ParkParm will contain qValpha, qVbeta.
;
;Parameters: 
; Input arguments: None
;
; Return:
;   Void
;
; SFR Settings required:
;         CORCON.SATA  = 0

; Support routines required: None
;
; Local Stack usage: none
;
; Registers modified: w3 -> w7, A
;
; Timing:
;  About 14 instruction cycles
;
;*******************************************************************
;
          .include "general.inc"

; External references
          .include "park.inc"

; Register usage
          .equ ParmW,  w3    ; Ptr to ParkParm structure

          .equ SinW,   w4
          .equ CosW,   w5
          .equ VdW,    w6    ; copy of qVd
          .equ VqW,    w7    ; copy of qVq



;=================== CODE =====================

          .section  .text
          .global   _InvPark
          .global   InvPark

_InvPark:
InvPark:
     ;; Get qVd, qVq from ParkParm structure
          mov.w     _ParkParm+Park_qVd,VdW
          mov.w     _ParkParm+Park_qVq,VqW

     ;; Get qSin, qCos from ParkParm structure
          mov.w     _ParkParm+Park_qSin,SinW
          mov.w     _ParkParm+Park_qCos,CosW

     ;; Valpha =  Vd*cos(Angle) - Vq*sin(Angle)

          mpy       CosW*VdW,A        ; Vd*qCos -> A
          msc       SinW*VqW,A        ; sub Vq*qSin from A

          mov.w     #_ParkParm+Park_qValpha,ParmW
          sac       A,[ParmW++]        ; store to qValpha, inc ptr to qVbeta

     ;; Vbeta  =  Vd*sin(Angle) + Vq*cos(Angle)
          mpy       SinW*VdW,A        ; Vd*qSin -> A
          mac       CosW*VqW,A        ; add Vq*qCos to A
          sac       A,[ParmW]          ; store to Vbeta

          return
          .end
