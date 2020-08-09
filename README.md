# GRASP-Flexible-Job-Shop-Problem
 Implementa a meta-heuristica GRASP para resolver o problema Flexible-Job-Shop-Problem. Ao finalizar a busca, renderiza um gráfico de gantt representando a melhor solução encontrada.

![GanttChartExemple](GanttChartExemple.png)

## Dependencias

glew e SDL2 

Certifique-se de ter as bibliotecas de desenvolvimento do glew e SDL2. Caso não tenha, elas podem ser obtidas, no Arch-linux por exemplo, instalando os pacotes

'''
sudo pacman -S lib32-sdl2
sudo pacman -S lib32-glew
'''

Os nomes dos pacotes podem ser um pouco diferentes para outras distribuições.

Se necessário altere as variaveis 'INCLUDE' e 'LIBS' no makefile para referir as pastas contendo os *headers*  e bibliotecas compartilhadas respectivamente
'''
INCLUDE = /usr/include
LIB = /usr/lib32
'''
Compile o projeto com o make.

## Uso

./exec mt06 | mt10 | mt20
