#!/bin/bash

echo "🚀 Configurando entorno virtual para el compilador Pascal..."
echo "================================================"

# Verificar si python3-venv está instalado
if ! dpkg -l | grep -q python3-venv; then
    echo "📦 Instalando python3-venv..."
    sudo apt update
    sudo apt install -y python3-venv
fi

# Crear entorno virtual si no existe
if [ ! -d "venv" ]; then
    echo "🔧 Creando entorno virtual..."
    python3 -m venv venv
fi

# Activar entorno virtual
echo "🔧 Activando entorno virtual..."
source venv/bin/activate

# Instalar dependencias
echo "📦 Instalando dependencias..."
pip install -r requirements.txt

echo "✅ Entorno virtual configurado correctamente!"
echo "📝 Para activar el entorno virtual manualmente, ejecuta:"
echo "   source venv/bin/activate"
echo ""
echo "🚀 Para ejecutar el servidor:"
echo "   source venv/bin/activate && python server.py" 