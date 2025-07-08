🔍 Iniciando verificación de tipos...
✅ Variable 'v' declarada con tipo int
✅ Variable 'i' declarada con tipo int
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
  subq $16, %rsp
  # Reservado $16 bytes para variables locales
  movq $1, %rax
  movq %rax, -16(%rbp)
for_0:
  movq -16(%rbp), %rax
  movq $5, %rax
  cmpq %rax, -16(%rbp)
  jg endfor_1
  subq $16, %rsp
  # Reservado $16 bytes para variables locales
  movq -16(%rbp), %rax
  pushq %rax
  movq $10, %rax
  movq %rax, %rcx
  popq %rax
  imulq %rcx, %rax
  pushq %rax
  movq -16(%rbp), %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  popq %rcx
  movq %rcx, (%rax)
  movq -16(%rbp), %rax
  addq $16, %rsp
  # Liberado $16 bytes de variables locales
  incq -16(%rbp)
  jmp for_0
endfor_1:
  movq $3, %rax
  pushq %rax
  leaq -8(%rbp), %rax
  popq %rcx
  imulq $8, %rcx
  addq %rcx, %rax
  movq (%rax), %rax
  movq $3, %rax
  movq %rax, %rsi
  leaq print_fmt(%rip), %rdi
  movl $0, %eax
  call printf@PLT
  addq $16, %rsp
  # Liberado $16 bytes de variables locales
  movl $0, %eax
  leave
  ret
.section .note.GNU-stack,"",@progbits
