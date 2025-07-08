🔍 Iniciando verificación de tipos...
✅ Variable 'n' declarada con tipo int
✅ Variable 'esPar' declarada con tipo bool
✅ Variable 'estaEnRango' declarada con tipo bool
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
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
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
  movq $17, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  pushq %rax
  movq $2, %rax
  movq %rax, %rcx
  popq %rax
  cqto
  idivq %rcx
  movq %rdx, %rax
  pushq %rax
  movq $0, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  sete %al
  movq %rax, -16(%rbp)
  movq -8(%rbp), %rax
  pushq %rax
  movq $10, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setge %al
  pushq %rax
  movq -8(%rbp), %rax
  pushq %rax
  movq $20, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setle %al
  movq %rax, %rcx
  popq %rax
  andq %rcx, %rax
  movq %rax, -24(%rbp)
  movq -16(%rbp), %rax
  pushq %rax
  movq -24(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  andq %rcx, %rax
  cmpq $0, %rax
  je else_0
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
  movq $1, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
  jmp endif_0
else_0:
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
  movq $0, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
endif_0:
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
