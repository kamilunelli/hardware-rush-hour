# 🚗 Hora do Rush — Jogo Físico com Arduino

O **Hora do Rush** é uma versão física do jogo de raciocínio lógico, desenvolvida com **Arduino Uno**, uma **matriz de LEDs RGB 8×8** e um **controle Xbox**.

O objetivo é movimentar os veículos até liberar o caminho para que o **carro vermelho** alcance a saída. O tabuleiro possui **6×6 posições** e seis níveis de dificuldade progressiva.

O jogo pode ser controlado diretamente pelo navegador, utilizando o site hospedado na Vercel.

## 🎮 Como jogar pelo navegador (Vercel)

**Acesse:** https://web-rush-hour.vercel.app

### O que você precisa

- Arduino Uno com o programa do jogo gravado (veja as instruções abaixo, caso ainda não esteja configurado);
- Matriz LED RGB 8×8 WS2812B conectada ao Arduino;
- Cabo USB de dados para conectar o Arduino ao computador;
- Computador com **Google Chrome** ou **Microsoft Edge**;
- Controle Xbox conectado ao computador (opcional: o teclado também pode ser utilizado).

### Passo a passo

1. Conecte a matriz LED ao Arduino e o Arduino ao computador pelo cabo USB.
2. Abra o site: **https://web-rush-hour.vercel.app**.
3. Clique em **MODO ARDUINO** e depois em **CONECTAR ARDUINO**.
4. Na janela do navegador, selecione a porta do Arduino (por exemplo, `COM5`) e autorize a conexão.
5. Escolha um dos seis níveis e comece a jogar usando o controle Xbox ou o teclado.

O tabuleiro físico será exibido também no navegador. É possível reiniciar a fase, visualizar os movimentos e utilizar as opções **Dica** e **Resolver** (esta última utiliza o algoritmo A* para encontrar a solução).

> **Importante:** feche o Monitor Serial do Arduino IDE e qualquer programa Python conectado ao Arduino antes de abrir a conexão pelo navegador. Apenas um programa pode utilizar a porta serial por vez. A conexão Web Serial requer um navegador compatível no computador; não funciona no Firefox nem no navegador do celular.

## 🕹️ Controles

| Controle Xbox | Função |
|---|---|
| Direcional | Escolher ou movimentar o veículo |
| **A** | Selecionar um veículo / avançar após vencer |
| **X** | Selecionar o próximo veículo |
| **Y** | Reiniciar a fase |

No navegador, também podem ser utilizadas as **setas do teclado**, **A/Enter**, **X** e **Y**.

O carro selecionado pisca na matriz. Veículos horizontais só se movimentam para os lados; veículos verticais só se movimentam para cima ou para baixo.

## 🔧 Configuração inicial do Arduino IDE (se necessário)

**Esta etapa só é necessária caso o Arduino ainda não tenha o programa gravado ou precise ser atualizado.** Depois disso, basta conectar a placa ao computador e utilizar o site.

### 1. Baixar o código

1. Acesse [este repositório](https://github.com/kamilunelli/hardware-rush-hour).
2. Clique em **Code → Download ZIP** e extraia os arquivos.
3. Localize o arquivo:

   `HoraDoRush/HoraDoRush/HoraDoRush.ino`

### 2. Instalar e preparar o Arduino IDE

1. Instale o [Arduino IDE](https://www.arduino.cc/en/software).
2. Abra o **Gerenciador de Bibliotecas** e instale a biblioteca **Adafruit NeoPixel**.
3. Abra o arquivo `HoraDoRush.ino`.
4. Conecte o Arduino ao computador por USB.
5. No menu **Ferramentas**, escolha **Placa → Arduino Uno** e selecione a **Porta COM** correspondente.
6. Clique em **Verificar (✓)** para compilar e em **Upload (→)** para gravar o programa.
7. Após o envio, feche o Monitor Serial e volte às instruções **Como jogar pelo navegador**.

### Ligações da matriz LED

| Matriz WS2812B | Arduino Uno |
|---|---|
| `DIN` | `D6` |
| `V− / GND` | `GND` |
| `V+ / 5V` | Alimentação regulada de `5V` |

**Atenção:** a matriz pode consumir mais corrente do que a porta USB ou o Arduino conseguem fornecer com segurança. Para utilização prolongada, use uma fonte de 5 V adequada à matriz, compartilhando o GND com o Arduino. Verifique a polaridade antes de energizar o circuito.

## 💻 Alternativa: jogar sem o site, usando Python

Se desejar utilizar apenas o controle Xbox e a matriz física, sem abrir o site, instale o [Python 3](https://www.python.org/downloads/) e as bibliotecas necessárias:

```powershell
py -m pip install pygame pyserial
```

No arquivo `HoraDoRush/python/controle.py`, confira a variável `PORTA` e ajuste para a porta COM do seu Arduino, caso necessário (por exemplo, `COM5`).

Abra um terminal na pasta `HoraDoRush/python` e execute:

```powershell
py controle.py
```

Para encerrar, pressione `Ctrl + C`.

> O modo Python e o modo navegador são **alternativos**: não execute os dois ao mesmo tempo, pois ambos usam a porta serial do Arduino.

## ❓ Problemas comuns

| Problema | O que fazer |
|---|---|
| O Arduino não aparece no navegador | Confira o cabo USB de dados, a porta COM e se está usando Chrome ou Edge. |
| A porta COM está ocupada | Feche o Monitor Serial, o Arduino IDE e o `controle.py`, se estiverem utilizando a porta. |
| A matriz não acende corretamente | Confira os fios `DIN`, `GND` e `5V`, a alimentação e os contatos. |
| O upload falha | Confira a placa e a porta selecionadas e feche programas que usam a porta COM. |
| Erro `Adafruit_NeoPixel.h` | Instale a biblioteca **Adafruit NeoPixel** no Arduino IDE. |
| O botão **Resolver** não funciona | Confira a conexão com a internet e a disponibilidade do serviço de resolução do site. |

## 🔗 Repositórios

- [Hardware — Arduino e controle Xbox](https://github.com/kamilunelli/hardware-rush-hour)
- [Aplicação web — Hora do Rush](https://github.com/kamilunelli/web-rush-hour)
- [Jogar na Vercel](https://web-rush-hour.vercel.app)

---

**Projeto Hora do Rush — Arduino Uno + Matriz RGB WS2812B + Controle Xbox + Web Serial.**
