🔍 Iniciando verificación de tipos...
✅ Variable 'x' declarada con tipo int
✅ Variable 'y' declarada con tipo int
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ If statement verificado correctamente
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
  subq $16, %rsp
  # Reservado $16 bytes para variables locales
  movq $5, %rax
  movq %rax, -8(%rbp)
  movq $0, %rax
  pushq %rax
  movq -8(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setl %al
  cmpq $0, %rax
  je else_0
  subq $16, %rsp
  # Reservado $16 bytes para variables locales
  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  pushq %rax
  movq -8(%rbp), %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
  movq %rax, -16(%rbp)
  addq $16, %rsp
  # Liberado $16 bytes de variables locales
  jmp endif_0
else_0:
  subq $16, %rsp
  # Reservado $16 bytes para variables locales
  movq -8(%rbp), %rax
  movq %rax, -16(%rbp)
  addq $16, %rsp
  # Liberado $16 bytes de variables locales
endif_0:
  movq -16(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $16, %rsp
  # Liberado $16 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
