#include <network.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "discord_rpc.h"
#include "networkops.h"

void Discord_UpdatePresence(const char* titleId) {
    if (!titleId) return;
    
    // Certifique-se de que a rede está ativa no ULGX
    if (!IsNetworkInit()) return;

    int sock = net_socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) return;

    struct sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST; // 255.255.255.255
    broadcastAddr.sin_port = htons(1337);

    int broadcastEnable = 1;
    net_setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    u8 buffer[11];
    buffer[0] = 'R';
    buffer[1] = 'B';
    buffer[2] = 'R';
    buffer[3] = 'E';
    buffer[4] = 0x00; // 0x00 = Update

    memset(&buffer[5], 0, 6);
    strncpy((char*)&buffer[5], titleId, 6);

    for(int i = 0; i < 3; i++) {
        net_sendto(sock, buffer, 11, 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr));
        usleep(10000); // 10ms
    }

    net_close(sock);
}

void Discord_ClearPresence() {
    if (!IsNetworkInit()) return;

    int sock = net_socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) return;

    struct sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;
    broadcastAddr.sin_port = htons(1337);

    int broadcastEnable = 1;
    net_setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));

    u8 buffer[11];
    memset(buffer, 0, 11);
    buffer[0] = 'R';
    buffer[1] = 'B';
    buffer[2] = 'R';
    buffer[3] = 'E';
    buffer[4] = 0x01; // 0x01 = Clear

    for(int i = 0; i < 3; i++) {
        net_sendto(sock, buffer, 11, 0, (struct sockaddr *)&broadcastAddr, sizeof(broadcastAddr));
        usleep(10000);
    }

    net_close(sock);
}
