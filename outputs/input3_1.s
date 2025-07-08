🔍 Iniciando verificación de tipos...
✅ Variable 'x' declarada con tipo int
✅ Asignación verificada correctamente
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
  movq $7, %rax
  movq %rax, -8(%rbp)
  movq -8(%rbp), %rax
  pushq %rax
  movq $0, %rax
  movq %rax, %rcx
  popq %rax
  cmpq %rcx, %rax
  movl $0, %eax
  setg %al
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
endif_0:
  addq $8, %rsp
  # Liberado $8 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
