#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib")

#define DEFAULT_PORT 8080
#define BUF_SIZE 1024

char consonantes[] = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
char vocales[] = "aeiouAEIOU";

void GenerarUsuario(int longitud, char* usuario) {
    int iniciaVocal = rand() % 2;
    for (int i = 0; i < longitud; i++) {
        if ((i % 2 == 0 && iniciaVocal) || (i % 2 != 0 && !iniciaVocal)) {
            usuario[i] = vocales[rand() % 10];
        } else {
            usuario[i] = consonantes[rand() % 42];
        }
    }
    usuario[longitud] = '\0';
}

void GenerarContrasena(int longitud, char* password) {
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < longitud; i++) {
        password[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    password[longitud] = '\0';
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c, recv_size;
    char mensaje_cliente[BUF_SIZE];

    // Inicialización de Winsock
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("Error Code: %d", WSAGetLastError());
        return 1;
    }

    // Crear Socket del Servidor
    if((server_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf("Error Socket Code: %d" , WSAGetLastError());
        return 1;
    }

    // Estructura sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    // Asociar socket
    if(bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Error en el bind. Código de error: %d", WSAGetLastError());
        return 1;
    }
    

    // Escuchar conexiones entrantes
    listen(server_socket, 3);

    // Aceptar conexión entrante
    printf("Esperando conexiones entrantes en el puerto %d...\n",DEFAULT_PORT);
    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET) {
        printf("Error al aceptar la conexion. Codigo de error: %d", WSAGetLastError());
        return 1;
    }
    printf("Conexion aceptada.\n");

    // Recibir Datos
    while((recv_size = recv(client_socket, mensaje_cliente, BUF_SIZE, 0)) != SOCKET_ERROR) {
        mensaje_cliente[recv_size] = '\0';
        char respuesta[BUF_SIZE];
        int longitud = atoi(mensaje_cliente + 1);

        if (mensaje_cliente[0] == '1') { // Validación de longitud de usuario
            if (longitud >= 5 && longitud <= 15) {
                GenerarUsuario(longitud, respuesta);
            } else {
                strcpy(respuesta, "Longitud de usuario invalida. Debe estar entre 5 y 15.");
            }
        } else if (mensaje_cliente[0] == '2') { // Validación de longitud de contrasena
            if (longitud >= 8 && longitud < 50) {
                GenerarContrasena(longitud, respuesta);
            } else {
                strcpy(respuesta, "Longitud de contrasena invalida. Debe estar entre 8 y 49.");
            }
        } else {
            strcpy(respuesta, "Opción invalida");
        }

        // Mostrar la respuesta antes de enviarla al cliente
        printf("Respuesta enviada al cliente: %s\n", respuesta);

        // Enviar respuesta al cliente
        send(client_socket, respuesta, strlen(respuesta), 0);
    }

    if (recv_size == SOCKET_ERROR) {
        printf("Recepción fallida. Error code: %d", WSAGetLastError());
    }

    closesocket(client_socket);
    closesocket(server_socket);
    WSACleanup();

    return 0;
}
