🔍 Iniciando verificación de tipos...
✅ Variable 'flag' declarada con tipo bool
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Print statement verificado correctamente
✅ If statement verificado correctamente
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
  movq $1, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  cmpq $0, %rax
  je else_0
  subq $8, %rsp
  # Reservado $8 bytes para variables locales
  movq $1, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $8, %rsp
  # Liberado $8 bytes de variables locales
  jmp endif_0
else_0:
  subq $8, %rsp
  # Reservado $8 bytes para variables locales
  movq $0, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $8, %rsp
  # Liberado $8 bytes de variables locales
endif_0:
  addq $8, %rsp
  # Liberado $8 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
