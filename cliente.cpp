#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

#define SERVER "127.0.0.1"
#define DEFAULT_PORT 8080
#define BUF_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char message[BUF_SIZE], server_reply[BUF_SIZE];
    int recv_size;
    boolean loop = 1;

    // Inicialización de Winsock
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Error Code: %d", WSAGetLastError());
        return 1;
    }

    // Crear Socket del Cliente
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Error Socket Code: %d", WSAGetLastError());
    }

    server.sin_addr.s_addr = inet_addr(SERVER);
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    // Conectar al servidor
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Connect error: %d", WSAGetLastError());
        return 1;
    }
     printf("Conectado al servidor.\n");

    // Menú
    while (loop) {
        int opcion;
        int longitud;

        printf("\n***********************ACTIVIDAD SOCKETS***********************\n");
        printf("\n1. Generar Usuario\n2. Generar Contrasena\n3. Salir de la Aplicacion\nElegir opcion: ");
        scanf("%d", &opcion);

        if (opcion == 3) break;

        printf("Ingresar longitud: ");
        scanf("%d", &longitud);

        sprintf(message, "%d%d", opcion, longitud);

        // Enviar mensaje al servidor
        if (send(s, message, strlen(message), 0) < 0) {
            printf("Envio fallido");
            return 1;
        }

        // Recibir respuesta del servidor
        if ((recv_size = recv(s, server_reply, BUF_SIZE, 0)) == SOCKET_ERROR) {
            printf("Recepcion fallida");
            break;
        }

        server_reply[recv_size] = '\0';
        printf("Respuesta del servidor: %s\n", server_reply);
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
