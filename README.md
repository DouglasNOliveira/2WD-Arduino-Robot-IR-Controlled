**MIT License**

Copyright (c) 2025 Douglas Nascimento de Oliveira

---
# 🤖 Robô 2WD Controlado por Infravermelho (IR)

Este projeto implementa um **robô 2WD controlado por infravermelho**, utilizando um **controle remoto IR comum**, um **receptor IR**, uma **ponte H L298N com ENA/ENB ativos (PWM)** e um **Arduino**.

O projeto foi desenvolvido com foco em **arquitetura de controle**, respeitando as limitações do protocolo IR e aplicando soluções de software para obter um comportamento semelhante a um carrinho radiocontrolado.

---

## 🎮 Funcionalidades

- Controle remoto usando **controle remoto infravermelho**
- Controle contínuo enquanto o botão é pressionado
- Curvas não exigem mais de um botão
- Controle dos motores com **PWM (ENA / ENB ativos)**
- **Feedback visual** com LED RGB

---

## 🧠 Conceito de Controle

Controles IR **não permitem múltiplos botões simultâneos** nem envio contínuo de comandos como rádio.  
Para contornar isso, o projeto utiliza:

- **Repetição IR (`IR_REPEAT`)** como fluxo contínuo
- **Watchdog por tempo**:
  - enquanto sinais IR chegam → robô se move
  - quando o sinal para → robô para automaticamente
- Mapeamento direto de comandos:
  - Frente → anda
  - Ré → anda para trás
  - Esquerda → curva para frente e esquerda
  - Direita → curva para frente e direita

---

## 🔧 Hardware Utilizado

- Arduino Uno R3 (ou compatível)
- Ponte H **L298N**
- Chassi 2WD com dois motores DC
- Receptor Infravermelho KY-022
- Controle remoto infravermelho
- LED RGB (ou 3 LEDs individuais)
- Baterias 18650 (3,7V - 4,2V)

---

## 🔌 Pinagem

### Ponte H L298N (ENA / ENB ativos)

| Função | Pino Arduino |
|------|-------------|
| ENA | 10 (PWM) |
| IN1 | 9 |
| IN2 | 8 |
| IN3 | 7 |
| IN4 | 6 |
| ENB | 5 (PWM) |

> ⚠️ A ordem do construtor da biblioteca `L298NX2` é **crítica**:
```cpp
L298NX2 motores(ENA, IN1, IN2, ENB, IN3, IN4);
