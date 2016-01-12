#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <syslog.h>
//Funcion que imprime en pantalla el contenido en Hexa
void imprimirHexa(unsigned char *,int);

void imprimirHexa(unsigned char * cadena,int tamano)
{
	int i = 0;
	for(i = 0; i < tamano;i++){		
		printf("%x:",cadena[i]);
	}
	printf("\n");	
}

//Funcion que envia datos hacia el dispositivo una vez que este
//Se encuentra conectado.
void enviarSuprema(int,unsigned char *,int);

void enviarSuprema(int sockfd,unsigned char * mensaje,int tamano)
{
	int n;
	char buffer[tamano];
	memcpy(buffer,mensaje,tamano);
	int i;
        for(i=0;i<40;i++){
                printf("%x:",buffer[i]);
        }
        printf("\n");
	n = write( sockfd, buffer, tamano );
	if ( n < 0 ) {
		perror( "escritura en socket" );
		exit( 1 );
	}
}

void enviarExtendido(int,unsigned char *,int,int,int,int);
void enviarExtendido(int sockfd,unsigned char * mensaje,int tamano,int id,int bit,int secuencia)
{
        int n;
        unsigned char buffer[tamano];
        memcpy(buffer,mensaje,tamano);
        int b0 = (id >> (0 * 8)) & 0xFF;
        int b1 = (id >> (1 * 8)) & 0xFF;
        int b2 = (id >> (2 * 8)) & 0xFF;
        int b3 = (id >> (3 * 8)) & 0xFF;
        buffer[4] = b0;
        buffer[5] = b1;
        buffer[6] = b2;
        buffer[7] = b3;
        if(secuencia == 2) buffer[32] = bit;
        if(secuencia == 3) buffer[32] = bit + 18;
        if(secuencia == 5) buffer[32] = bit + 24;
        if(secuencia == 7) buffer[32] = bit + 5;
        if(secuencia == 8) buffer[32] = bit - 155;
        if(secuencia == 9) buffer[32] = bit - 129;
	/*int i;
	for(i=0;i<40;i++){
		printf("%x:",buffer[i]);
	}
	printf("\n");*/
        n = write( sockfd, buffer, tamano );
        if ( n < 0 ) {
                perror( "escritura en socket" );
                exit( 1 );
        }
}

void enviarExtendido2(int,unsigned char *,int,int,int,int);
void enviarExtendido2(int sockfd,unsigned char * mensaje,int tamano,int id,int bit,int secuencia)
{
        int n;
        unsigned char buffer[tamano];
        memcpy(buffer,mensaje,tamano);
        int b0 = (id >> (0 * 8)) & 0xFF;
        int b1 = (id >> (1 * 8)) & 0xFF;
        int b2 = (id >> (2 * 8)) & 0xFF;
        int b3 = (id >> (3 * 8)) & 0xFF;
        buffer[4] = b0;
        buffer[5] = b1;
        buffer[6] = b2;
        buffer[7] = b3;
        if(secuencia == 0) buffer[32] = bit;
        if(secuencia == 1) buffer[32] = bit + 4;
        int i;
        for(i=0;i<40;i++){
                printf("%x:",buffer[i]);
        }
        printf("\n");
        n = write( sockfd, buffer, tamano );
        if ( n < 0 ) {
                perror( "escritura en socket" );
                exit( 1 );
        }
}

//Funcion que recibe los datos de suprema
void recibirSuprema(int,unsigned char *,int);

void recibirSuprema(int sockfd,unsigned char recibido[],int tamano)
{
	int n;
	char buffer[tamano];
	memset( buffer, 0, tamano );
	memset(recibido,0,tamano);
	n = read( sockfd, buffer,  tamano);			
	if ( n < 0 ) {
		perror( "lectura de socket" );
		exit(1);
		}
	memcpy(recibido,buffer,tamano);
}

//0 para paquetes normales
//1 para paquetes de datos
int detectar( char * );
int detectar(char * buffer)
{	
	char a[3];
	memcpy(a,&buffer[38],2);
	a[2] = '\0';
	if(!strncmp(a,"}}",2)) return 0;
	else return 1;
	
	
}

//Si llegan los paquetes de datos vamos a extraer los datos
int extraerDatos(unsigned char *);
int extraerDatos(unsigned char * buffer)
{
	char a[9],b[9],i,c[16];
	char buff[20];
	//int d;
	memcpy(a,&buffer[8],7);
	//a[8] = '\0';
	//b[8] = '\0';
	c[15] = '\0';
	for(i = 0; i < 8; i++){ //Ordenado
		 b[i] = a[7-i];
		//printf("b[%d]=%x ",i,b[i]);
	}
	sprintf(c,"%x%x%x%x%x%x%x%x",b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7]);
	strcpy(buff,c);
	//printf("%s",c);
	//printf(" srtlen - %int",(int)strlen(buff));
	int resultado,lon;
	resultado = 0;
	lon = (int)strlen(c) - 1;
	for(i=0;i<lon+1;i++){
		if((int)'a' == (int)c[i]) resultado += 10 * pow(16,lon-i);
		else if((int)'b' == (int)c[i]) resultado += 11 * pow(16,lon-i);
		else if((int)'c' == (int)c[i]) resultado += 12 * pow(16,lon-i);
		else if((int)'d' == (int)c[i]) resultado += 13 * pow(16,lon-i);
		else if((int)'e' == (int)c[i]) resultado += 14 * pow(16,lon-i);
		else if((int)'f' == (int)c[i]) resultado += 15 * pow(16,lon-i);
		else resultado += (((int)c[i]-'0')%48) * pow(16,lon-i);
	}	
	printf("Usuario %d\n",resultado);
	return resultado;
	
}

