🔍 Iniciando verificación de tipos...
✅ Variable 'x' declarada con tipo int
✅ Variable 'px' declarada con tipo int
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Dispose statement verificado correctamente
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
  movq $15, %rax
  movq %rax, -8(%rbp)
  leaq -8(%rbp), %rax
  movq %rax, -16(%rbp)
  movq -16(%rbp), %rax
  movq (%rax), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq $99, %rax
  pushq %rax
  movq -16(%rbp), %rax
  popq %rcx
  movq %rcx, (%rax)
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  # dispose statement - liberando memoria para puntero: px
  movq -16(%rbp), %rdi
  call free@PLT
  addq $16, %rsp
  # Liberado $16 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
