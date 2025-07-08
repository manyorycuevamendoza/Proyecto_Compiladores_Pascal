import subprocess
import os

# Carpeta de entrada y salida
input_dir = "inputs"
output_dir = "outputs"

# Archivos fuente para el compilador completo
source_files = [
    "main.cpp", "parser.cpp", "scanner.cpp", "token.cpp",
    "visitor.cpp", "exp.cpp", "gencode.cpp", "imp_type.cpp", "imp_value.cpp", "imp_interpreter.cpp",
    "type_checker.cpp"
]

# Archivos fuente para el generador de código assembly
asm_source_files = [
    "main_asm.cpp", "parser.cpp", "scanner.cpp", "token.cpp",
    "visitor.cpp", "exp.cpp", "gencode.cpp", "imp_type.cpp", "imp_value.cpp"
]

# Archivos fuente para el generador simplificado
simple_source_files = [
    "main_simple.cpp", "parser.cpp", "scanner.cpp", "token.cpp",
    "visitor.cpp", "exp.cpp", "gencode_simple.cpp", "imp_type.cpp", "imp_value.cpp"
]

# Crear carpeta de salida si no existe
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Compilar compilador completo
print("Compilando compilador completo...")
compile_cmd = ["g++", "-o", "compilador"] + source_files
result = subprocess.run(compile_cmd)

if result.returncode != 0:
    print("Error de compilación del compilador completo.")
    exit(1)

print("Compilación del compilador completo exitosa.")

# Compilar generador de código assembly
print("Compilando generador de código assembly...")
asm_compile_cmd = ["g++", "-o", "compilador_asm"] + asm_source_files
result = subprocess.run(asm_compile_cmd)

if result.returncode != 0:
    print("Error de compilación del generador de código assembly.")
    exit(1)

print("Compilación del generador de código assembly exitosa.")

# Compilar generador simplificado
print("Compilando generador simplificado...")
simple_compile_cmd = ["g++", "-o", "compilador_simple"] + simple_source_files
result = subprocess.run(simple_compile_cmd)

if result.returncode != 0:
    print("Error de compilación del generador simplificado.")
    exit(1)

print("Compilación del generador simplificado exitosa.\n")

# Procesar todos los archivos .txt en la carpeta inputs
for filename in os.listdir(input_dir):
    if filename.endswith(".txt"):
        input_file = os.path.join(input_dir, filename)
        output_file = os.path.join(input_dir, filename.rsplit(".", 1)[0] + ".s")
        print(f"\nEjecutando con {input_file}")
        with open(output_file, "w") as outfile:
            subprocess.run(["./compilador_asm", input_file], stdout=outfile, stderr=subprocess.DEVNULL)
        print(f"Salida ensamblador guardada en {output_file}")