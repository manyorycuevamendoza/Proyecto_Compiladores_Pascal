#!/bin/bash
echo "🔍 Buscando procesos en el puerto 5000..."

# Buscar procesos que usen el puerto 5000
PIDS=$(lsof -ti:5000)

if [ -z "$PIDS" ]; then
    echo "✅ No hay procesos usando el puerto 5000"
else
    echo "🔄 Encontrados procesos: $PIDS"
    echo "🗑️  Matando procesos..."
    
    for PID in $PIDS; do
        echo "   Matando proceso $PID"
        kill -9 $PID
    done
    
    echo "✅ Procesos eliminados"
fi

echo "🚀 Ahora puedes ejecutar el servidor sin problemas" 