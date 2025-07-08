🔍 Iniciando verificación de tipos...
✅ Variable 'i' declarada con tipo int
✅ Variable 'pares' declarada con tipo int
✅ Variable 'impares' declarada con tipo int
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ If statement verificado correctamente
✅ For statement verificado correctamente
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
  movq $0, %rax
  movq %rax, -16(%rbp)
  movq $0, %rax
  movq %rax, -24(%rbp)
  movq $1, %rax
  movq %rax, -8(%rbp)
for_0:
  movq -8(%rbp), %rax
  movq $10, %rax
  cmpq %rax, -8(%rbp)
  jg endfor_1
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
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
  cmpq $0, %rax
  je else_2
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
  movq -16(%rbp), %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, -16(%rbp)
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
  jmp endif_2
else_2:
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
  movq -24(%rbp), %rax
  pushq %rax
  movq $1, %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, -24(%rbp)
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
endif_2:
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
  incq -8(%rbp)
  jmp for_0
endfor_1:
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
