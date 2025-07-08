🔍 Iniciando verificación de tipos...
✅ Variable 'datos' declarada con tipo int
✅ Variable 'i' declarada con tipo int
✅ Variable 'suma' declarada con tipo int
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ For statement verificado correctamente
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
  movq $3, %rax
  pushq %rax
  movq $1, %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $1, %rax
  movq $5, %rax
  pushq %rax
  movq $2, %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq $2, %rax
  movq $0, %rax
  movq %rax, -24(%rbp)
  movq $1, %rax
  movq %rax, -16(%rbp)
for_0:
  movq -16(%rbp), %rax
  movq $2, %rax
  cmpq %rax, -16(%rbp)
  jg endfor_1
  subq $24, %rsp
  # Reservado $24 bytes para variables locales
  movq -24(%rbp), %rax
  pushq %rax
  movq -16(%rbp), %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  movq (%rax), %rax
  movq -16(%rbp), %rax
  movq %rax, %rcx
  popq %rax
  addq %rcx, %rax
  movq %rax, -24(%rbp)
  addq $24, %rsp
  # Liberado $24 bytes de variables locales
  incq -16(%rbp)
  jmp for_0
endfor_1:
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
