.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  # new statement - asignando memoria para puntero: px
  movq $8, %rdi
  call malloc@PLT
  movq %rax, -8(%rbp)
  # new statement - asignando memoria para puntero: p
  movq $8, %rdi
  call malloc@PLT
  movq %rax, -16(%rbp)
  movq $15, %rax
  pushq %rax
  movq -8(%rbp), %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $1, %rax
  pushq %rax
  movq -16(%rbp), %rax
  popq %rcx
  movq %rcx, (%rax)
  movq -8(%rbp), %rax
  movq (%rax), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -16(%rbp), %rax
  movq (%rax), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  # dispose statement - liberando memoria para puntero: px
  movq -8(%rbp), %rdi
  call free@PLT
  # dispose statement - liberando memoria para puntero: p
  movq -16(%rbp), %rdi
  call free@PLT
  movl $0, %eax
  jmp .end_main
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
