🔍 Iniciando verificación de tipos...
✅ Función 'suma' registrada para verificación de tipos
✅ Variable 'x' declarada con tipo int
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Verificación de tipos exitosa
.data
print_fmt: .string "%ld\n"
.text
  # procesando 1 funciones

# Función: suma
.globl suma
suma:
  # Prologue
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq %rdi, -16(%rbp)
  movq -16(%rbp), %rax
  pushq %rax
  movq $5, %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  # Epilogue
  movq %rbp, %rsp
  popq %rbp
  ret

# Función main
.globl main
main:
  pushq %rbp
  movq %rsp, %rbp
  subq $8, %rsp
  # Reservado $8 bytes para variables locales
  movq $10, %rax
  movq %rax, -8(%rbp)
  pushq %rax
  pushq %rdi
  movq -8(%rbp), %rax
  movq %rax, %rdi
  call suma
  popq %rdi
  popq %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $8, %rsp
  # Liberado $8 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
