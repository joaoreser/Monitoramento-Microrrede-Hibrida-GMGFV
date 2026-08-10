#!/bin/bash

# Desativar suspensão e descanso de tela
xset s noblank
xset s off
xset -dpms

# Aguardar o Grafana iniciar completamente na porta 3000
while ! curl -s -f http://localhost:3000 > /dev/null; do
    sleep 2
done

# Disparar Chromium em tela cheia (Kiosk Mode)
/usr/bin/chromium-browser --kiosk --no-sandbox --use-gl=swiftshader --disable-dev-shm-usage http://localhost:3000