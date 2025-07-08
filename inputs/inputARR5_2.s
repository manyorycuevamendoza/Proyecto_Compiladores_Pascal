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
  movq $5, %rax
  cmpq %rax, -48(%rbp)
  jg endfor_0
  subq $48, %rsp
  movq -48(%rbp), %rax
  pushq %rax
  movq $2, %rax
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
  pushq %rax
  movq -48(%rbp), %rax
  pushq %rax
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
