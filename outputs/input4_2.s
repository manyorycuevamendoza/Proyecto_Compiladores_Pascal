🔍 Iniciando verificación de tipos...
✅ Función 'Resta' registrada para verificación de tipos
✅ Variable 'x' declarada con tipo int
✅ Variable 'y' declarada con tipo int
✅ Variable 'resultado' declarada con tipo int
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Asignación verificada correctamente
✅ Print statement verificado correctamente
✅ Verificación de tipos exitosa
.data
print_fmt: .string "%ld\n"
.text
  # procesando 1 funciones

# Función: Resta
.globl Resta
Resta:
  # Prologue
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp
  movq %rdi, -16(%rbp)
  movq -16(%rbp), %rax
  pushq %rax
