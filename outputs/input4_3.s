🔍 Iniciando verificación de tipos...
✅ Función 'Factorial' registrada para verificación de tipos
✅ Variable 'k' declarada con tipo int
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Verificación de tipos exitosa
.data
print_fmt: .string "%ld\n"
.text
  # procesando 1 funciones

# Función: Factorial
.globl Factorial
Factorial:
  # Prologue
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq %rdi, -16(%rbp)
  movq -16(%rbp), %rax
  pushq %rax
  movq $0, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  sete %al
  cmpq $0, %rax
  je else_0
  movq $1, %rax
  movq %rax, -8(%rbp)
  jmp endif_0
else_0:
  movq -16(%rbp), %rax
  pushq %rax
  pushq %rax
  pushq %rdi
  movq -16(%rbp), %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  subq %rcx, %rax
  movq %rax, %rdi
  call Factorial
  popq %rdi
  popq %rax
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
  movq %rax, -8(%rbp)
endif_0:
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
  movq $5, %rax
  movq %rax, -8(%rbp)
  pushq %rax
  pushq %rdi
  movq -8(%rbp), %rax
  movq %rax, %rdi
  call Factorial
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
