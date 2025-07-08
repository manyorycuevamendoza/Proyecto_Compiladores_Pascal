🔍 Iniciando verificación de tipos...
✅ Variable 'x' declarada con tipo int
✅ Variable 'arr' declarada con tipo int
✅ Variable 'p' declarada con tipo int
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Verificación de tipos exitosa
.data
print_fmt: .string "%ld\n"
.text
  # procesando 0 funciones

# Función main
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
  movq $5, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  pushq %rax
  movq $2, %rax
  pushq %rax
  leaq -16(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $2, %rax
  movq $10, %rax
  pushq %rax
  movq $3, %rax
  pushq %rax
  leaq -16(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $3, %rax
  movq $2, %rax
  pushq %rax
  leaq -16(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  movq (%rax), %rax
  movq $2, %rax
  pushq %rax
  movq $3, %rax
  pushq %rax
  leaq -16(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  movq (%rax), %rax
  movq $3, %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  pushq %rax
  movq $4, %rax
  pushq %rax
  leaq -16(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $4, %rax
  movq $4, %rax
  pushq %rax
  leaq -16(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  movq (%rax), %rax
  movq $4, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  leaq -8(%rbp), %rax
  movq %rax, -24(%rbp)
  movq -24(%rbp), %rax
  movq (%rax), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
