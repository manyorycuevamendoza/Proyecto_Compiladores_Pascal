.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $24, %rsp
  movq $10, %rax
  pushq %rax
  movq $1, %rax
  pushq %rax
  movq %rax, %rcx
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
  movq %rax, %rcx
  leaq -8(%rbp), %rax
  popq %rcx
  subq $1, %rcx
  imulq $8, %rcx
  subq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $1, %rax
  movq %rax, -24(%rbp)
for_2:
  movq -24(%rbp), %rax
  pushq %rax
  movq $2, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setle %al
  movzbq %al, %rax
  cmpq $0, %rax
  je endfor_2
  movq -24(%rbp), %rax
  pushq %rax
  movq %rax, %rcx
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
  incq -24(%rbp)
  jmp for_2
endfor_2:
  movl $0, %eax
  jmp .end_main
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
