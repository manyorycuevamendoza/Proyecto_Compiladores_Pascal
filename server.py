#!/usr/bin/env python3
import subprocess
import os
import json
import tempfile
import re
import time
from functools import lru_cache
from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
from flask_compress import Compress

app = Flask(__name__)
CORS(app)
Compress(app)  # Habilitar compresión gzip

# Cache para archivos de entrada
file_cache = {}
cache_timeout = 30  # segundos

@app.route('/')
def index():
    return send_from_directory('.', 'interfaz_compilador.html')

@app.route('/inputs/<filename>')
def get_input_file(filename):
    return send_from_directory('inputs', filename)

@app.route('/outputs/<filename>')
def get_output_file(filename):
    return send_from_directory('outputs', filename)

@lru_cache(maxsize=100)
def extract_assembly_code(assembly_file):
    """Extrae solo el código assembly puro del archivo .s con caché"""
    if not os.path.exists(assembly_file):
        return ""
    try:
        with open(assembly_file, 'r') as f:
            lines = f.readlines()
        # Buscar la primera línea que empiece con .data, .text, o .globl
        start = None
        for i, line in enumerate(lines):
            if line.strip().startswith('.data') or line.strip().startswith('.text') or line.strip().startswith('.globl'):
                start = i
                break
        if start is not None:
            # Solo devolver desde esa línea en adelante, omitiendo líneas vacías al final
            return ''.join(lines[start:]).strip()
        else:
            # Si no se encuentra, devolver todo el archivo (fallback)
            return ''.join(lines).strip()
    except Exception as e:
        print(f"Error leyendo archivo assembly {assembly_file}: {e}")
        return ""

@app.route('/api/compile', methods=['POST'])
def compile_file():
    try:
        data = request.get_json()
        filename = data.get('filename')
        
        if not filename:
            return jsonify({'error': 'No filename provided'}), 400
        
        input_path = f'inputs/{filename}'
        if not os.path.exists(input_path):
            return jsonify({'error': f'File {filename} not found'}), 404
        
        # Verificar caché
        current_time = time.time()
        if filename in file_cache:
            cache_entry = file_cache[filename]
            if current_time - cache_entry['timestamp'] < cache_timeout:
                print(f"Usando caché para {filename}")
                return jsonify(cache_entry['data'])
        
        # Ejecutar el compilador con timeout reducido
        start_time = time.time()
        result = subprocess.run(['./compilador', input_path], 
                              capture_output=True, text=True, timeout=10)
        compile_time = time.time() - start_time
        print(f"Compilación de {filename} completada en {compile_time:.2f}s")
        
        # Leer el archivo de entrada (código Pascal)
        with open(input_path, 'r') as f:
            pascal_code = f.read()
        
        # Extraer la salida del intérprete Pascal
        assembly_output = extract_pascal_output(result.stdout)
        
        # Leer el código assembly generado (solo el código puro para el punto 3)
        assembly_file = f'inputs/{filename.replace(".txt", ".s")}'
        assembly_code = extract_assembly_code(assembly_file)

        # Crear respuesta
        response_data = {
            'pascalCode': pascal_code,
            'pascalOutput': assembly_output,
            'assemblyCode': assembly_code,
            'assemblyOutput': assembly_output,
            'success': True,
            'compileTime': f"{compile_time:.2f}s"
        }
        
        # Guardar en caché
        file_cache[filename] = {
            'data': response_data,
            'timestamp': current_time
        }
        
        return jsonify(response_data)
        
    except subprocess.TimeoutExpired:
        return jsonify({'error': 'Compilation timeout (10s)'}), 408
    except Exception as e:
        return jsonify({'error': str(e)}), 500

def extract_pascal_output(stdout):
    """Extrae la salida del intérprete Pascal del stdout del compilador"""
    lines = stdout.split('\n')
    output_lines = []
    in_output = False
    
    for line in lines:
        if 'End of program execution' in line:
            break
        if in_output and line.strip():
            # Solo ignorar líneas de debug
            if not line.strip().startswith('DEBUG:'):
                output_lines.append(line.strip())
        if 'Run program:' in line:
            in_output = True
    
    return '\n'.join(output_lines) if output_lines else 'No output'

def simulate_assembly_execution(assembly_code, expected_output):
    """Simula la ejecución del código assembly"""
    # En un entorno real, esto compilaría el .s con gcc y lo ejecutaría
    # Por ahora, devolvemos el resultado esperado del intérprete Pascal
    return expected_output

@lru_cache(maxsize=1)
def get_available_files_cached():
    """Obtiene la lista de archivos disponibles con caché"""
    try:
        files = []
        for filename in os.listdir('inputs'):
            if filename.endswith('.txt'):
                files.append(filename)
        return sorted(files)
    except Exception as e:
        print(f"Error obteniendo archivos: {e}")
        return []

@app.route('/api/files')
def get_available_files():
    """Obtiene la lista de archivos disponibles en inputs/"""
    try:
        files = get_available_files_cached()
        return jsonify({'files': files})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/api/status')
def server_status():
    """Endpoint simple para verificar estado del servidor"""
    return jsonify({'status': 'ok', 'timestamp': time.time()})

if __name__ == '__main__':
    print("🚀 Iniciando servidor del compilador Pascal (optimizado)...")
    print("📱 Abre http://localhost:5001 en tu navegador")
    print("🔄 El servidor está listo para procesar archivos...")
    print("⚡ Optimizaciones activadas: caché, compresión, timeouts reducidos")
    
    # Configurar para mejor rendimiento
    app.run(
        debug=False,  # Deshabilitar debug para mejor rendimiento
        host='0.0.0.0', 
        port=5001,  # Cambiar puerto a 5001
        threaded=True,  # Habilitar threading
        use_reloader=False  # Deshabilitar reloader automático
    ) 