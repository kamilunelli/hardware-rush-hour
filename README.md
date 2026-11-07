# Hora do Rush — Arduino + Matriz LED RGB 8×8 (tabuleiro 6×6)

Projeto de implementação física do jogo **Hora do Rush (Rush Hour)**, utilizando um Arduino Uno, uma matriz de LEDs RGB 8×8 e um controle Xbox conectado ao computador.

O objetivo é retirar o carro vermelho de um estacionamento, movimentando os outros veículos para liberar sua passagem. Cada veículo só pode se mover na direção correspondente à sua orientação, respeitando os limites do tabuleiro e a posição dos demais carros.

O projeto possui **seis níveis** (um tutorial, três fáceis e dois "desafios" um pouco mais difíceis), com progressão após a conclusão de cada fase.

Ele também pode ser jogado pelo **Modo Arduino** do site [web-rush-hour](https://github.com/kamilunelli/web-rush-hour): o site conecta ao Arduino pela USB (Web Serial), lê o controle Xbox, espelha o tabuleiro físico na tela e executa a solução ótima do algoritmo **A\*** movendo os carros de verdade na matriz (seção 16).

## 1. Sobre o projeto

O jogo utiliza uma matriz de 64 LEDs para representar visualmente um tabuleiro de 6×6 posições.

Como a matriz possui 8 linhas e 8 colunas, os LEDs externos são utilizados para criar uma borda ao redor do tabuleiro.

A borda é branca, enquanto a saída do carro vermelho é identificada por um LED verde na lateral direita.

Cada veículo possui uma cor diferente para facilitar sua identificação.

### Funcionalidades

- Tabuleiro jogável de 6×6 posições.

- Seis níveis com dificuldade crescente, todos validados por busca exaustiva.

- Movimentação de carros horizontais e verticais.

- Identificação dos veículos por cores.

- Seleção de veículos utilizando um controle Xbox.

- Verificação de colisões entre veículos.

- Impedimento de movimentos fora do tabuleiro.

- Contagem de movimentos realizados (andar várias casas seguidas com o mesmo carro conta como 1 movimento, como no jogo web).

- Contagem de vitórias desde que o Arduino foi ligado.

- Identificação da vitória ao liberar o carro vermelho.

- Progressão para o próximo nível.

- Reinício do nível atual.

- Animação visual ao concluir uma fase.

## 2. Tecnologias utilizadas

| Tecnologia | Finalidade |

|---|---|

| Arduino Uno | Controlar a matriz e executar as regras do jogo |

| C++ / Arduino | Implementar a lógica dos níveis, veículos e LEDs |

| Python 3 | Receber os comandos do controle Xbox |

| Pygame | Detectar os botões e o direcional do controle |

| PySerial | Enviar comandos do computador ao Arduino |

| Adafruit NeoPixel | Controlar os LEDs RGB da matriz |

| Arduino IDE | Compilar e gravar o código no Arduino |

| VS Code | Editar os arquivos e gerenciar o Git |

## 3. Componentes necessários

Para montar e executar o projeto, são necessários:

- 1 Arduino Uno ou placa compatível.

- 1 matriz LED RGB 8×8 compatível com WS2812B.

- 1 cabo USB de dados para conectar o Arduino ao computador.

- Fios jumper para realizar as conexões.

- 1 controle Xbox compatível com o computador.

- 1 computador com Windows.

- Protoboard, se necessária para organizar as conexões.

**Atenção:** este projeto utiliza uma matriz RGB WS2812B. Ela é diferente de uma matriz monocromática MAX7219. Os códigos e as ligações elétricas não são intercambiáveis.

## 4. Estrutura do projeto

A organização esperada é:

```text

HoraDoRush/

│

├── HoraDoRush.ino

│

├── python/

│   └── controle.py

│

└── README.md

```

### Descrição dos arquivos

**`HoraDoRush.ino`**

É o programa principal executado pelo Arduino.

Responsável por:

- Inicializar a matriz LED.

- Carregar os níveis.

- Armazenar a posição dos veículos.

- Processar comandos recebidos pela porta serial.

- Verificar movimentos permitidos.

- Detectar colisões.

- Identificar quando o jogador vence.

- Desenhar o tabuleiro e os carros.

**`python/controle.py`**

É o programa executado no computador.

Responsável por:

- Detectar o controle Xbox.

- Identificar os botões pressionados.

- Capturar os movimentos do direcional.

- Converter essas ações em comandos.

- Enviar os comandos ao Arduino pela porta serial.

- Exibir mensagens do Arduino no terminal.

**`README.md`**

Contém a documentação para instalação, montagem e execução do projeto.

## 5. Download e preparação do projeto

Para facilitar o primeiro acesso, recomenda-se baixar o projeto em **ZIP** diretamente do GitHub. Não é necessário instalar o Git nem possuir conta no GitHub para baixar um repositório público.

### 5.1. Baixar os arquivos

1. Acesse a página do repositório no GitHub.
2. Clique no botão verde **Code**, acima da lista de arquivos.
3. Selecione **Download ZIP**.
4. Aguarde o download, normalmente salvo na pasta **Downloads**.

### 5.2. Extrair o ZIP

1. Abra o **Explorador de Arquivos** do Windows e acesse **Downloads**.
2. Clique com o botão direito no ZIP e escolha **Extrair Tudo...**.
3. Escolha uma pasta de destino, como **Documentos**, e clique em **Extrair**.
4. Abra a pasta extraída e localize os arquivos do jogo.

**Importante:** não execute ou edite o projeto diretamente dentro do ZIP. A pasta baixada pode ter um sufixo como `-main`, e os arquivos podem estar em uma subpasta.

### 5.3. Identificar os arquivos

A estrutura principal esperada é:

```text
HoraDoRush/
├── HoraDoRush.ino
├── python/
│   └── controle.py
└── README.md
```

- **`HoraDoRush.ino`:** código gravado no Arduino, responsável pelos níveis, regras e LEDs.
- **`python/controle.py`:** recebe os comandos do controle Xbox e os envia ao Arduino.
- **`README.md`:** instruções de montagem, instalação e execução.

Mantenha os arquivos em suas pastas. A pasta que contém `HoraDoRush.ino` deve conservar esse nome para ser aberta como sketch no Arduino IDE.

### 5.4. Abrir no VS Code

1. Abra o VS Code e escolha **File → Open Folder** (**Arquivo → Abrir Pasta**).
2. Selecione a pasta extraída do projeto.
3. Clique em **Selecionar Pasta** e confira os arquivos no painel lateral.

O VS Code será utilizado para editar os arquivos e executar o Python. Para compilar e gravar o código na placa, use o Arduino IDE, conforme a seção 8.

## 6. Instalação das ferramentas e bibliotecas

Essas instalações normalmente precisam ser feitas apenas uma vez por computador.

### 6.1. Instalar o Arduino IDE

1. Acesse o [site oficial do Arduino IDE](https://www.arduino.cc/en/software).
2. Baixe o **Arduino IDE 2** para Windows.
3. Execute o instalador e siga as instruções.
4. Aceite a instalação dos drivers, se solicitado.
5. Abra o Arduino IDE para confirmar que a instalação funcionou.

O Arduino IDE será utilizado para verificar (compilar) o código e enviá-lo ao Arduino Uno.

### 6.2. Instalar a biblioteca Adafruit NeoPixel

1. Abra o Arduino IDE.
2. Clique no **Gerenciador de Bibliotecas** na barra lateral.
3. Pesquise por `Adafruit NeoPixel`.
4. Selecione a biblioteca da **Adafruit** e clique em **Install / Instalar**.
5. Aguarde a conclusão.

Essa biblioteca é necessária porque o código contém:

```cpp
#include <Adafruit_NeoPixel.h>
```

Se aparecer `Adafruit_NeoPixel.h: No such file or directory` ao compilar, confira a instalação da biblioteca.

### 6.3. Instalar o Python 3

1. Acesse o [site oficial do Python](https://www.python.org/downloads/).
2. Baixe o instalador do **Python 3** para Windows.
3. Execute o instalador. Se disponível, marque **Add Python to PATH**.
4. Conclua a instalação.
5. No VS Code, abra **Terminal → New Terminal** e execute:

```powershell
py --version
```

O terminal deve exibir a versão instalada, por exemplo `Python 3.13.5`. A versão pode ser diferente. Se `py` não for reconhecido, confira a instalação do Python e do Python Launcher.

### 6.4. Instalar as bibliotecas Python

O programa utiliza **Pygame** para ler o controle Xbox e **PySerial** para se comunicar com o Arduino.

No terminal do VS Code, execute:

```powershell
py -m pip install pygame pyserial
```

Aguarde a conclusão. Para verificar:

```powershell
py -m pip show pygame
py -m pip show pyserial
```

Se o terminal mostrar informações sobre cada pacote, a instalação foi encontrada.

**Observação:** a biblioteca é instalada como `pyserial`, mas no código Python é importada como `serial`.

### 6.5. Conferência antes da montagem

- [ ] O ZIP foi baixado e extraído.
- [ ] A pasta do projeto abre no VS Code.
- [ ] O Arduino IDE está instalado.
- [ ] A biblioteca Adafruit NeoPixel está instalada.
- [ ] O Python responde a `py --version`.
- [ ] `pygame` e `pyserial` estão instalados.
- [ ] Os arquivos `HoraDoRush.ino` e `python/controle.py` foram localizados.

Com tudo pronto, siga para a conexão da matriz e a gravação do programa.

## 7. Conexão da matriz LED ao Arduino

Antes de conectar os componentes, desconecte o Arduino da alimentação.

As ligações utilizadas são:

| Matriz RGB WS2812B | Arduino Uno |

|---|---|

| DIN | D6 |

| V+ / 5V | 5V |

| V− / GND | GND |

### Procedimento

1. Identifique os terminais da matriz LED.

2. Conecte o terminal `DIN` ao pino digital `D6` do Arduino.

3. Conecte o terminal `V−` ao `GND` do Arduino.

4. Conecte o terminal `V+` à alimentação de 5 V apropriada.

5. Verifique se os fios estão firmes e não existem curtos-circuitos.

6. Conecte o Arduino ao computador pelo cabo USB.

**Cuidados com a alimentação:** a matriz possui 64 LEDs RGB e pode consumir corrente elevada quando muitos LEDs estão acesos. A ligação ao pino 5V do Arduino deve ser utilizada somente em testes de consumo reduzido, com brilho muito baixo. Para uma montagem definitiva, utilize uma fonte regulada de 5 V dimensionada para a matriz, com o GND da fonte conectado ao GND do Arduino.

Não conecte uma fonte externa sem verificar sua polaridade e tensão.

## 8. Abrir o projeto no Arduino IDE

1. Localize a pasta do projeto.

2. Abra a pasta `HoraDoRush`.

3. Dê dois cliques no arquivo `HoraDoRush.ino`.

4. Aguarde a abertura no Arduino IDE.

5. Conecte o Arduino ao computador.

No Arduino IDE, selecione:

**Ferramentas → Placa → Arduino AVR Boards → Arduino Uno**

Em seguida, selecione a porta de comunicação em:

**Ferramentas → Porta**

No Windows, ela normalmente aparece como `COM3`, `COM5` ou outro número.

O número depende do computador e da conexão utilizada.

### 8.1. Compilar o código

Clique no botão **Verificar (✓)**.

Se a compilação funcionar, o Arduino IDE exibirá informações sobre o espaço de armazenamento e a memória utilizados.

Isso significa que o código foi compilado, mas ainda não necessariamente enviado à placa.

### 8.2. Enviar o código ao Arduino

Clique em **Upload (→)**.

Aguarde até que o envio seja concluído.

Após o upload, o Arduino passa a executar o programa gravado nele.

Não é necessário manter o Arduino IDE aberto para o jogo funcionar.

## 9. Configurar a porta serial no Python

Abra o arquivo:

```text

python/controle.py

```

Procure a linha:

```python

PORTA = 'COM5'

```

Substitua `COM5` pela porta em que o Arduino está conectado, caso seja diferente.

Por exemplo:

```python

PORTA = 'COM3'

```

**Importante:** o Arduino IDE e o Python não devem tentar utilizar a mesma porta serial simultaneamente.

Após fazer o upload, feche o Monitor Serial e, preferencialmente, o Arduino IDE antes de iniciar o programa Python.

## 10. Conectar o controle Xbox

O controle Xbox deve estar conectado ao computador por USB.

Para verificar se o Windows reconhece o controle:

1. Conecte ou pareie o controle.

2. Pressione `Win + R`.

3. Digite `joy.cpl`.

4. Pressione Enter.

5. Verifique se o controle aparece na lista.

6. Selecione o controle e teste seus botões em **Propriedades**, quando disponível.

O programa Python utiliza o Pygame para identificar os comandos enviados pelo controle.

## 11. Executar o jogo

Com o código já gravado no Arduino e o controle Xbox conectado, siga as etapas abaixo.

### 11.1. Abrir o projeto no VS Code

1. Abra o VS Code.

2. Clique em **File → Open Folder**.

3. Selecione a pasta do projeto.

4. Abra o terminal integrado em **Terminal → New Terminal**.

### 11.2. Acessar a pasta Python

No terminal, execute:

```bash

cd python

```

Caso o terminal já esteja nessa pasta, não é necessário repetir o comando.

### 11.3. Iniciar o controle

Execute:

```bash

py controle.py

```

O terminal deverá exibir mensagens semelhantes a:

```text

HORA DO RUSH - 6 NIVEIS (6x6)

Direcional: movimentar carro

A: selecionar / proximo nivel

X: proximo carro

Y: reiniciar nivel

Ctrl+C: sair

```

A matriz exibirá o nível atual.

### 11.4. Encerrar o programa

Para encerrar a comunicação com o controle, pressione:

```text

Ctrl + C

```

O programa Python será finalizado e a porta serial será liberada.

## 12. Controles do jogo

| Botão | Ação |

|---|---|

| Direcional | Movimentar o carro selecionado |

| A | Selecionar o carro ou avançar de nível após vencer |

| X | Selecionar o próximo carro |

| Y | Reiniciar o nível atual |

| B | Não utilizado |

### Como jogar

1. Inicie o programa Python.

2. Observe os veículos apresentados na matriz.

3. Pressione **A** para selecionar o carro inicial.

4. Utilize o direcional para movimentá-lo.

5. Pressione **X** para passar ao próximo carro.

6. Continue movimentando os veículos para liberar o caminho.

7. Leve o carro vermelho até a saída verde.

8. Quando vencer, pressione **A** para avançar ao próximo nível.

O botão **X não reinicia a partida**. Ele altera apenas o veículo selecionado, preservando as posições dos demais carros.

O botão **Y reinicia o nível atual**, restaurando as posições iniciais e zerando o contador de movimentos.

## 13. Funcionamento do tabuleiro

A matriz física possui 8 linhas e 8 colunas.

Entretanto, a área utilizada para movimentação dos carros possui 6 linhas e 6 colunas.

A linha e a coluna externas formam a borda do tabuleiro.

### Identificação visual

- **Borda branca:** delimita o tabuleiro.

- **Saída verde:** indica o local de saída do carro vermelho.

- **Carro vermelho:** veículo que deve escapar.

- **Demais cores:** representam veículos que precisam ser movimentados.

- **Carro piscando:** indica o veículo em foco ou selecionado.

A saída está localizada na lateral direita, alinhada à linha do carro vermelho.

O programa utiliza a função:

```cpp

int pixel(byte row, byte col) {

    return row * 8 + col;

}

```

Esse mapeamento considera que os LEDs são organizados por linhas, da esquerda para a direita, começando pelo LED 0 no canto superior esquerdo.

Se a matriz utilizada possuir outra disposição física, será necessário adaptar o mapeamento.

## 14. Regras de movimentação

Cada carro possui os seguintes atributos:

```cpp

struct Car {

    byte row;

    byte col;

    byte len;

    bool horizontal;

    byte color;

};

```

Esses atributos representam:

- `row`: linha inicial do veículo.

- `col`: coluna inicial do veículo.

- `len`: comprimento em casas.

- `horizontal`: orientação horizontal ou vertical.

- `color`: identificador da cor.

### Restrições

O programa impede que:

1. Um veículo ultrapasse os limites do tabuleiro.

2. Dois veículos ocupem a mesma posição.

3. Um veículo horizontal se movimente verticalmente.

4. Um veículo vertical se movimente horizontalmente.

Cada comando válido movimenta o veículo uma casa e incrementa o contador de movimentos.

Movimentos bloqueados não devem alterar a posição do veículo.

## 15. Níveis

O projeto contém seis níveis, pensados para crianças: os quatro primeiros são tranquilos e só os dois últimos são um pouco mais difíceis.

| Nível | Dificuldade | Veículos | Solução ótima |
|---|---|---|---|
| 1 | Tutorial | 4 | 2 movimentos |
| 2 | Fácil | 5 | 3 movimentos |
| 3 | Fácil | 5 | 4 movimentos |
| 4 | Fácil | 6 | 5 movimentos |
| 5 | Desafio | 7 | 7 movimentos |
| 6 | Desafio | 7 | 8 movimentos |

A "solução ótima" usa a mesma regra do jogo web: deslizar um carro várias casas seguidas conta como 1 movimento. Os mapas foram gerados por computador e o ótimo de cada um foi conferido por busca exaustiva (BFS) e pelo solver A\* do web-rush-hour.

Cada nível tem no máximo 8 veículos, nunca sobrepostos, um de cada cor (vermelho, azul, verde, amarelo, roxo, ciano, rosa e laranja), para que nenhum carro se confunda com outro na matriz.

Os níveis são definidos no código Arduino, por meio da estrutura `Level`, gravada na memória de programa (`PROGMEM`).

Cada nível informa a quantidade de veículos, a solução ótima e as posições iniciais.

A variável `levelIndex` identifica o nível atual.

Ao concluir um nível, o Arduino aguarda o jogador pressionar **A** para carregar o próximo.

Após o sexto nível, o programa informa que todos os níveis foram concluídos.

Para jogar novamente, pressione **Y** para reiniciar o sexto nível, escolha um nível no site ou reinicie o Arduino para retornar ao primeiro.

## 16. Comunicação entre Python e Arduino

O controle Xbox não está conectado diretamente ao Arduino.

A comunicação ocorre da seguinte maneira:

```text

Controle Xbox

      |

      v

Computador Windows

      |

      v

Python + Pygame

      |

      v

PySerial (porta COM)

      |

      v

Arduino Uno

      |

      v

Matriz LED RGB 8x8

```

O Python identifica os botões pressionados e envia caracteres pela porta serial.

O Arduino recebe esses caracteres e executa a ação correspondente.

### Comandos enviados

| Caractere | Significado |

|---|---|

| A | Selecionar / avançar |

| X | Próximo carro |

| Y | Reiniciar nível |

| D | Direita |

| E | Esquerda |

| W | Cima |

| S | Baixo |

| L1 a L6 | Carregar o nível indicado |

| C0 a C7 | Selecionar o carro de índice indicado (usado pelo Resolver) |

| Q | Cancelar o nível (tabuleiro vazio) |

| ? | Pedir o estado atual |

A comunicação utiliza uma velocidade de **115200 baud** (o `controle.py` e o site já usam esse valor).

### Mensagens enviadas pelo Arduino

Além das mensagens de texto (`NIVEL 1/6`, `VITORIA!` etc.), o Arduino envia uma linha de estado sempre que algo muda. O site usa essa linha para espelhar o tabuleiro:

```text
@HELLO 2 6
@S 3 5 8 1 1 2 1 202H0 322H1 252V2 122V3 003H4 413H5 033V6 513H7
```

`@S <nível> <movimentos> <ótimo> <vitórias> <status> <carro atual> <selecionado> <carros...>`. O status é 0 = sem nível, 1 = jogando, 2 = venceu e 3 = todos concluídos. Cada carro tem 5 caracteres: linha, coluna, tamanho, `H`/`V` e cor. O `controle.py` ignora as linhas que começam com `@`.

### Jogar pelo site (Modo Arduino)

Em vez do `controle.py`, é possível usar o site [web-rush-hour](https://github.com/kamilunelli/web-rush-hour):

```text
Controle Xbox ──> Navegador (Chrome/Edge) ──Web Serial──> Arduino Uno ──> Matriz LED
                        │
                        └── /solve (backend A*) ── solução ótima
```

1. Grave o `HoraDoRush.ino` no Arduino e **feche o Arduino IDE e o `controle.py`**, porque só um programa pode usar a porta COM por vez.
2. Abra o site no **Google Chrome** ou no **Microsoft Edge** do computador. A Web Serial não funciona no Firefox nem no celular.
3. Clique em **MODO ARDUINO** e depois em **CONECTAR ARDUINO**, e escolha a porta do Arduino.
4. Aperte qualquer botão do controle Xbox para o navegador reconhecê-lo. O teclado também funciona: setas, `A`/Enter, `X` e `Y`.

Na tela é possível:

- **Escolher o nível** (1 a 6).
- **Reiniciar** e **Cancelar** o nível. Cancelar também interrompe o Resolver.
- **Resolver**: o backend calcula a solução ótima (A\*) **a partir da posição atual** e o site executa os movimentos no Arduino, um carro de cada vez.
- **Dica**: executa só o próximo movimento da solução ótima.
- Ver o **espelho do tabuleiro físico**, com o carro em foco piscando, além do histórico do que foi movido, dos movimentos, do ótimo e das vitórias.

O botão Resolver precisa do backend do site no ar. Sem internet, rode o web-rush-hour localmente com `docker compose up` e abra `http://localhost:3000`.

## 17. Problemas comuns e soluções

### Erro: `Acesso negado` na COM5

Exemplo:

```text

Error: cannot open port \\\.\COM5: Acesso negado.

```

**Causa provável:** outro programa está utilizando a porta serial.

**Solução:**

1. Encerre o `controle.py` com `Ctrl + C`.

2. Feche outros programas Python que estejam usando a porta.

3. Feche o Monitor Serial e o Plotter Serial do Arduino IDE.

4. Tente realizar o upload novamente.

5. Se necessário, desconecte e reconecte o Arduino.

### Erro: `No module named pygame`

Instale a biblioteca:

```bash

py -m pip install pygame

```

### Erro: `No module named serial`

Instale o PySerial:

```bash

py -m pip install pyserial

```

### Erro: `Adafruit_NeoPixel.h: No such file or directory`

A biblioteca Adafruit NeoPixel não está instalada ou não foi encontrada.

Abra o Gerenciador de Bibliotecas do Arduino IDE e instale **Adafruit NeoPixel**.

### O controle Xbox não é encontrado

1. Verifique a conexão USB.

2. Teste o controle com `joy.cpl`.

3. Feche o programa Python.

4. Conecte novamente o controle.

5. Execute `py controle.py` outra vez.

### A matriz acende, mas os LEDs aparecem incorretamente

Verifique:

- Se `DIN` está conectado ao pino `D6`.

- Se a alimentação está correta.

- Se o GND está conectado.

- Se os contatos estão firmes.

- Se o mapeamento dos LEDs corresponde ao modelo utilizado.

### O código compila, mas não faz upload

Verifique a placa e a porta selecionadas no Arduino IDE.

Também confirme que o programa Python não está utilizando a porta serial.

### O botão X reinicia a partida

Verifique se o Arduino recebeu a versão atualizada do código.

Na função responsável pelos comandos, `X` deve selecionar o próximo veículo, enquanto `Y` deve reiniciar o nível.

Modificar somente o `controle.py` não é suficiente quando a lógica antiga continua gravada no Arduino.

## 18. Cuidados com o hardware

A matriz RGB pode consumir uma quantidade significativa de energia, principalmente com muitos LEDs brancos acesos.

Por segurança:

- Utilize brilho reduzido nos testes.

- Evite ligar todos os LEDs em intensidade máxima.

- Verifique a polaridade antes de conectar a alimentação.

- Não altere as conexões com o circuito energizado.

- Utilize fios e contatos firmes.

- Evite curtos-circuitos entre os terminais.

- Utilize alimentação externa adequada quando necessário.

No código, o brilho é configurado por:

```cpp

leds.setBrightness(20);

```

O valor reduzido ajuda a limitar o consumo durante os testes, mas não substitui o dimensionamento adequado da fonte de alimentação.

## 19. Desenvolvimento e versionamento

O projeto pode ser desenvolvido no Arduino IDE ou no VS Code.

O Arduino IDE é utilizado principalmente para verificar e gravar o código na placa.

O VS Code pode ser utilizado para editar os arquivos, organizar a estrutura e realizar commits no GitHub.

### Enviar alterações pelo VS Code

1. Abra a pasta do repositório Git no VS Code.

2. Modifique os arquivos necessários.

3. Abra **Source Control**, utilizando `Ctrl + Shift + G`.

4. Confira os arquivos modificados.

5. Clique em **Stage Changes**.

6. Escreva uma mensagem de commit.

7. Clique em **Commit**.

8. Clique em **Push** ou **Sync Changes** para enviar ao GitHub.

Exemplo de mensagem:

```text

Implementa jogo Hora do Rush com 5 niveis e controle Xbox

```

## 20. Limitações e melhorias futuras

O projeto atualmente depende de um computador para interpretar os comandos do controle Xbox.

O Arduino executa a lógica do jogo e controla a matriz, mas não realiza diretamente a comunicação com o controle.

Entre as possíveis melhorias futuras estão:

- Implementar um sistema de pontuação.

- Armazenar o progresso do jogador.

- Utilizar uma fonte de alimentação dedicada para a matriz.

- Implementar um controle conectado diretamente ao microcontrolador, com hardware compatível.

## 21. Resumo de execução rápida

Para quem já instalou todos os programas e bibliotecas:

1. Conecte a matriz ao Arduino.

2. Conecte o Arduino ao computador.

3. Abra `HoraDoRush.ino` no Arduino IDE.

4. Selecione Arduino Uno e a porta COM correta.

5. Compile e faça o upload.

6. Feche o Arduino IDE.

7. Conecte o controle Xbox.

8. Abra a pasta `python` no terminal.

9. Execute:

```bash

py controle.py

```

10. Utilize **A**, **X**, **Y** e o direcional para jogar.

---

**Projeto Hora do Rush — Arduino Uno + WS2812B + Python + Controle Xbox.**
