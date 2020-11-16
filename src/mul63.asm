section .text
    global mul_dq ;2 arguments are pointers.

mul_dq:
    mov  rax, [rdi]
    mul  qword [rsi]
    mov  rbx, 0x8000000000000000
    shl  rdx, 1
    and  rbx, rax
    cmp  rbx, 0
    jne  incr_msq
    je   ret_out
incr_msq:
    inc  rdx
    ;and rax, 0x7fffffffffffffff causes MS bit causes yasm to warn.
    shl  rax, 1
    shr  rax, 1
ret_out:
    mov  [rdi], rdx
    mov  [rdi + 8], rax
    ret
