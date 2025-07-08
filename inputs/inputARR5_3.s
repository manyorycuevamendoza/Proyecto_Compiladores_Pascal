.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $32, %rsp
  movq $10, %rax
  pushq %rax
  movq $1, %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  subq $1, %rcx
  imulq $8, %rcx
  subq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $20, %rax
  pushq %rax
  movq $2, %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  subq $1, %rcx
  imulq $8, %rcx
  subq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $30, %rax
  pushq %rax
  movq $3, %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  subq $1, %rcx
  imulq $8, %rcx
  subq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $1, %rax
  movq %rax, -32(%rbp)
for_0:
  movq -32(%rbp), %rax
  movq $3, %rax
  cmpq %rax, -32(%rbp)
  jg endfor_0
  subq $32, %rsp
  movq -32(%rbp), %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  subq $1, %rcx
  imulq $8, %rcx
  subq %rcx, %rax
  movq (%rax), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  incq -32(%rbp)
  jmp for_0
endfor_0:
  movl $0, %eax
  jmp .end_main
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
