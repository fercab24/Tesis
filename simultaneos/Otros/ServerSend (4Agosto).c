#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define TAM 40
#include "Datos.h"
#include "Funciones.h"

int main( int argc, char *argv[] ) {
	int sockfd, newsockfd, puerto, clilen, pid;
	char buffer[TAM];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	unsigned char recibidoLarge[1460];
	unsigned char recibido[250];
	unsigned char puerto2[40];
	unsigned char datos1Puerto2[32];
	unsigned char datos2Puerto2[16];
	unsigned char datos3Puerto2[8];
	unsigned char usuario[8];

	if ( argc < 2 ) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit( 1 );
	}

	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) { 
		perror( " apertura de socket ");
		exit( 1 );
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		perror( "ligadura" );
		exit( 1 );
	}

        printf( "Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );
	
	listen( sockfd, 5 );
	clilen = sizeof( cli_addr );	
	
	while( 1 ) {
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
		if ( newsockfd < 0 ) {
			perror( "accept" );
			exit( 1 );
		}
		int primeraConexion = ntohs(cli_addr.sin_port);
		//char peer_addr_str[ INET_ADDRSTRLEN ];
		//inet_ntop( AF_INET, &cli_addr, peer_addr_str, INET_ADDRSTRLEN );
		printf("Cliente Nuevo conectado, Puerto: %d \n",primeraConexion);
		pid = fork();
		if ( pid < 0 ) {
			perror( "fork" );
			exit( 1 );
		}

		if ( pid == 0 ) {  // Proceso hijo
			close( sockfd );
			if(ntohs(cli_addr.sin_port) == 2){
				//10 segundos dsp del 3 way handshake
				sleep(10);
				enviarSuprema(newsockfd,Dosprimero,40);		
				//Ahora recibimos 2 paquetes del dispositivo
				recibirSuprema(newsockfd,&puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Primer Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);

				recibirSuprema(newsockfd,&puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Segundo Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);
			
				//Ahora enviamos otro
				enviarSuprema(newsockfd,Dossegundo,40);		

				//Recibimos
				recibirSuprema(newsockfd,&puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Tercer Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);

				//Enviamos 3ero
				enviarSuprema(newsockfd,Dostercero,40);		

				//Recibimos
				recibirSuprema(newsockfd,&puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Cuarto Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);

				//Enviamos el de ping pong
				enviarSuprema(newsockfd,Dospingpong,40);		

				//Recibimos Datos
				recibirSuprema(newsockfd,&datos1Puerto2,32);
				//Imprimimos mensaje recibido
				//printf("Apertura Paq Datos: ");
				//imprimirHexa(datos1Puerto2,32);
				//Segunda Parte
				recibirSuprema(newsockfd,&datos2Puerto2,16);
				//Imprimimos mensaje recibido
				//printf("Datos Paq Datos: ");
				//imprimirHexa(datos2Puerto2,16);
				//Tercera Parte
				recibirSuprema(newsockfd,&datos3Puerto2,8);
				//Imprimimos mensaje recibido
				//printf("Cierre Paq Datos: ");
				//imprimirHexa(datos3Puerto2,8);
				sleep(1); //para que termine de recibir
				enviarSuprema(newsockfd,Doscuarto,40);
				//Enviamos el de ping pong
				while(1){
					/*recibirSuprema(newsockfd,&puerto2,40);
					printf("Tipo de Paquete %d\n",detectar(puerto2));
					sleep(0.5);
					enviarSuprema(newsockfd,repetir,40);*/
					//Primero recibo 40 bytes, no c que tipo de paquete es
					recibirSuprema(newsockfd,&puerto2,40);
					//Vemos que tipo de paquete es ?
					//Paquete normal
					if(detectar(puerto2) == 0) {
						//printf("Tipo de paquete Normal\n");
						sleep(1);
						enviarSuprema(newsockfd,repetir,40);
					}
					//Paquete de datos
					else{
						//memset(usuario,'\0',8);
						recibirSuprema(newsockfd,&datos2Puerto2,16);
						memcpy(usuario,extraerDatos(datos2Puerto2),8);
						printf("Usuario: ");
						imprimirHexa(usuario,8);
						//printf("String: %s",usuario);
						recibirSuprema(newsockfd,&datos3Puerto2,8);
						sleep(1);
						enviarSuprema(newsockfd,ack,40);
					}
				}


			}


			else {	
				//Delay observado en la captura
				sleep(10);

				//Mandamos el primero de 40 bytes
				enviarSuprema(newsockfd,primero,40);

				//Recibimos el primero de 40 bytes
				recibirSuprema(newsockfd,&recibido,40);
				
				//Recibimos El segundo de 40
				recibirSuprema(newsockfd,&recibido,40);

				//Mandamos el segundo de 40 - igual en todas
				enviarSuprema(newsockfd,segundo,40);
				sleep(5); //Delay observado en la captura

				//Mandamos el tercero de 40 - igual en todas
				enviarSuprema(newsockfd,tercero,40);

				//Recibimos tercero que es  distinto en todas las capturas, pero de 40
				recibirSuprema(newsockfd,&recibido,40);
				
				//Cuarto Paquete Servidor
				//Ahora el servidor envia un paquete que es diferente en todas las capturas.
				//Pero de 40, y no c que manda, no es un timestamp
				enviarSuprema(newsockfd,cuarto,40);

				//Recibimos los datos que envia el dispositivo -- primero el de 32
				//Parece ser que envia todos los paquetes de pecho.
				recibirSuprema(newsockfd,&recibido,32);
				recibirSuprema(newsockfd,&recibidoLarge,1460);
				recibirSuprema(newsockfd,&recibido,8);

				//Mandamos el quinto de 40 - igual en todas
				sleep(1); //Es necesario para que termine de recibir los datos.
				enviarSuprema(newsockfd,quinto,40);
				
				sleep(4);//Delay observado en la captura
				//Ahora el servidor envia un paquete que es diferente en todas las capturas.
				//Pero de 40, y no c que manda
				enviarSuprema(newsockfd,sexto,40);
				
				//Y Esperamos recibir mas datos del dispositivo
				recibirSuprema(newsockfd,&recibido,32);
				recibirSuprema(newsockfd,&recibidoLarge,1460);
				recibirSuprema(newsockfd,&recibido,8);						

				
				//Mandamos los 2 paquetes de la esperanza, son iguales en las capturas
				sleep(1); //Esperando que termine de recibir
				enviarSuprema(newsockfd,septimo,40); 
				sleep(4); //Delay observado en la caputra
				enviarSuprema(newsockfd,octavo,40);  
				
				//Ahora recibimos uno mas que es igual en todas las capturas
				recibirSuprema(newsockfd,&recibido,40);
				
				//Despues mandamos otro que es igual en todas las capturas
				enviarSuprema(newsockfd,noveno,40);
				//Recibimos otro set de paquetes de datos
				recibirSuprema(newsockfd,&recibido,32);
				recibirSuprema(newsockfd,&recibido,240);
				recibirSuprema(newsockfd,&recibido,8);
				sleep(1);
				//Empiezan los paquetes bien conocidos
				while(1){
					enviarSuprema(newsockfd,mensaje,40);
					recibirSuprema(newsockfd,&recibido,40);
					sleep(1);
				}
				
			}//end while
		}
		else {
			printf( "SERVIDOR: Nuevo cliente, que atiende el proceso hijo: %d\n", pid );
			close( newsockfd );
		}
	} //End While mas grande
	return 0; 
} 
