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
    lconsw  r1, 8
    sub  sp, sp, r1
    lcons  r0, 0
    lconsw  r5, 4
    sub  r5, bp, r5
    stor_p  r5, r0
    lcons  r0, 1
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_JOUYN3:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 5
    pop  r1
    jl  r1, r0, .loc_WCJ1VX
    lconsb  r0, 0
    jmp  .loc_VPR95G
.loc_WCJ1VX:
    lconsb  r0, 1
.loc_VPR95G:
    jz  r0, .loc_5GS381
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 2
    pop  r1
    imul  r0, r1, r0
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 1
    pop  r1
    add  r0, r1, r0
    lconsw  r5, 4
    sub  r5, bp, r5
    stor_p  r5, r0
    jmp  .loc_JOUYN3
.loc_5GS381:
    lconsw  r5, 8
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