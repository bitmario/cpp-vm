
call  .main
printi  t0, 1
halt
.main:
    push  r0
    push  r1
    push  r5
    push  ra
    push  bp
    mov  bp, sp
    lconsw  r1, 16
    sub  sp, sp, r1
    lcons  r0, 0
    lconsw  r5, 16
    sub  r5, bp, r5
    stor_p  r5, r0
    lcons  r0, 2
    lconsw  r5, 4
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_60PT8G:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 100000
    pop  r1
    jl  r1, r0, .loc_Z1M0U8
    lconsb  r0, 0
    jmp  .loc_4EKRW3
.loc_Z1M0U8:
    lconsb  r0, 1
.loc_4EKRW3:
    jz  r0, .loc_G8OJZ7
    lcons  r0, 1
    lconsw  r5, 12
    sub  r5, bp, r5
    stor_p  r5, r0
    lcons  r0, 2
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_1W2TS0:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    pop  r1
    jl  r1, r0, .loc_1ZXXXI
    lconsb  r0, 0
    jmp  .loc_M3ET0A
.loc_1ZXXXI:
    lconsb  r0, 1
.loc_M3ET0A:
    jz  r0, .loc_2YSPO4
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    pop  r1
    imod  r0, r1, r0
    push  r0
    lcons  r0, 0
    pop  r1
    je  r1, r0, .loc_CUL30K
    lconsb  r0, 0
    jmp  .loc_Z30MBF
.loc_CUL30K:
    lconsb  r0, 1
.loc_Z30MBF:
    jz  r0, .loc_8NOB6G
    lcons  r0, 0
    lconsw  r5, 12
    sub  r5, bp, r5
    stor_p  r5, r0
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_8NOB6G:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
    jmp  .loc_1W2TS0
.loc_2YSPO4:
    lconsw  r5, 12
    sub  r5, bp, r5
    load_p  r0, r5
    jz  r0, .loc_K24JQ2
    lconsw  r5, 16
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 1
    pop  r1
    add  r0, r1, r0
    lconsw  r5, 16
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_K24JQ2:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    inc  r0
    stor_p  r5, r0
    jmp  .loc_60PT8G
.loc_G8OJZ7:
    lconsw  r5, 16
    sub  r5, bp, r5
    load_p  r0, r5
    mov  t0, r0
    mov  sp, bp
    pop  bp
    pop  ra
    pop  r5
    pop  r1
    pop  r0
    ret
    lconsb  r0, 0
    mov  t0, r0
    mov  sp, bp
    pop  bp
    pop  ra
    pop  r5
    pop  r1
    pop  r0
    ret