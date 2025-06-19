# CNC HMI - CrowPanel

Interface homem-máquina para controle de máquina CNC baseada em ESP32, com tela capacitiva CrowPanel.

## Recursos
- Comunicação via ModBus RTU
- Gerenciamento de tela via SPI
- Sistema multitarefa usando FreeRTOS
- Drivers próprios para interface homem-máquina

## Como compilar
- Requisitos: ESP-IDF, VSCode, Python 3.10
- Instruções: `idf.py build && idf.py flash`

## Status: Em desenvolvimento
