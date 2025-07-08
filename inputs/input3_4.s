.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $8, %rsp
  movq $16, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  pushq %rax
  movq $11, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setge %al
  movzbq %al, %rax
  cmpq $0, %rax
  je else_0
  subq $8, %rsp
  movq -8(%rbp), %rax
  pushq %rax
  movq $18, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setge %al
  movzbq %al, %rax
  cmpq $0, %rax
  je else_1
  subq $8, %rsp
  movq $1, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  jmp .end_
  jmp endif_1
else_1:
  subq $8, %rsp
  movq $2, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  jmp .end_
endif_1:
  jmp .end_
  jmp endif_0
else_0:
  subq $8, %rsp
  movq $3, %rax
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
