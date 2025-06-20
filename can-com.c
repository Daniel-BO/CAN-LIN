#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int main() {
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    
    // Crear socket CAN
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error al crear el socket CAN");
        return 1;
    }
    
    strcpy(ifr.ifr_name, "can0");
    ioctl(s, SIOCGIFINDEX, &ifr);
    
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error en bind");
        return 1;
    }
    
    // Configurar frame CAN
    frame.can_id = 0x123;
    frame.can_dlc = 8;
    frame.data[0] = 0x11;
    frame.data[1] = 0x22;
    frame.data[2] = 0x33;
    frame.data[3] = 0x44;
    frame.data[4] = 0x55;
    frame.data[5] = 0x66;
    frame.data[6] = 0x77;
    frame.data[7] = 0x88;
    
    // Enviar frame
    if (write(s, &frame, sizeof(frame)) != sizeof(frame)) {
        perror("Error al enviar frame");
        return 1;
    }
    
    // Recibir frame
    int nbytes = read(s, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("Error al recibir frame");
        return 1;
    }
    
    printf("ID: 0x%X DLC: %d Data: ", frame.can_id, frame.can_dlc);
    for (int i = 0; i < frame.can_dlc; i++) {
        printf("%02X ", frame.data[i]);
    }
    printf("\n");
    
    close(s);
    return 0;
}