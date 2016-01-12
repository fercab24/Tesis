#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#define TAM 40
#include "Datos.h"
#include "Funciones.h"
#include "ListMethods.h"
#include <my_global.h> //Interfaz con mysql
#include <mysql.h>
#include "/etc/gacceso/gacceso-dev.config"

struct Dispositivo Lista[TAM_LIST];

int main( int argc, char *argv[] ) {
	int sockfd, newsockfd, puerto, clilen, pid;
	char buffer[TAM],mensajeLog[256],sql[300];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	unsigned char recibidoLarge[1460];
	unsigned char recibido[250];
	unsigned char puerto2[40];
	unsigned char datos1Puerto2[32];
	unsigned char datos2Puerto2[16];
	unsigned char datos3Puerto2[8];
	unsigned char usuario[8];
	int primerPuerto = 0;
	int posDisponible,contador,valida;
	char flag = 1;
	char ipstr[INET6_ADDRSTRLEN + 1];

	openlog("Suprema Monitoring Server",LOG_CONS | LOG_PID | LOG_NDELAY,LOG_USER );

	
	/**
	* Inicializamos la Esttructura para preparla para la conexion con la base de datos
	*/
	MYSQL *con; //= mysql_init(NULL);
	/*con = mysql_init(NULL);
	if(con == NULL){	
		syslog(LOG_NOTICE,"%s","Error Iniciando Struct MySql\n");
		exit( 1 );
	}*/
	/**
	* Creamos la conexion con la base de datos
	*/
	/*if(mysql_real_connect(con,host,user,pass,"Suprema",0,NULL,0) == NULL){
		mysql_close(con);	
		syslog(LOG_NOTICE,"%s","Error Estableciendo Conexion MySql\n");
		exit( 1 );
	}*/	
	
	/**
	* Creamos la zona de memoria compartida para poder mantener solo una tabla de los dispositivos conectados
	*/
	struct Dispositivo *ListaShared = mmap(Lista, (sizeof Lista), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	//printf("Inicializando Lista\n");
	Inicializar(ListaShared,TAM);

	/**
	* Parte de sockets
	*/
	if ( argc < 2 ) {
		memset(mensajeLog,'\0',256);
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		sprintf( mensajeLog, "Uso: %s <puerto>\n", argv[0] );
		syslog(LOG_NOTICE,"%s",mensajeLog);
		exit( 1 );
	}

	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) { 
		memset(mensajeLog,'\0',256);
		sprintf(mensajeLog,"Error Apertura Socket\n");
		syslog(LOG_NOTICE,"%s",mensajeLog);
		perror( " apertura de socket ");
		exit( 1 );
	}

	int yes=1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
	    memset(mensajeLog,'\0',256);
            sprintf(mensajeLog,"Error SetSockOpt\n");
            syslog(LOG_NOTICE,"%s",mensajeLog);
	    perror("setsockopt");
	    exit(1);
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		memset(mensajeLog,'\0',256);
                sprintf(mensajeLog,"Error Bind\n");
                syslog(LOG_NOTICE,"%s",mensajeLog);
		perror( "ligadura" );
		exit( 1 );
	}

	//Quedamos A la espera de clientes que se conecten	
        //printf( "Proceso: %d - socket servidor disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );
	memset(mensajeLog,'\0',256);
        sprintf(mensajeLog,"Proceso: %d - socket servidor disponible: %d\n", getpid(), ntohs(serv_addr.sin_port));
        syslog(LOG_NOTICE,"%s",mensajeLog);
	//syslog(LOG_NOTICE, "%s", "Socket Servidor Disponible");
	listen( sockfd, TAM_LIST );
	clilen = sizeof( cli_addr );	
	
	while( 1 ) {
		//Aceptamos la nueva de conexion de un cliente
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
		if ( newsockfd < 0 ) {
			memset(mensajeLog,'\0',256);
	                sprintf(mensajeLog,"Error Accept New Client\n");
			syslog(LOG_NOTICE,"%s",mensajeLog);
			perror( "accept" );
			exit( 1 );
		}

		//Tomamos La informacion de Puerto e Ip del cliente conectado
		if(flag){ primerPuerto = ntohs(cli_addr.sin_port);flag = 0;}
		struct sockaddr_in *s = (struct sockaddr_in *)&cli_addr;
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
		printf("Cliente Nuevo conectado - IP: %s Puerto: %d \n",ipstr,ntohs(cli_addr.sin_port));
		memset(mensajeLog,'\0',256);
                sprintf(mensajeLog,"Cliente Nuevo conectado - IP: %s Puerto: %d \n",ipstr,ntohs(cli_addr.sin_port));
                syslog(LOG_NOTICE,"%s",mensajeLog);
		
		//Actualizamos los valores de la Lista de dispositivos conectados
		//Preguntamos si el dispositivo ya existe en la lista
		posDisponible = getPosicion(ListaShared,ipstr,TAM_LIST);
		
		//Si no existe, agregas una nueva entrada en la tabla.
		//E incrementas su contador en 1.
		if(posDisponible == -1){
			posDisponible = getLastUnUsed(ListaShared,TAM_LIST);
			setIp(ListaShared,posDisponible,ipstr);
			incrementarContador(ListaShared,posDisponible);
		}
		//Si existe Incrementamos el contador
		else{
			incrementarContador(ListaShared,posDisponible);
			//Tenemos que ver si el contador llega a 3 o mas, eso significa que se desconecto
			//Y se esta intentando conectando de nuevo. Entonces bajamos la bandera
			contador = getContador(ListaShared,posDisponible);
			if(contador > 2 && getSec(ListaShared,posDisponible)){
				clearFlag(ListaShared,posDisponible);
				clearContador(ListaShared,posDisponible);
				incrementarContador(ListaShared,posDisponible);
				setSec(ListaShared,posDisponible);
			}
			//No debemos permitir 2 conexiones simultaneas, tiene que pasar
			//La secuencia del primer puerto
			//if(!getFlag(ListaShared,posDisponible)) 
				//close(newsockfd);
		}

		//Hacemos fork para atender a los clientes conectados
		pid = fork();
		if ( pid < 0 ) {
			memset(mensajeLog,'\0',256);
                        sprintf(mensajeLog,"Error Fork\n");
                        syslog(LOG_NOTICE,"%s",mensajeLog);
			perror( "fork" );
			exit( 1 );
		}

		if ( pid == 0 ) {  // Proceso hijo
			close( sockfd );
			//Listar(ListaShared,TAM_LIST);
			//printf("Tengo la Ip %s\n",ipstr);
			posDisponible = getPosicion(ListaShared,ipstr,TAM_LIST);
			//printf("Tengo la Ip %s y Contador %d y flag es %d y secuencia1 = %d\n",ipstr,getContador(ListaShared,posDisponible),getFlag(ListaShared,posDisponible),getSec(ListaShared,posDisponible));
			if(getContador(ListaShared,posDisponible) == 2 && !getFlag(ListaShared,posDisponible)){
				printf("Cerrando Sock Invalido Sec 2\n");
				memset(mensajeLog,'\0',256);
	                        sprintf(mensajeLog,"Cerrando Socket Invalido Segunda Secuencia\n");
				syslog(LOG_NOTICE,"%s",mensajeLog);

				close(newsockfd);
				exit(1);
			}
			if(getContador(ListaShared,posDisponible) == 1 && !getSec(ListaShared,posDisponible)){
				printf("Cerrando Sock Invalido Sec 1\n");
				memset(mensajeLog,'\0',256);
	                        sprintf(mensajeLog,"Cerrando Sock Invalido Primera Secuencia\n");
	                        syslog(LOG_NOTICE,"%s",mensajeLog);
				close(newsockfd);
				exit(1);
			}

			if(getContador(ListaShared,posDisponible) > 2){
				printf("Cerrando Sock Contador Invalido\n");
				memset(mensajeLog,'\0',256);
	                        sprintf(mensajeLog,"Cerrando Socket Contador Invalido\n");
	                        syslog(LOG_NOTICE,"%s",mensajeLog);
				close(newsockfd);
				exit(1);
			}

			if(getContador(ListaShared,posDisponible) == 2 && getFlag(ListaShared,posDisponible)){
				printf("Entro Secuencia 2\n");
				setPuerto2(ListaShared,posDisponible,ntohs(cli_addr.sin_port));
				//printf("Entro Secuencia 2 y seteo puerto %d\n",getPuerto2(ListaShared,posDisponible));
				//printf("Entro en la secuencia 2\n");	
				//Si la conexion no es valida salimos
				valida = getPuerto2(ListaShared,posDisponible)-getPuerto1(ListaShared,posDisponible);
				if(valida != 1){
					//printf("Conexion no valida\n");
					memset(mensajeLog,'\0',256);
		                        sprintf(mensajeLog,"Cerrando Conexion No Valida Puerto2 != Puerto1+1\n");
		                        syslog(LOG_NOTICE,"%s",mensajeLog);
					close(newsockfd);
					exit(1);
				}


				//printf("Entro Secuencia 2\n");
				//10 segundos dsp del 3 way handshake
				sleep(10);
				enviarSuprema(newsockfd,Dosprimero,40);		
				//Ahora recibimos 2 paquetes del dispositivo
				recibirSuprema(newsockfd,puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Primer Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);

				recibirSuprema(newsockfd,puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Segundo Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);
			
				//Ahora enviamos otro - igual al recibido
				//enviarSuprema(newsockfd,Dossegundo,40);		
				enviarSuprema(newsockfd,puerto2,40);
				setBit2(ListaShared,posDisponible,extraerBit(puerto2));
                                printf("El bit es = %x\n",getBit2(ListaShared,posDisponible));		

				//Recibimos
				recibirSuprema(newsockfd,puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Tercer Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);

				//Enviamos 3ero
				enviarSuprema(newsockfd,Dostercero,40);		

				//Recibimos
				recibirSuprema(newsockfd,puerto2,40);
				//Imprimimos mensaje recibido
				//printf("Otro Puerto Cuarto Mensaje Recibido: ");
				//imprimirHexa(puerto2,40);

				//Enviamos el de ping pong
				//enviarSuprema(newsockfd,repetir,40);
				enviarExtendido2(newsockfd,repetir,40,getId(ListaShared,posDisponible),getBit2(ListaShared,posDisponible),1);		

				//Recibimos Datos
				recibirSuprema(newsockfd,datos1Puerto2,32);
				//Imprimimos mensaje recibido
				//printf("Apertura Paq Datos: ");
				//imprimirHexa(datos1Puerto2,32);
				//Segunda Parte
				recibirSuprema(newsockfd,datos2Puerto2,16);
				//Imprimimos mensaje recibido
				//printf("Datos Paq Datos: ");
				//imprimirHexa(datos2Puerto2,16);
				//Tercera Parte
				recibirSuprema(newsockfd,datos3Puerto2,8);
				//Imprimimos mensaje recibido
				//printf("Cierre Paq Datos: ");
				//imprimirHexa(datos3Puerto2,8);
				sleep(1); //para que termine de recibir
				//enviarSuprema(newsockfd,ack,40);
				enviarExtendido2(newsockfd,ack,40,getId(ListaShared,posDisponible),getBit2(ListaShared,posDisponible),0);
				//Enviamos el de ping pong
				memset(mensajeLog,'\0',256);
	                        sprintf(mensajeLog,"Listo Para Recibir Datos\n");
	                        syslog(LOG_NOTICE,"%s",mensajeLog);
				printf("Listo !!!\n");
				while(1){
					/*recibirSuprema(newsockfd,&puerto2,40);
					printf("Tipo de Paquete %d\n",detectar(puerto2));
					sleep(0.5);
					enviarSuprema(newsockfd,repetir,40);*/
					//Primero recibo 40 bytes, no c que tipo de paquete es
					recibirSuprema(newsockfd,puerto2,40);
					//Vemos que tipo de paquete es ?
					//Paquete normal
					if(detectar(puerto2) == 0) {
						//printf("Tipo de paquete Normal\n");
						sleep(1);
						//enviarSuprema(newsockfd,repetir,40);
						enviarExtendido2(newsockfd,repetir,40,getId(ListaShared,posDisponible),getBit2(ListaShared,posDisponible),1);
					//	printf("Repetir\n");
					}
					//Paquete de datos
					else{
						//memset(usuario,'\0',8);
						recibirSuprema(newsockfd,datos2Puerto2,16);
						//imprimirHexa(datos2Puerto2,16);
						if(extraerDatos(datos2Puerto2) != 0 && extraerDatos(datos2Puerto2) < 50000){  //Solo los usuarios, no las puertas
							//writeLog(extraerDatos(datos2Puerto2),getId(ListaShared,posDisponible),extraerEvento(datos2Puerto2));
							//extraerDatos(datos2Puerto2);
							memset(sql,'\0',300); //Limpiamos el buffer del comando sql
							if(extraerEventoInt(datos2Puerto2) == 5) {//Escribir 0 en la base de datos en usuario no reconocido
								sprintf(sql,"Insert into Registro(Dispositivo,Usuario,Evento) Values(%d,%d,%d)",getId(ListaShared,posDisponible),0,extraerEventoInt(datos2Puerto2));
							writeLog(0,getId(ListaShared,posDisponible),extraerEvento(datos2Puerto2));
}
							else{
								sprintf(sql,"Insert into Registro(Dispositivo,Usuario,Evento) Values(%d,%d,%d)",getId(ListaShared,posDisponible),extraerDatos(datos2Puerto2),extraerEventoInt(datos2Puerto2));
							writeLog(extraerDatos(datos2Puerto2),getId(ListaShared,posDisponible),extraerEvento(datos2Puerto2));
}
							con = mysql_init(NULL);
							if(con == NULL){	
								syslog(LOG_NOTICE,"%s","Error Iniciando Struct MySql\n");
								exit( 1 );
							}
							/**
							* Establecemos la conexion con la base de datos
							*/
							if(mysql_real_connect(con,host,user,pass,"Suprema",0,NULL,0) == NULL){
								mysql_close(con);	
								syslog(LOG_NOTICE,"%s","Error Estableciendo Conexion MySql\n");
								exit( 1 );
							}	
							mysql_query(con,sql);
							mysql_close(con);
						}
						//writeLog(extraerDatos(datos2Puerto2),getId(ListaShared,posDisponible),extraerEvento(datos2Puerto2));
						//extraerEvento(datos2Puerto2);
						//printf("Del dispositivo %d\n",getId(ListaShared,posDisponible));
						
						recibirSuprema(newsockfd,datos3Puerto2,8);
						sleep(1);
						//enviarSuprema(newsockfd,ack,40);
						enviarExtendido2(newsockfd,ack,40,getId(ListaShared,posDisponible),getBit2(ListaShared,posDisponible),0);
					}
				}


			}

			
			if(getContador(ListaShared,posDisponible) == 1 && getSec(ListaShared,posDisponible)){	
				setPuerto1(ListaShared,posDisponible,ntohs(cli_addr.sin_port));
				printf("Entro Secuencia 1 y bajo bandera y seteo puerto %d\n",getPuerto1(ListaShared,posDisponible));
				clearSec(ListaShared,posDisponible);
				//Delay observado en la captura
				sleep(10);
				//Mandamos el primero de 40 bytes
				enviarSuprema(newsockfd,primero,40);
				printf("Mando Primero\n");

				//Recibimos el primero de 40 bytes
				recibirSuprema(newsockfd,recibido,40);
				printf("Recibo Primero\n");
				//printf("pos = %d, id = %d\n"),posDisponible,extraerId(recibido);
				setId(ListaShared,posDisponible,extraerId(recibido));

				//Recibimos El segundo de 40
				recibirSuprema(newsockfd,recibido,40);
				printf("Recibo Segundo\n");
				//Mandamos el segundo de 40 - igual en todas
				enviarSuprema(newsockfd,recibido,40);
				setBit(ListaShared,posDisponible,extraerBit(recibido));
				printf("El bit es = %x\n",getBit(ListaShared,posDisponible));
				printf("Mando Segundo\n");
				sleep(5); //Delay observado en la captura

				//Mandamos el tercero de 40 - igual en todas
				//enviarSuprema(newsockfd,tercero,40);
				enviarExtendido(newsockfd,tercero,40,getId(ListaShared,posDisponible),getBit(ListaShared,posDisponible),3);
				printf("Mando tercero \n");

				//Recibimos tercero que es  distinto en todas las capturas, pero de 40
				recibirSuprema(newsockfd,recibido,40);
				printf("Recibo Tercero\n");
				//Cuarto Paquete Servidor
				//Ahora el servidor envia un paquete que es diferente en todas las capturas.
				//Pero de 40, y no c que manda, no es un timestamp
				con = mysql_init(NULL);
                                if(con == NULL){
	                                syslog(LOG_NOTICE,"%s","Error Iniciando Struct MySql\n");
        	                        exit( 1 );
                	                }
                                /**
                                * Establecemos la conexion con la base de datos
                                */
                                if(mysql_real_connect(con,host,user,pass,"Suprema",0,NULL,0) == NULL){
                        	        mysql_close(con);
                                	syslog(LOG_NOTICE,"%s","Error Estableciendo Conexion MySql\n");
                                	exit( 1 );
                               	 	}
				char idstr[9];
				memset(idstr,'\0',9);
				memset(sql,'\0',300);
				sprintf(idstr,"%d",getId(ListaShared,posDisponible));
				strcpy(sql,"Select Contenido From Paquetes where Orden = 4 and Id = ");
				strcat(sql,idstr);
				printf("COMANDO SQL ----------------- %s\n",sql);
                                mysql_query(con,sql);
				MYSQL_RES *result = mysql_store_result(con);
				MYSQL_ROW row = mysql_fetch_row(result);
				printf("Resultado de base Datos %s\n",row[0]);
				mysql_free_result(result);
                                mysql_close(con);
				copiar(row[0],cuarto);
				printf("Es el resultado dsp de copiar ");
				imprimirHexa(cuarto,40);
				enviarSuprema(newsockfd,cuarto,40);
				printf("Mando Cuarto\n");
				//Recibimos los datos que envia el dispositivo -- primero el de 32
				//Parece ser que envia todos los paquetes de pecho.
				printf("Inicio Bardo\n");
				recibirSuprema(newsockfd,recibido,32);
				recibirSuprema(newsockfd,recibidoLarge,1460);
				recibirSuprema(newsockfd,recibido,8);
				printf("Final Bardo\n");

				//Mandamos el quinto de 40 - igual en todas
				sleep(1); //Es necesario para que termine de recibir los datos.
				//enviarSuprema(newsockfd,quinto,40);
				enviarExtendido(newsockfd,quinto,40,getId(ListaShared,posDisponible),getBit(ListaShared,posDisponible),5);
				
				sleep(4);//Delay observado en la captura
				//Ahora el servidor envia un paquete que es diferente en todas las capturas.
				//Pero de 40, y no c que manda
				 con = mysql_init(NULL);
                                if(con == NULL){
                                        syslog(LOG_NOTICE,"%s","Error Iniciando Struct MySql\n");
                                        exit( 1 );
                                        }
                                /**
                                * Establecemos la conexion con la base de datos
                                */
                                if(mysql_real_connect(con,host,user,pass,"Suprema",0,NULL,0) == NULL){
                                        mysql_close(con);
                                        syslog(LOG_NOTICE,"%s","Error Estableciendo Conexion MySql\n");
                                        exit( 1 );
                                        }
				memset(sql,'\0',300);
                                strcpy(sql,"Select Contenido From Paquetes where Orden = 6 and Id = ");
                                strcat(sql,idstr);
                                printf("COMANDO SQL ----------------- %s\n",sql);
                                mysql_query(con,sql);
				//mysql_query(con,"select Contenido from Paquetes where Orden = 6");
                                result = mysql_store_result(con);
                                row = mysql_fetch_row(result);
                                printf("Resultado de base Datos----------------------- %s\n",row[0]);
                                mysql_free_result(result);
                                mysql_close(con);
                                copiar(row[0],sexto);
				enviarSuprema(newsockfd,sexto,40);
				
				//Y Esperamos recibir mas datos del dispositivo
				recibirSuprema(newsockfd,recibido,32);
				recibirSuprema(newsockfd,recibidoLarge,1460);
				recibirSuprema(newsockfd,recibido,8);						

				
				//Mandamos los 2 paquetes de la esperanza, son iguales en las capturas
				sleep(1); //Esperando que termine de recibir
				//enviarSuprema(newsockfd,septimo,40); 
				enviarExtendido(newsockfd,septimo,40,getId(ListaShared,posDisponible),getBit(ListaShared,posDisponible),7);
				sleep(4); //Delay observado en la caputra
				//enviarSuprema(newsockfd,octavo,40);  
				enviarExtendido(newsockfd,octavo,40,getId(ListaShared,posDisponible),getBit(ListaShared,posDisponible),8);
				
				//Ahora recibimos uno mas que es igual en todas las capturas
				recibirSuprema(newsockfd,recibido,40);
				
				//Despues mandamos otro que es igual en todas las capturas
				//enviarSuprema(newsockfd,noveno,40);
				enviarExtendido(newsockfd,noveno,40,getId(ListaShared,posDisponible),getBit(ListaShared,posDisponible),9);
				//Recibimos otro set de paquetes de datos
				recibirSuprema(newsockfd,recibido,32);
				recibirSuprema(newsockfd,recibido,240);
				recibirSuprema(newsockfd,recibido,8);
				sleep(1);
				//Y seteamos la bandera para que se pueda contestar en el segundo puerto
				posDisponible = getPosicion(ListaShared,ipstr,TAM_LIST);
				setFlag(ListaShared,posDisponible);
				setSec(ListaShared,posDisponible);
				printf("Terminando Secuencia 1 en puerto %d\n",ntohs(cli_addr.sin_port));
				//Empiezan los paquetes bien conocidos
				while(1){
					//enviarSuprema(newsockfd,mensaje,40);
					enviarExtendido(newsockfd,mensaje,40,getId(ListaShared,posDisponible),getBit(ListaShared,posDisponible),0);
					recibirSuprema(newsockfd,recibido,40);
					sleep(1);
					//printf("Ping Pong\n");
				}
				
			}//end while
		}
		else {
			//printf( "SERVIDOR: Nuevo cliente, que atiende el proceso hijo: %d\n", pid );
			close( newsockfd );
			if(signal(SIGINT,controlc)== SIG_ERR)
			{ perror("signal");
			  exit(1);
			}
		}
	} //End While mas grande
	return 0; 
} 
