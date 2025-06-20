#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define LIN_DEVICE "/dev/ttyUSB0"

int main() {
    int lin_fd;
    struct termios tty;
    unsigned char lin_frame[8] = {0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
    
    // Abrir dispositivo UART (LIN)
    lin_fd = open(LIN_DEVICE, O_RDWR | O_NOCTTY);
    if (lin_fd < 0) {
        perror("Error al abrir dispositivo LIN");
        return 1;
    }
    
    // Configurar UART
    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(lin_fd, &tty) != 0) {
        perror("Error en tcgetattr");
        return 1;
    }
    
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    
    tty.c_cflag &= ~PARENB; // Sin paridad
    tty.c_cflag &= ~CSTOPB; // 1 bit de stop
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bits de datos
    
    if (tcsetattr(lin_fd, TCSANOW, &tty) != 0) {
        perror("Error en tcsetattr");
        return 1;
    }
    
    // Enviar trama LIN
    write(lin_fd, lin_frame, sizeof(lin_frame));
    
    // Leer respuesta
    unsigned char response[8];
    int n = read(lin_fd, response, sizeof(response));
    if (n > 0) {
        printf("Respuesta LIN: ");
        for (int i = 0; i < n; i++) {
            printf("%02X ", response[i]);
        }
        printf("\n");
    }
    
    close(lin_fd);
    return 0;
}