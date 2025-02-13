# 🚀 Controle de Joystick e Display com RP2040


## 📌 Descrição do Projeto
Este projeto implementa o controle de um **joystick analógico** para ajustar a intensidade de **LEDs RGB** utilizando PWM e exibir a posição do cursor em um **display SSD1306** via I2C. 

Ele foi desenvolvido para a placa **BitDogLab** com o microcontrolador **RP2040**, proporcionando uma experiência interativa e visualmente intuitiva. 🎮✨

## 🎯 Funcionalidades Implementadas
✅ **Leitura do Joystick**: Captura dos valores analógicos dos eixos X e Y.
✅ **Controle de LEDs RGB via PWM**:
   - 🔵 LED Azul: Varia sua intensidade com base no eixo **X**.
   - 🔴 LED Vermelho: Varia sua intensidade com base no eixo **Y**.
✅ **Exibição Gráfica no Display SSD1306**:
   - 🖥️ Mostra um **quadrado de 8x8 pixels** cuja posição reflete o movimento do joystick.
   - 🎨 Alterna entre estilos de **borda** ao pressionar o botão do joystick.
✅ **Interação por Botões**:
   - 🎛️ O botão do **joystick** alterna o LED **Verde** e muda a borda do display.
   - 🕹️ O botão **A** ativa/desativa os LEDs RGB controlados por PWM.
✅ **Uso de Interrupções (IRQ) e Debounce** para evitar leituras falsas dos botões.

## 🛠 Componentes Utilizados
📟 **Microcontrolador**: RP2040  
🔌 **Placa**: BitDogLab  
🎮 **Joystick**: GPIOs 26 (X) e 27 (Y)  
💡 **LED RGB**: GPIO 11 (Verde), 12 (Azul), 13 (Vermelho)  
🔘 **Botão do Joystick**: GPIO 22  
🎛 **Botão A**: GPIO 5  
📟 **Display OLED SSD1306**: I2C (GPIO 14 - SDA, GPIO 15 - SCL)  


## 🚀 Como Compilar e Executar
1️⃣ **🔧 Configurar o ambiente:**
   - Instale o SDK do Raspberry Pi Pico.
   - Configure o CMake e a toolchain do Pico.
   - Clone o repositório e navegue até a pasta do projeto.

2️⃣ **💻 Compilar e carregar o código:**

3️⃣ **🔍 Abrir o Serial Monitor:**
   - Use o Serial Monitor VS Code para monitorar a comunicação.
4️⃣ Execute o programa e observe o comportamento do **joystick, LEDs e display**.

## 🎥 Demonstração
📌 Assista ao vídeo de demonstração do projeto no link:  
[🎬 Google Drive - [Adicionar Link Aqui](https://drive.google.com/file/d/1uBfmNY8swe8t0qo1fZozgNtTrl2L5VPO/view?usp=sharing)]

## 👨‍💻 Autor
- **Thiago De Andrade Barros Machado** - [[GitHub](https://github.com/ThiagoMachado2)]


---
💡 _Este projeto foi desenvolvido como parte de um estudo sobre ADC, PWM e comunicação I2C com RP2040._
