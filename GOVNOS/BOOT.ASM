;
; BOOT.ASM - GovnOS operating system
;
; Copyright 2024 Xi816 <xi816/cgovnos>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;

CALL MAIN
  PUSH %00
  PUSH %00
  INT
  WELCOME-MS:
    BYTES "GOVNBOOT$"
    BYTES "  PRESS A TO START$"
    BYTES "  PRESS H TO SHOW INFO$"
    BYTES "  PRESS Y TO SHUTDOWN (FORCE CLOSE)$"
    BYTES "  PRESS Z TO SHUTDOWN (SAFE HALT)$^@"
  INFO-MS:
    BYTES "INFO ABOUT GOVNBOOT$"
    BYTES "  VERSION: 0.1$"
    BYTES "  MADE BY: XI-816$^@"
  SAFE-TO-SD-MS:
    BYTES "IT IS NOW SAFE TO SHUT DOWN YOUR COMPUTER$^@"
  LOAD-KERNEL-MS:
    BYTES "LOADING KERNEL AT POSITION 0800H$^@"
  NOT-IMP-MS:
    BYTES "NOT IMPLEMENTED$HALTING...$^@"
  HALT:
    BYTES %0A9 %55
MAIN:
  PUSH WELCOME-MS
  PUSH %01
  INT
INP-BOOTMODE:
  CSP

  DUP
  PUSH %61            ; IF USER PRESSED A
  CMP
  CALE LOAD-KERNEL

  DUP
  PUSH %68            ; IF USER PRESSED H
  CMP
  CALE SHOW-INFO

  DUP
  PUSH %79            ; IF USER PRESSED Y
  CMP
  JE FORCE-EXIT

  DUP
  PUSH %7A            ; IF USER PRESSED Z
  CMP
  JE SAFE-EXIT

  JMP INP-BOOTMODE    ; ELSE JUMP BACK

SHOW-INFO:            ; SHOW INFO ABOUT THE
  PUSH INFO-MS        ; BOOTLOADER
  PUSH %01
  INT
  RET

LOAD-KERNEL:          ; LOAD THE KERNEL
  PUSH LOAD-KERNEL-MS
  PUSH %01
  INT

  PUSH NOT-IMP-MS
  PUSH %01
  INT
  BYTES %0A9 %55

FORCE-EXIT:           ; FORCE THE EXITING
  PUSH %00
  PUSH %00
  INT

SAFE-EXIT:
  PUSH SAFE-TO-SD-MS
  PUSH %01
  INT
  JMP HALT

; END WITH FFH TO SHOW TO
; GOVNOS THAT THIS CODE IS
; BOOTABLE
BYTES %0FF

