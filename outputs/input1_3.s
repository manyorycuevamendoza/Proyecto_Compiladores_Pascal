🔍 Iniciando verificación de tipos...
✅ Variable 'a' declarada con tipo int
✅ Variable 'b' declarada con tipo int
✅ Variable 'c' declarada con tipo int
✅ Variable 'flag' declarada con tipo bool
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
  subq $32, %rsp
  # Reservado $32 bytes para variables locales
  movq $10, %rax
  movq %rax, -8(%rbp)
  movq $20, %rax
  movq %rax, -16(%rbp)
  movq -16(%rbp), %rax
  pushq %rax
  movq -8(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, -24(%rbp)
  movq $1, %rax
  movq %rax, -32(%rbp)
  movq -8(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -24(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  movq -32(%rbp), %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $32, %rsp
  # Liberado $32 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
