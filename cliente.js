const net = require('net');
const readline = require('readline');

const SERVER = '127.0.0.1';
const DEFAULT_PORT = 8080;


const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// Crear Socket del Cliente
const client = new net.Socket();

// Conectar al servidor
client.connect(DEFAULT_PORT, SERVER, () => {
    console.log('Conectado al servidor.');
    menu();
});

// Manejar datos recibidos del servidor
client.on('data', (data) => {
    console.log(`Respuesta del servidor: ${data.toString()}`);
    menu();
});

// Manejar errores
client.on('error', (err) => {
    console.error(`Error: ${err.message}`);
});

// Cierre de la conexión
client.on('close', () => {
    console.log('Conexión cerrada.');
});

// Menú
function menu() {
    console.log('\n***********************ACTIVIDAD SOCKETS***********************');
    console.log('\n1. Generar Usuario');
    console.log('2. Generar Contraseña');
    console.log('3. Salir de la Aplicación');
    rl.question('Elegir opción: ', (opcion) => {
        if (opcion == '3') {
            client.end();
            rl.close();
            return;
        }

        rl.question('Ingresar longitud: ', (longitud) => {
            const message = `${opcion}${longitud}`;
            // Enviar mensaje al servidor
            client.write(message);
        });
    });
}
