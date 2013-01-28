/*
 *  analex.c
 *  Analizador Lexico
 *
 *  Created by Alan Rodriguez on 9/6/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "analex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

char buf[BUFSIZ];


//char c;

int main (int argc, const char * argv[]) {
	
    if (argc != 2) { 
		fprintf(stderr, "Forma de uso: %s origen\n",argv[0]);
		return -1;
	}
	
	if((fuente=fopen(argv[1],"r")) == NULL){
		fprintf(stderr, "Error al abrir el archivo: %s",argv[1]);
		return -1;
	}
	
	if ((fdLexemas=open("lexemas.txt",O_WRONLY | O_CREAT | O_TRUNC,0666)) < 0) { 
		fprintf(stderr, "Error al crear archivo lexemas.txt"); 
		return -1;
	}

	if ((fdTabla=open("tabla_simbolos.txt",O_WRONLY | O_CREAT | O_TRUNC,0666)) < 0) { 
		fprintf(stderr, "Error al crear archivo tabla_simbolos.txt"); 
		return -1;
	}
	
	//Crear automatas
	
	automata automatas[cuantosAutomatas];
	automatas[0].funcion = automataComentarios;
	automatas[1].funcion = automataIdentificadores;
	automatas[2].funcion = automataOperadoresLogicos;
	automatas[3].funcion = automataOperadoresComparacion;
	automatas[4].funcion = automataOperadoresAsignacion;
	automatas[5].funcion = automataNumerosReales;
	automatas[6].funcion = automataNumeros;
	automatas[7].funcion = automataPuntuacion;
	automatas[8].funcion = automataCadenaCaracteres;
	automatas[9].funcion = automataOperadoresAgrupacion;
	automatas[10].funcion = automataAritmetico;
	
	reset(automatas);
	
	char c;
	
	while ((c = getc(fuente)) != EOF) {
		
		//fprintf(stdout, "%c",c);
		// fseek(fuente, -1, SEEK_CUR);
		// Si el automata regresa 1 es que llega a estado terminal
		if ((int)c == 10) {
			//fprintf(stdout, "enter!!!!");
		}
		
		if (automatas[0].funcion(&automatas[0],c) == 1) {
			reset(automatas);
			continue;
		}
		if (automatas[1].funcion(&automatas[1],c) == 1) {
			reset(automatas);
			continue;
		}
		if (automatas[2].funcion(&automatas[2],c) == 1) {
			reset(automatas);
			continue;
		}
		if (automatas[3].funcion(&automatas[3],c) == 1) {
			reset(automatas);
			continue;
		}
		if (automatas[4].funcion(&automatas[4],c) == 1) {
			reset(automatas);
			continue;
		}
		if (automatas[5].funcion(&automatas[5],c) == 1) {
			reset(automatas);
			continue;
		}
		if (automatas[6].funcion(&automatas[6],c) == 1) {
			reset(automatas);
			continue;
		}
		
		if (automatas[7].funcion(&automatas[7],c) == 1) {
			reset(automatas);
			continue;
		}
		
		if (automatas[8].funcion(&automatas[8],c) == 1) {
			reset(automatas);
			continue;
		}
		
		if (automatas[9].funcion(&automatas[9],c) == 1) {
			reset(automatas);
			continue;
		}
		
		if (automatas[10].funcion(&automatas[10],c) == 1) {
			reset(automatas);
			continue;
		}
		
		// Si es el fin del archivo dejo de leer.
//		char siguiente; 
//		if((siguiente = getc(fuente)) == EOF){
//			fprintf(stdout, "Siguiente es EOF");
//			break;
//		}else{
//			fseek(fuente, -1, SEEK_CUR);
//		}
	}	
	
//	char asd='s';
//	fprintf(stdout, "%d",(int)asd);
	fclose(fuente);
	
    return 0;
}

