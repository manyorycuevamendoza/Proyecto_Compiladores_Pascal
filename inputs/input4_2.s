.data
print_fmt: .string "%ld\n"
.text

# Función: Resta
.globl Resta
Resta:
  # Prologue
  pushq %rbp
  movq %rsp, %rbp
  movq %rdi, -8(%rbp)
  movq %rsi, -16(%rbp)
  subq $16, %rsp
  movq -8(%rbp), %rax
  pushq %rax
  movq -16(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  subq %rcx, %rax
.end_Resta:
  leave
  ret
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $24, %rsp
  movq $20, %rax
  movq %rax, -8(%rbp)
  movq $7, %rax
  movq %rax, -16(%rbp)
  movq -16(%rbp), %rax
  movq %rax, %rsi
  movq -8(%rbp), %rax
  movq %rax, %rdi
  call Resta
  movq %rax, -24(%rbp)
  movq -24(%rbp), %rax
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
