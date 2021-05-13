//<MiMacSerialclass.h>


//Desarrollo de una serial class propia que permita la comunicación con el sistema operativo MacOS X
//Bibliografía para el realización de la misma:
// * https://www.pololu.com/docs/0J73/15.5
// * https://github.com/todbot/arduino-serial
// * https://forum.arduino.cc/t/mac-os-x-c-question/41272
// * https://github.com/kleydon/Mac-SerialPort-Cpp
// * https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
// * http://www.science.smith.edu/dftwiki/index.php/CSC231_Arduino-serial.c
// * https://github.com/Marzac/rs232


#include <iostream>
#include <chrono>
#include <fcntl.h> //función buscada con esta librería: open()
#include <stdio.h>
#include <termios.h>
#include <unistd.h> // funciones buscadas con esta librería: write(), read(), close(), ssize_t
#include <errno.h> //función buscada con esta librería: errno
#include <cstring>

int apertura_y_config_del_puerto(const char* puerto, int baudRate);
bool puerto_abierto();
ssize_t vaciar_datos();
ssize_t escritura_de_datos(const char* bytes, size_t espacio);
ssize_t lectura_de_datos(char* bytes, size_t espacio);
ssize_t cerrar_puerto(void);
int descripcion_del_puerto(void);

using namespace std::chrono;

//Estado del puerto
static const int puerto_no_disponible = -1;
static int edp; // estado del puerto;


int apertura_y_config_del_puerto(const char* puerto, int baudRate) {
    
    //Si el puerto ya está abierto, se cierra
    if (puerto_abierto()) {
        close(edp);
    }
    //Apertura del puerto y busqueda de posibles errores
    edp = open(puerto, (O_RDWR | O_NOCTTY | O_NDELAY) );
    if (edp == -1) {
        printf("No se puede abrir el puerto : %s a una velocidad en baudios de: %d\n", puerto, baudRate);
        return edp;
    }
    
    // **** El siguiente código está extraído de https://github.com/Marzac/rs232 para el ajuste de la velocidad en baudios
    struct termios options;

    tcgetattr(edp, &options);
    switch (baudRate) {
        case 9600:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
        case 19200:
            cfsetispeed(&options, B19200);
            cfsetospeed(&options,B19200);
            break;
        case 38400:
            cfsetispeed(&options, B38400);
            cfsetospeed(&options, B38400);
            break;
        case 57600:
            cfsetispeed(&options, B57600);
            cfsetospeed(&options, B57600);
            break;
        default:
            printf("Requested baud rate %d not currently supported. Defaulting to 9,600.\n", baudRate);
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
    }
    
    options.c_iflag &= ~(INLCR | ICRNL);
    options.c_iflag |= IGNPAR | IGNBRK;
    options.c_oflag &= ~(OPOST | ONLCR | OCRNL);
    options.c_cflag &= ~(PARENB | PARODD | CSTOPB | CSIZE | CRTSCTS);
    options.c_cflag |= CLOCAL | CREAD | CS8;
    options.c_lflag &= ~(ICANON | ISIG | ECHO);
    options.c_cc[VTIME] = 1;
    options.c_cc[VMIN]  = 0;
  
    if (tcsetattr(edp, TCSANOW, &options) < 0) {
        printf("Error al configurar las propiedades del puerto.\n");
        close(edp);
        return -2; //Using negative value; -1 used above for different failure
    }
    // ****
    return edp;
}

bool puerto_abierto() {
    return edp != puerto_no_disponible;
}

milliseconds tiempo_en_ms() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch());
}

ssize_t vaciar_datos() {
    
    float duracion_en_ms = 150.0f;
    
    ssize_t resultado = 0;
    milliseconds inicio_marca_del_tiempo = tiempo_en_ms();
    while (tiempo_en_ms().count() - inicio_marca_del_tiempo.count() < duracion_en_ms) {
        char buffer[1];
        resultado = lectura_de_datos(buffer, 1);
        if (resultado < 0) {
            printf("lectura_de_datos() ha fallado. Error: %s", strerror(errno));
        }
    };      
    return resultado;
}

//Devuelve -1 en caso de error, con errno establecido adecuadamente
ssize_t escritura_de_datos(const char* bytesToWrite, size_t num_bytes_para_escribir) {
    
    ssize_t num_Bytes_escritos = write(edp, bytesToWrite, num_bytes_para_escribir);
    if (num_Bytes_escritos < 0) {
        printf("write() ha fallado. Error: %s", strerror(errno));
    }
    
    return num_Bytes_escritos;
}

//Devuelve -1 en caso de error, con errno establecido adecuadamente
ssize_t lectura_de_datos(char* const rxBuffer, size_t num_bytes_a_Recibir) {
    
    ssize_t num_Bytes_leidos = read(edp, rxBuffer, num_bytes_a_Recibir);
    if (num_Bytes_leidos < 0) {
        printf("La lectura del puerto serie () falló. Error:%s", strerror(errno));
    }
    
    return num_Bytes_leidos;
}


ssize_t cerrar_puerto(void) {
    ssize_t resultado = 0;
    if (puerto_abierto()) {
        resultado = close(edp);
        edp = puerto_no_disponible;
    }
    return resultado;
}


int descripcion_del_puerto(void) {
    return edp;
}
