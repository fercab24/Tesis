#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_LIST 2000
/*
* Struct: Dispositivo
* Description: Almacena informacion de los dispositivos conectados al servidor, con el proposito de manejar la 
* primera y segunda conexion de cada Dispositivo conectado. Un mismo dispositivo se conecta 2 veces por los puertos x y x+1
* Ip: Es la de cada dispositivo conectado
* Contador: Cada vez que se registra una conexion de una misma ip se incrementa contador.
* Flag:	Evita conexiones simultaneas de una misma ip, hasta que la primer secuencia termina
* y habilita esta flag para permitir la segunda conexion
* Los puertos son para validar la conexion, una conexion es valida solo si puerto2 = puerto1 + 1
* bit es el valor del byte 33 del segundo paquete recibido en la primera secuencia
* bit2 es el valor del byte 33 del segundo paquete recibido en la segunda secuencia
*/
struct Dispositivo{
	char Ip[15];
	int Id;
	int contador;
	char flag;	
	char sec1;
	int puerto1;
	int puerto2;
	int bit;
	int bit2;
};	

/*
* Function: infoDispositivo
* Description: Hace un printf de campos de la Struct Dispositivo
* Struct Dispositivo - Disp: variable sobre la que se van a imprimir los campos.
* Return: void	
*/
void infoDispositivo(struct Dispositivo Disp); // Imprime todos los campos de un dispositivo

/*
* Function: Listar
* Description: Toma array de Dispositivos para imprimir los valores de cada uno.
* Struct Dispositivo - Lista[]: Array que almacena los dispositivos
* length: Longitud del array
* Return: void
*/
void Listar(struct Dispositivo Lista[],int length); //Muestra la Informacion de todos los dispositivos

/*
* Function:
* Description:
* Return
*	
*/
void Inicializar(struct Dispositivo Lista[],int length); //Inicia todos los valores de las structs a 0.0.0.0 y 0

/*
* Function:
* Description:
* Return
*	
*/
void setIp(struct Dispositivo Lista[],int indice,char * ip); //Asigna valor de ip a una struct

/*
* Function:
* Description:
* Return
*	
*/
char * getIp(struct Dispositivo Lista[],int indice); //Retorna el valor ip de una struct

/*
* Function:
* Description:
* Return
*	
*/
void incrementarContador(struct Dispositivo Lista[],int indice); //Incrementa el valor del contador de una struct en 1

/*
* Function:
* Description:
* Return
*	
*/
int getContador(struct Dispositivo Lista[],int indice); //Retorna el campo contador de una struct

/*
* Function:
* Description:
* Return
*	
*/
void clearContador(struct Dispositivo Lista[],int indice); //Reinicia a 0 el valor de contador de una struct

/*
* Function:
* Description:
* Return
*	
*/
int getPosicion(struct Dispositivo Lista[],char * ip,int tam); //Devuelve la posicion que ocupa en la lista, un cierto dispositivo, buscando por Ip

/*
* Function:
* Description:
* Return
*	
*/
int getLastUnUsed(struct Dispositivo Lista[],int tam); //Devuelve el indice en el que encuentra 0.0.0.0, este es un espacio disponible

/*
* Function:
* Description:
* Return
*	
*/
void setFlag(struct Dispositivo Lista[],int indice); //Pone bandera en 1

/*
* Function:
* Description:
* Return
*	
*/
void clearFlag(struct Dispositivo Lista[],int indice); //Pone bandera en 0

/*
* Function:
* Description:
* Return
*	
*/
char getFlag(struct Dispositivo Lista[],int indice); //Adquirimos el valor de la flag

/*
* Function:
* Description:
* Return
*	
*/

int getPuerto1(struct Dispositivo Lista[],int indice);

/*
* Function:
* Description:
* Return
*	
*/
void setPuerto1(struct Dispositivo Lista[],int indice,int puerto);

/*
* Function:
* Description:
* Return
*	
*/
int getPuerto2(struct Dispositivo Lista[],int indice);

/*
* Function:
* Description:
* Return
*	
*/
void setPuerto2(struct Dispositivo Lista[],int indice,int puerto);

/*
* Function:
* Description:
* Return
*	
*/
void setId(struct Dispositivo Lista[],int indice,int id); //Asigna valor de id de un dispositivo a una struct

/*
* Function:
* Description:
* Return
*	
*/
int getId(struct Dispositivo Lista[],int indice); //Retorna el valor id de una struct

