# Projeto: Exibição de Números em Matriz 5x5 com Raspberry Pi Pico e Wokwi

Este projeto utiliza a **placa BigDotLab** e o **simulador Wokwi** para exibir números de 0 a 9 em uma matriz 5x5 de LEDs NeoPixel (WS2812). A interação é feita por meio de dois botões para incrementar e decrementar os números exibidos.

## 🚀 Funcionalidades

- Exibe números de 0 a 9 em uma matriz 5x5 de LEDs NeoPixel.
- Possui dois botões:
  - **Botão A:** Incrementa o número exibido.
  - **Botão B:** Decrementa o número exibido.
- Um LED vermelho pisca continuamente para indicar funcionamento.
- Debounce para evitar múltiplas leituras de botão em curto intervalo.
- Implementação em **C** com suporte ao **Pico SDK**.

## 🛠️ Tecnologias Utilizadas

- **Raspberry Pi Pico**
- **Linguagem C**
- **Pico SDK**
- **Simulador Wokwi**
- **BigDotLab**
- **Bibliotecas padrão do Pico (hardware/pio.h, hardware/clocks.h, pico/stdlib.h)**

## 📌 Estrutura do Código

O código é composto pelos seguintes módulos:

- **Configuração de hardware:** Inicialização dos botões, LED e matriz de LEDs.
- **Manipulação dos botões:** Uso de interrupções para capturar eventos de pressionamento.
- **Exibição dos números:** Mapeamento de cada número (0-9) para a matriz de LEDs.
- **Controle dos LEDs:** Atualização dos LEDs NeoPixel com os valores correspondentes.

## 🔧 Como Executar o Projeto

### 1️⃣ Requisitos

- **Placa BigDotLab (ou Raspberry Pi Pico real)**
- **Ambiente de Desenvolvimento com Pico SDK**
- **Simulador Wokwi (opcional)**

### 2️⃣ Clonando o Repositório

```sh
git clone [https://github.com/seu-usuario/seu-repositorio.git](https://github.com/camillerodriguees/embarcatech-interrupcoes.git)


