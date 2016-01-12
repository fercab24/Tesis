char comparar(char [],char []);

//Paquete cuando la conexion ya esta establecida
/*unsigned char mensaje[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x18,0x24,
0x61,0x83,0xaf,0x6a,0x36,0x43,0xac,0xe1,
0xa2,0xbc,0xe1,0x6b,0xcc,0x7a,0xd1,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char mensaje[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x18,0x24,
0x61,0x83,0xaf,0x6a,0x36,0x43,0xac,0xe1,
0xa2,0xbc,0xe1,0x6b,0xcc,0x7a,0xd1,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

/*unsigned char mensajebis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x18,0x24,
0x61,0x83,0xaf,0x6a,0x36,0x43,0xac,0xe1,
0xa2,0xbc,0xe1,0x6b,0xcc,0x7a,0xd1,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char mensajebis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x18,0x24,
0x61,0x83,0xaf,0x6a,0x36,0x43,0xac,0xe1,
0xa2,0xbc,0xe1,0x6b,0xcc,0x7a,0xd1,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


//Primer paquete enviado por el servidor, en todas las capturas es igual.
//Se envia a los 10 segundos despues de haber hecho el 3 way hand shake
unsigned char primero[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x01,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

//Tercero paquete enviado por el servidor, en todas las capturas es igual.
/*unsigned char tercero[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x13,0xc4,
0x1c,0x7a,0x94,0xc5,0xd5,0x3d,0x03,0xbd,
0x9d,0x8e,0x87,0xb1,0xeb,0x65,0xe1,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char tercero[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x13,0xc4,
0x1c,0x7a,0x94,0xc5,0xd5,0x3d,0x03,0xbd,
0x9d,0x8e,0x87,0xb1,0xeb,0x65,0x00,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

/*unsigned char tercerobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x13,0xc4,
0x1c,0x7a,0x94,0xc5,0xd5,0x3d,0x03,0xbd,
0x9d,0x8e,0x87,0xb1,0xeb,0x65,0xd0,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char tercerobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x13,0xc4,
0x1c,0x7a,0x94,0xc5,0xd5,0x3d,0x03,0xbd,
0x9d,0x8e,0x87,0xb1,0xeb,0x65,0xd0,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


//Hasta aca esta bien la secuencia.

//Cuarto paquete enviado por el servidor, es distinto en todas las capturas
//Vamos a probar tirando fruta
//Este teoricamente genera los paquetes de datos de respuesta de 1460
//Despues de este paquete el servidor envia los paquetes de datos, que el primero es de 32,
//variable, puede haber otro variable y cierra con uno de 8
unsigned char cuarto[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x4e,0x9b,
0x86,0xfd,0x06,0x06,0x9d,0x11,0x33,0x26,
0x9f,0xa0,0x9e,0x0a,0xa9,0x0e,0x2b,0x04,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

//Este teoricamente genera los paquetes de datos de respuesta de 80 - solo uno
/*unsigned char cuartobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x4c,0xb9,
0x97,0xe3,0x0c,0x98,0x03,0xa6,0xa2,0x83,
0xe4,0x46,0x73,0xae,0x98,0xaa,0xce,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char cuartobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x4c,0xb9,
0x97,0xe3,0x0c,0x98,0x03,0xa6,0xa2,0x83,
0xe4,0x46,0x73,0xae,0x98,0xaa,0xce,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


//Este teoricamente genera un mensaje de 96 - paquete 628
unsigned char cuarto14Julb[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x11,0x73,
0x09,0x88,0xef,0x09,0xd9,0x8b,0xea,0xc4,
0xf7,0x77,0xb5,0x53,0x34,0xca,0xc3,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

//-------------------------------------------------------------------------------------------

//quinto paquete enviado por el servidor, en todas las capturas es igual.
//Despues de recibir los datos
/*unsigned char quinto[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x2d,0x7b,
0x6f,0xa1,0xa8,0xda,0x06,0x0a,0x34,0x1e,
0x8f,0x01,0x1a,0x8c,0x9f,0xb7,0xe7,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char quinto[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x2d,0x7b,
0x6f,0xa1,0xa8,0xda,0x06,0x0a,0x34,0x1e,
0x8f,0x01,0x1a,0x8c,0x9f,0xb7,0x00,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

/*unsigned char quintobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x2d,0x7b,
0x6f,0xa1,0xa8,0xda,0x06,0x0a,0x34,0x1e,
0x8f,0x01,0x1a,0x8c,0x9f,0xb7,0xd6,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char quintobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x2d,0x7b,
0x6f,0xa1,0xa8,0xda,0x06,0x0a,0x34,0x1e,
0x8f,0x01,0x1a,0x8c,0x9f,0xb7,0xd6,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


//sexto paquete enviado por el servidor, es distinto en todas las capturas
//Lo envia justo dsp de enviar el paquete anterior, manda los 2 seguidos
//Vamos a probar tirando fruta
unsigned char sexto[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xb5,0x08,
0xa6,0x7a,0xb0,0xfc,0x58,0x3f,0x84,0x47,
0xe7,0xc6,0xc7,0x95,0x16,0x70,0x8e,0x04,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

/*unsigned char sextobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xe3,0xc9,
0x2b,0xf7,0x90,0x74,0x26,0x74,0x17,0xc6,
0x84,0x72,0x52,0x93,0x86,0x30,0x82,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char sextobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xe3,0xc9,
0x2b,0xf7,0x90,0x74,0x26,0x74,0x17,0xc6,
0x84,0x72,0x52,0x93,0x86,0x30,0x82,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


unsigned char sexto14Julb[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xc6,0x29,
0xf4,0x48,0xcc,0x51,0xc0,0x84,0x0c,0x7e,
0xd0,0x7a,0x52,0x68,0xff,0x45,0x77,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


//Estos 2 paquetes de datos que siguen son iguales en las 2 capturas.
//Y dsp de recibir estos 2 el dispositivo manda al 0.0.0.0
/*unsigned char septimo[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xa6,0xa8,
0x2c,0x23,0x59,0xaf,0x5c,0x1d,0x99,0xbf,
0xef,0x03,0x23,0xc9,0x29,0x61,0xd4,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char septimo[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xa6,0xa8,
0x2c,0x23,0x59,0xaf,0x5c,0x1d,0x99,0xbf,
0xef,0x03,0x23,0xc9,0x29,0x61,0x00,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

/*unsigned char septimobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xa6,0xa8,
0x2c,0x23,0x59,0xaf,0x5c,0x1d,0x99,0xbf,
0xef,0x03,0x23,0xc9,0x29,0x61,0xc3,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char septimobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0xa6,0xa8,
0x2c,0x23,0x59,0xaf,0x5c,0x1d,0x99,0xbf,
0xef,0x03,0x23,0xc9,0x29,0x61,0xc3,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


/*unsigned char octavo[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x94,0x3b,
0x27,0xf8,0x24,0x2b,0xf4,0x85,0xa1,0xd7,
0x5c,0x9d,0x38,0x5b,0x45,0xbd,0xd2,0x04,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/

/*unsigned char octavo[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x67,0xf6,
0x8c,0x2a,0x0f,0x7c,0xbc,0x92,0x5d,0x82,
0x83,0xa4,0x87,0x03,0x3f,0x4b,0x34,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char octavo[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x67,0xf6,
0x8c,0x2a,0x0f,0x7c,0xbc,0x92,0x5d,0x82,
0x83,0xa4,0x87,0x03,0x3f,0x4b,0x00,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};
unsigned char octavobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x67,0xf6,
0x8c,0x2a,0x0f,0x7c,0xbc,0x92,0x5d,0x82,
0x83,0xa4,0x87,0x03,0x3f,0x4b,0x23,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


/*unsigned char noveno[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x9f,0x16,
0xa2,0xb7,0x4f,0x2b,0xa3,0x1d,0x85,0x74,
0x3a,0x3f,0xf8,0xaa,0xc1,0x22,0x4e,0x03,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char noveno[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x9f,0x16,
0xa2,0xb7,0x4f,0x2b,0xa3,0x1d,0x85,0x74,
0x3a,0x3f,0xf8,0xaa,0xc1,0x22,0x00,0x03,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

/*unsigned char novenobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x9f,0x16,
0xa2,0xb7,0x4f,0x2b,0xa3,0x1d,0x85,0x74,
0x3a,0x3f,0xf8,0xaa,0xc1,0x22,0x3d,0x03,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};*/
unsigned char novenobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x7c,0xd6,0x00,0x00,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x9f,0x16,
0xa2,0xb7,0x4f,0x2b,0xa3,0x1d,0x85,0x74,
0x3a,0x3f,0xf8,0xaa,0xc1,0x22,0x3d,0x03,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};


//----------------------------------------------------------
// Esto Para el segundo puerto.
// ---------------------------------------------------------

unsigned char Dosprimero[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x01,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

unsigned char Dostercero[40] = {0x7b,0x7b,
0x3c,0x3c,0x6e,0x54,0x7b,0x20,0x01,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x06,0x01,0x00,0x00,0xe4,0xfe,
0xfb,0x55,0x00,0x00,0x00,0x00,0xd0,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

unsigned char Dostercerobis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x06,0x01,0x00,0x00,0xe4,0xfe,
0xfb,0x55,0x00,0x00,0x00,0x00,0xf4,0x05,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

//bit+4
unsigned char repetir[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x07,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xd4,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

unsigned char repetirbis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x07,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xc3,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

//bit de pecho
unsigned char ack[40] = {0x7b,0x7b,
0x3c,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x03,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xd0,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};

unsigned char ackbis[40] = {0x7b,0x7b,
0x3c,0x3c,0x76,0xd6,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x03,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xbf,0x02,
0x00,0x00,0x3e,0x3e,0x7d,0x7d};



char comparar(char A[],char B[])
{
	char i;
	for(i = 0; i < 40; i++)
	{
		if(A[i] != B[i])
			return 0;
	}
	return 1;
}
