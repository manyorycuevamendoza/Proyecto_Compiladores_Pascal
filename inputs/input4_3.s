.data
print_fmt: .string "%ld\n"
.text

# Función: Factorial
.globl Factorial
Factorial:
  # Prologue
  pushq %rbp
  movq %rsp, %rbp
  movq %rdi, -8(%rbp)
  subq $8, %rsp
  movq -8(%rbp), %rax
  pushq %rax
  movq $0, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  sete %al
  movzbq %al, %rax
  cmpq $0, %rax
  je else_0
  movq $1, %rax
  jmp .end_Factorial
  jmp endif_0
else_0:
  movq -8(%rbp), %rax
  pushq %rax
  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  subq %rcx, %rax
  movq %rax, %rdi
  call Factorial
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
  jmp .end_Factorial
endif_0:
.end_Factorial:
  leave
  ret
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $8, %rsp
  movq $5, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  movq %rax, %rdi
  call Factorial
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
