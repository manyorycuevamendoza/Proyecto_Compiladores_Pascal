🔍 Iniciando verificación de tipos...
✅ Variable 'a' declarada con tipo int
✅ New statement verificado correctamente
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
  subq $8, %rsp
  # Reservado $8 bytes para variables locales
  # new statement - asignando memoria para puntero: a
  movq $8, %rdi
  call malloc@PLT
  movq %rax, -8(%rbp)
  movq $10, %rax
  pushq %rax
  movq -8(%rbp), %rax
  popq %rcx
  movq %rcx, (%rax)
  movq -8(%rbp), %rax
  movq (%rax), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  # dispose statement - liberando memoria para puntero: a
  movq -8(%rbp), %rdi
  call free@PLT
  addq $8, %rsp
  # Liberado $8 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
