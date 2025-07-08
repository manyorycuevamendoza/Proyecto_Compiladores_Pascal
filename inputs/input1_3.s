.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $32, %rsp
  movq $10, %rax
  movq %rax, -8(%rbp)
  movq $20, %rax
  movq %rax, -16(%rbp)
  movq -16(%rbp), %rax
  pushq %rax
  movq -8(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, -24(%rbp)
  movq $1, %rax
  movq %rax, -32(%rbp)
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -24(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -32(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movl $0, %eax
  jmp .end_main
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
