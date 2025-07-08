#!/usr/bin/env python3
"""
Script para ejecutar la interfaz web del compilador Pascal
"""

import os
import sys
import subprocess
import platform

def check_compiler():
    """Verificar si el compilador existe"""
    if os.path.exists("./compiler"):
        print("✅ Compilador ya existe")
        return True
    else:
        print("❌ Compilador no encontrado")
        return False

def setup_virtual_environment():
    """Configurar entorno virtual y dependencias"""
    print("📦 Configurando entorno virtual...")
    
    # Verificar si el entorno virtual existe
    if not os.path.exists("venv"):
        print("🔧 Creando entorno virtual...")
        try:
            subprocess.run([sys.executable, "-m", "venv", "venv"], check=True)
        except subprocess.CalledProcessError:
            print("❌ Error creando entorno virtual")
            return False
    
    # Determinar el comando pip correcto
    if platform.system() == "Windows":
        pip_cmd = "venv\\Scripts\\pip"
    else:
        pip_cmd = "venv/bin/pip"
    
    # Instalar dependencias
    print("📦 Instalando dependencias...")
    try:
        subprocess.run([pip_cmd, "install", "-r", "requirements.txt"], check=True)
        print("✅ Dependencias instaladas correctamente")
        return True
    except subprocess.CalledProcessError as e:
        print(f"❌ Error instalando flask: {e}")
        return False

def run_server():
    """Ejecutar el servidor Flask"""
    print("🚀 Iniciando servidor...")
    
    # Determinar el comando python correcto
    if platform.system() == "Windows":
        python_cmd = "venv\\Scripts\\python"
    else:
        python_cmd = "venv/bin/python"
    
    try:
        subprocess.run([python_cmd, "server.py"])
    except KeyboardInterrupt:
        print("\n👋 Servidor detenido")
    except Exception as e:
        print(f"❌ Error ejecutando servidor: {e}")

def main():
    print("🚀 Iniciando interfaz del compilador Pascal...")
    print("=" * 50)
    
    # Verificar compilador
    if not check_compiler():
        print("❌ No se puede continuar sin el compilador")
        return
    
    # Configurar entorno virtual
    if not setup_virtual_environment():
        print("❌ No se pudieron instalar las dependencias. Saliendo...")
        return
    
    # Ejecutar servidor
    run_server()

if __name__ == "__main__":
    main() 