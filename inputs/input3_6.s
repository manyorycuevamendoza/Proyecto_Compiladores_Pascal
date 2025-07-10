.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq $5, %rax
  movq %rax, -8(%rbp)
  movq $1, %rax
  movq %rax, -16(%rbp)
while_0:
  movq -8(%rbp), %rax
  pushq %rax
  movq $0, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setg %al
  movzbq %al, %rax
  cmpq $0, %rax
  je endwhile_0
  movq -8(%rbp), %rax
  pushq %rax
  movq -16(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
  movq %rax, -16(%rbp)
  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  subq %rcx, %rax
  movq %rax, -8(%rbp)
  jmp while_0
endwhile_0:
  movq -16(%rbp), %rax
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
