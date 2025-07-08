.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $8, %rsp
  movq $1, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  cmpq $0, %rax
  je else_0
  subq $8, %rsp
  movq $1, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  jmp .end_
  jmp endif_0
else_0:
  subq $8, %rsp
  movq $0, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  jmp .end_
endif_0:
  movl $0, %eax
  jmp .end_main
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
