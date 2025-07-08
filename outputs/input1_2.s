🔍 Iniciando verificación de tipos...
✅ Variable 'x' declarada con tipo int
✅ Variable 'y' declarada con tipo int
✅ Variable 'z' declarada con tipo int
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Print statement verificado correctamente
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
  movq $2, %rax
  movq %rax, -8(%rbp)
  movq $5, %rax
  movq %rax, -16(%rbp)
  movq $34, %rax
  movq %rax, -24(%rbp)
  movq $9, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -16(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -24(%rbp), %rax
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
