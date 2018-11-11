
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
.loc_X99F9E:
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 10000
    pop  r1
    jl  r1, r0, .loc_CVGPJ0
    lconsb  r0, 0
    jmp  .loc_H9VISR
.loc_CVGPJ0:
    lconsb  r0, 1
.loc_H9VISR:
    jz  r0, .loc_BRRZYP
    lcons  r0, 1
    lconsw  r5, 12
    sub  r5, bp, r5
    stor_p  r5, r0
    lcons  r0, 2
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
.loc_KUPMKB:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lconsw  r5, 4
    sub  r5, bp, r5
    load_p  r0, r5
    pop  r1
    jl  r1, r0, .loc_RB64I7
    lconsb  r0, 0
    jmp  .loc_4JI35Z
.loc_RB64I7:
    lconsb  r0, 1
.loc_4JI35Z:
    jz  r0, .loc_KIT2BU
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
    je  r1, r0, .loc_3BADJ2
    lconsb  r0, 0
    jmp  .loc_BJA4GC
.loc_3BADJ2:
    lconsb  r0, 1
.loc_BJA4GC:
    jz  r0, .loc_WSIWP4
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
.loc_WSIWP4:
    lconsw  r5, 8
    sub  r5, bp, r5
    load_p  r0, r5
    push  r0
    lcons  r0, 1
    pop  r1
    add  r0, r1, r0
    lconsw  r5, 8
    sub  r5, bp, r5
    stor_p  r5, r0
    jmp  .loc_KUPMKB
.loc_KIT2BU:
    lconsw  r5, 12
    sub  r5, bp, r5
    load_p  r0, r5
    jz  r0, .loc_SZ7AXD
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
.loc_SZ7AXD:
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
    jmp  .loc_X99F9E
.loc_BRRZYP:
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