char * extraerEvento(unsigned char * buffer)
{
	char a[5];
	//memcpy(a,&buffer[0],2);
	sprintf(a,"%x%x",buffer[0],buffer[1]);
	a[5] = '\0';
	if(!strncmp(a,"272f",4)) return "Concedido\n"; //printf("Granted\n");
	else if(!strncmp(a,"8a",2)) return "Relay off - Door Open\n";//printf("Relay off - Door Open\n");
	else if(!strncmp(a,"81",2)) return "Relay on - Door Close\n";//printf("Relay on - Door Close\n");
	else if(!strncmp(a,"284a",4)) return "Unregisted User\n"; //printf("Unregisted User\n");
	else return "No Granted\n"; //printf("No Granted\n");
}

int extraerEventoInt(unsigned char * buffer)
{
	char a[5];
	//memcpy(a,&buffer[0],2);
	sprintf(a,"%x%x",buffer[0],buffer[1]);
	a[5] = '\0';
	if(!strncmp(a,"272f",4)) return 3; //printf("Granted\n");
	else if(!strncmp(a,"8a",2)) return 1;//printf("Relay off - Door Open\n");
	else if(!strncmp(a,"81",2)) return 2;//printf("Relay on - Door Close\n");
	else if(!strncmp(a,"284a",4)) return 5; //printf("Unregisted User\n");
	else return 4; //printf("No Granted\n");
}


int extraerId(char * buffer);
int extraerId(char * buffer){
	int a[4];
	memcpy(a,&buffer[24],4);
	/*//printf("algo !!! %x%x%x%x len buffer = %d",a[0],a[1],a[2],a[3],strlen(a));
	//for(i = 0; i < 4; i++){ //Ordenado
	//	 b[i] = a[3-i];
	//}
	sprintf(c,"%x%x%x%x",b[0],b[1],b[2],b[3]);
	//printf("El Id de dispostivo en hexa es %s\n",c);
	int resultado,lon;
	resultado = 0;
	lon = (int)strlen(c) - 1;
	for(i=0;i<lon+1;i++){
		if((int)'a' == (int)c[i]) resultado += 10 * pow(16,lon-i);
		else if((int)'b' == (int)c[i]) resultado += 11 * pow(16,lon-i);
		else if((int)'c' == (int)c[i]) resultado += 12 * pow(16,lon-i);
		else if((int)'d' == (int)c[i]) resultado += 13 * pow(16,lon-i);
		else if((int)'e' == (int)c[i]) resultado += 14 * pow(16,lon-i);
		else if((int)'f' == (int)c[i]) resultado += 15 * pow(16,lon-i);
		else resultado += (((int)c[i]-'0')%48) * pow(16,lon-i);
	}	
	printf("Dispositivo en Int %d\n",resultado);
	//return resultado;*/
	return a[0];
}

void writeLog(int usuario,int dispositivo,char* evento);
void writeLog(int usuario,int dispositivo,char* evento)
{
	FILE *fp;
	fp = fopen("Log/log","a");
	if(fp==NULL){
		printf("Error Abrir Archivo!");
		exit(1);
	}
	
	char string[256];
	char fecha[25];
	char usuarioString[30];
	char dispositivoString[30];
	memset(string,'\0',256);
	memset(usuarioString,'\0',30);
	memset(dispositivoString,'\0',30);
	sprintf(usuarioString,"%d",usuario);
	sprintf(dispositivoString,"%d",dispositivo);
	time_t mytime;
	mytime = time(NULL);
	strcpy(fecha,ctime(&mytime));
	fecha[24] = '\0';
	strcat(string,fecha);
	strcat(string," - Dispositivo: ");
	strcat(string,dispositivoString);
	strcat(string," - Usuario: ");
	strcat(string,usuarioString);
	strcat(string," - Evento: ");
	strcat(string,evento);
	//strcat(string,"\n");
	fputs(string,fp);
	syslog(LOG_NOTICE, "%s", string);
	fclose(fp);
}

int extraerBit(unsigned char * buffer);
int extraerBit(unsigned char * buffer){
        //printf("Este es el bit %x\n",buffer[32]);
        return buffer[32];
}

void copiar(char *, unsigned char *);
void copiar(char * db, unsigned char* buffer)
{
        const char s[2] = ":";
	char * a;
        unsigned char *token;
        unsigned char val;
        char algo[120];
        int i = 0;
        memcpy(algo,db,strlen(db)+1);
        token = strtok(algo, s);
        while( token != NULL )
        {
                printf("%s ",token);
		buffer[i] = strtoul(token,&a,16);
                i++;
                token = strtok(NULL, s);
        }
}


//Manejadores para cuando se cierra el servidor para que no bloquee el puerto
void controlc(int sig);

void controlc(int sig)
{
	
	//close(sock);
	printf("Cerrando Socket en 1480\n");
	exit(EXIT_SUCCESS);
	printf("Cerrando Socket en 1480\n");
}
