
import time
import pygame
import serial

PORTA = 'COM5'
VELOCIDADE = 115200  # deve ser igual ao Serial.begin() do HoraDoRush.ino

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    raise RuntimeError(
        'Controle Xbox nao encontrado. Conecte-o e tente novamente.'
    )

controle = pygame.joystick.Joystick(0)
controle.init()

arduino = serial.Serial(PORTA, VELOCIDADE, timeout=0.05)
time.sleep(2)  # Arduino Uno reinicia ao abrir a porta serial

print('HORA DO RUSH - 6 NIVEIS (6x6)')
print('Direcional: movimentar carro')
print('A: selecionar / proximo nivel')
print('X: proximo carro')
print('Y: reiniciar nivel')
print('Ctrl+C: sair')

commands = {
    (1, 0): b'D',
    (-1, 0): b'E',
    (0, 1): b'W',
    (0, -1): b'S'
}

try:
    while True:
        for evento in pygame.event.get():

            if evento.type == pygame.JOYHATMOTION:
                horizontal, vertical = evento.value
                comando = commands.get((horizontal, vertical))

                if comando:
                    arduino.write(comando)

            elif evento.type == pygame.JOYBUTTONDOWN:

                if evento.button == 0:  # A: selecionar / avancar
                    arduino.write(b'A')

                elif evento.button == 2:  # X: proximo carro
                    arduino.write(b'X')
                    print('Proximo carro')

                elif evento.button == 3:  # Y: reiniciar nivel
                    arduino.write(b'Y')
                    print('Reiniciando nivel')

        while arduino.in_waiting:
            mensagem = arduino.readline().decode(
                'utf-8', errors='replace'
            ).strip()

            # Linhas com "@" sao do protocolo do site (Modo Arduino).
            if mensagem and not mensagem.startswith('@'):
                print(mensagem)

        time.sleep(0.01)

except KeyboardInterrupt:
    print('\nJogo encerrado.')

finally:
    arduino.close()
    pygame.quit()
