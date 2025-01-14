/**************************************************************
Autor: Daniel Gonzalez
Materia: Sistemas Operativos
Fecha: 17/10/24
Tema: Pipe Named o FIFO o Tuberia
Topico: Comunicacion Uni-Direccional
El servidor recibe mensajes del cliente (FIFO_CLIENTE) y enviar respuestas . 
Crea los FIFOs si no existen y los abre para lectura y escritura. En un bucle infinito, espera recibir un mensaje del cliente, lo imprime y verifica si es "end".
 Si el mensaje no es "end", el servidor invierte la cadena recibida mediante la funciσn reverse_string y envνa la cadena invertida de vuelta al cliente. 
Al recibir "end", cierra ambos FIFOs y elimina los archivos FIFO, finalizando su ejecuciσn.
 *********************** SERVIDOR **************
*****************************************************************/
   #include <stdio.h>
   #include <string.h>
   #include <fcntl.h>
   #include <unistd.h>
   #include <sys/stat.h>

   #define FIFO_CLIENT "FIFO_CLIENTE" // FIFO para recibir mensajes del cliente
   #define FIFO_SERVER "FIFO_SERVER"  // FIFO para enviar mensajes al cliente

   void reverse_string(char *str);

   int main() {
       int fd_client, fd_server;
       char readbuf[80];
       char end[10];
       int to_end;
       int read_bytes;

 
    printf("\n\t <<< BIENVENIDOS AL SERVIDOR >>> \n");
    printf("βββββββββββββββββββββββββββββββββββββββββββββββββ\n");
    printf("β                Estado del Servidor            β\n");
    printf("βββββββββββββββββββββββββββββββββββββββββββββββββ€\n");
    printf("β Estado: β En funcionamiento                  β\n");
    printf("β Conexiones: 12 / 100                          β\n");
    printf("β Uptime: 5 dΓ­as, 3 horas                       β\n");
    printf("| Acontinuacion Iniciaran los mensajes          |\n");                     
    printf("βββββββββββββββββββββββββββββββββββββββββββββββββ\n");

       // Crear los FIFOs si no existen
       mkfifo(FIFO_CLIENT, 0666); // FIFO para recibir mensajes del cliente
       mkfifo(FIFO_SERVER, 0666); // FIFO para enviar mensajes al cliente

       strcpy(end, "end");

       // Abre los FIFOs: lee del cliente y escribe al servidor
       fd_client = open(FIFO_CLIENT, O_RDONLY); // Servidor lee de FIFO_CLIENTE
       fd_server = open(FIFO_SERVER, O_WRONLY); // Servidor escribe en FIFO_SERVER

       while (1) {
           // Leer mensaje del cliente
           read_bytes = read(fd_client, readbuf, sizeof(readbuf));
           readbuf[read_bytes] = '\0';
           printf("FIFOSERVER: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

           to_end = strcmp(readbuf, end);
           if (to_end == 0) {
               close(fd_client);
               close(fd_server);
               break;
           }

           // Invertir la cadena recibida
           reverse_string(readbuf);
           printf("FIFOSERVER: Sending Reversed String: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf));

           // Enviar mensaje de vuelta al cliente
           write(fd_server, readbuf, strlen(readbuf));

           // Esperar un momento antes de la siguiente iteraciΓ³n
           sleep(2);
       }

       // Eliminar los FIFOs al terminar
       unlink(FIFO_CLIENT);
       unlink(FIFO_SERVER);

       return 0;
   }

   void reverse_string(char *str) {
       int last, limit, first;
       char temp;
       last = strlen(str) - 1;
       limit = last / 2;
       first = 0;

       while (first < last) {
           temp = str[first];
           str[first] = str[last];
           str[last] = temp;
           first++;
           last--;
       }
   }