/*
* Function:
* Description:
* Return
*       
*/
int getBit(struct Dispositivo Lista[],int indice); //Retorna el valor id de una struct

/*
* Function:
* Description:
* Return
*       
*/
int setBit(struct Dispositivo Lista[],int indice,int bite); //Retorna el valor id de una struct

/*
* Function:
* Description:
* Return
*       
*/
int getBit2(struct Dispositivo Lista[],int indice); //Retorna el valor id de una struct

/*
* Function:
* Description:
* Return
*       
*/
int setBit2(struct Dispositivo Lista[],int indice,int bite); //Retorna el valor id de una struct

//IMPLEMENTACIONES

void infoDispositivo(struct Dispositivo Disp)
{
	printf("Ip = %s \n",Disp.Ip);
	printf("Contador = %d \n",Disp.contador);	
}

void Listar(struct Dispositivo Lista[],int length)
{
	int i;
	for(i = 0;i<length;i++) 
		infoDispositivo(Lista[i]);
}

void Inicializar(struct Dispositivo Lista[],int length)
{
	int i;
	for(i = 0;i<length;i++){
		strcpy(Lista[i].Ip,"0.0.0.0");
		Lista[i].contador = 0;
		Lista[i].flag = 0;
		Lista[i].sec1 = 1;
		Lista[i].puerto1 = -1;
		Lista[i].puerto2 = -1;
		Lista[i].Id = 0;
		Lista[i].bit = 0;
	}
}

void setIp(struct Dispositivo Lista[],int indice,char * ip)
{
	strcpy(Lista[indice].Ip,ip);
}

char * getIp(struct Dispositivo Lista[],int indice)
{
	return Lista[indice].Ip;
}

void setId(struct Dispositivo Lista[],int indice,int id)
{
	Lista[indice].Id = id;
}

int getId(struct Dispositivo Lista[],int indice)
{
	return Lista[indice].Id;
}



void incrementarContador(struct Dispositivo Lista[],int indice)
{
	Lista[indice].contador += 1;
}

int getContador(struct Dispositivo Lista[],int indice)
{
	return Lista[indice].contador;
}

void clearContador(struct Dispositivo Lista[],int indice)
{
	Lista[indice].contador = 0;
}

int getPosicion(struct Dispositivo Lista[],char * ip,int tam)
{
	int i;
	for(i = 0; i<tam ;i++){
		if(!strcmp(ip,Lista[i].Ip)) return i;
	}
	return -1;
}

int getLastUnUsed(struct Dispositivo Lista[],int tam)
{
	int i;
	for(i = 0; i<tam ;i++){
		if(!strcmp("0.0.0.0",Lista[i].Ip)) return i;
	}
	return -1;
}

void setFlag(struct Dispositivo Lista[],int indice)
{
	Lista[indice].flag = 1;
}

void clearFlag(struct Dispositivo Lista[],int indice)
{
	Lista[indice].flag = 0;
}

char getFlag(struct Dispositivo Lista[],int indice)
{
	return Lista[indice].flag;
}

void setSec(struct Dispositivo Lista[],int indice)
{
	Lista[indice].sec1 = 1;
}

void clearSec(struct Dispositivo Lista[],int indice)
{
	Lista[indice].sec1 = 0;
}

char getSec(struct Dispositivo Lista[],int indice)
{
	return Lista[indice].sec1;
}

int getPuerto1(struct Dispositivo Lista[],int indice)
{
	return Lista[indice].puerto1;
}

void setPuerto1(struct Dispositivo Lista[],int indice,int puerto)
{
	Lista[indice].puerto1 = puerto;
}

int getPuerto2(struct Dispositivo Lista[],int indice)
{
	return Lista[indice].puerto2;
}

void setPuerto2(struct Dispositivo Lista[],int indice,int puerto)
{
	Lista[indice].puerto2 = puerto;
}

int getBit(struct Dispositivo Lista[],int indice)
{
        return Lista[indice].bit;
}


int setBit(struct Dispositivo Lista[],int indice,int bite)
{
        Lista[indice].bit = bite;
}

int getBit2(struct Dispositivo Lista[],int indice)
{
        return Lista[indice].bit2;
}


int setBit2(struct Dispositivo Lista[],int indice,int bite)
{
        Lista[indice].bit2 = bite;
}

