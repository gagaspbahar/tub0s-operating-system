global _interrupt

_interrupt:
  push bp
  mov  bp, sp
  mov  ax, [bp+4]
  push ds
  mov  bx, cs
  mov  ds, bx
  mov  si, intr
  mov  [si+1], al
  pop  ds
  mov  ax, [bp+6]
  mov  bx, [bp+8]
  mov  cx, [bp+10]
  mov  dx, [bp+12]

intr: int 0x00

  mov ah, 0
  pop bp
  ret