.data
print_fmt: .string "%ld\n"
.text
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $48, %rsp
  movq $1, %rax
  movq %rax, -48(%rbp)
for_0:
  movq -48(%rbp), %rax
  pushq %rax
  movq $5, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setle %al
  movzbq %al, %rax
  cmpq $0, %rax
  je endfor_0
  movq -48(%rbp), %rax
  pushq %rax
  movq $10, %rax
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
  pushq %rax
  movq -48(%rbp), %rax
  pushq %rax
  movq %rax, %rcx
  leaq -8(%rbp), %rax
  popq %rcx
  subq $1, %rcx
  imulq $8, %rcx
  subq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  incq -48(%rbp)
  jmp for_0
endfor_0:
  movq $3, %rax
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
  movl $0, %eax
  jmp .end_main
.end_main:
  leave
  ret
.section .note.GNU-stack,"",@progbits
