/*
 *  analex.h
 *  Analizador Lexico
 *
 *  Created by Alan Rodriguez
 *  Ariana Ibañez
 *	Hugo Sanchez
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>

#define cuantosAutomatas 11

// Declaracion de variables globales
int fdLexemas;
int fdTabla;
FILE * fuente;
char escritura[BUFSIZ];

// Arreglo de palabras reservadas (keywords)
char *keywords[24] = {"auto","struct","break","else","switch","case","enum",
					"register","typedef","extern","return","union",
					"const","continue","for","void","default","goto",
					"sizeof","volatile","do","if","static","while"};

// Arreglo de tipos de datos
char *dataType[10] = {"char","int","float","double","short","long","unsigned",
					"signed","enum","const"};

int isComment,isString;

/* Estructura automata, contiene todo lo necesario para cada automata
 * buffer = buffer donde se escribe el lexema
 * estado = estado en el que se encuentra actualmente el automata
 * contiene un apuntador a funcion que recibe la estructura y el caracter que va a leer 
**/ 
typedef struct automata {
	char buffer[BUFSIZ];
	int estado;
	int (*funcion)(struct automata *a,char c);
}automata;


/**
 * Reinicia todos los automatas, pone su estado en 0 y limpia el buffer
 * Pone las variable de comentario y cadena de caracteres en 0
 *
 * @param automata[]
 * @return void
 **/
void reset(automata arr[]){
    int i;
    for(i = 0; i < cuantosAutomatas; i++){
        arr[i].estado = 0;
        memset(arr[i].buffer, '\0', BUFSIZ);
    }
	isComment = 0;
	isString = 0;
}

/**
 * Regresa la ultima posicion donde se encuentra el buffer dado
 *
 * @param char[]
 * @return int
 **/
int getLastIndex(char buffer[]) {
	int i = 0;
	while(buffer[i] != '\0') {
		i++;
		if (i >= (BUFSIZ)) {
			break;
		}
	}
	return i;
}

/**
 * Checa si la cadena de caracteres mostrada es un keyword
 *
 * @param string
 * @return int
 **/
int isKeyword(char str[]) {
	int size,i;
	size = sizeof(keywords)/sizeof(keywords[1]);
	for (i=0; i<size; i++) {
		if (strcmp(str, keywords[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

/**
 * Checa si la cadena de caracteres mostrada es un tipo de dato
 *
 * @param string
 * @return int
 **/
int isDataType(char str[]) {
	int size,i;
	size = sizeof(dataType)/sizeof(dataType[1]);
	for (i=0; i<size; i++) {
		if (strcmp(str, dataType[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

/**
 * Escribe en el archivo de texto lexema.txt
 *
 * @param char*
 * @param char*
 * @return void
 **/
void lexema(char *l, char *t){
    // Limpia buffer
	memset(escritura, '\0', BUFSIZ);
	// Escribo buffer
    sprintf(escritura,"%s\t%s\n", l, t);
    write(fdLexemas, escritura, BUFSIZ);
}

/**
 * Escribe en el archivo de texto tabla_simbolos.txt
 *
 * @param char*
 * @return void
 **/
void tabla(char *l){
    // Limpia buffer
	memset(escritura, '\0', BUFSIZ);
	// Escribo buffer
    sprintf(escritura,"%s\n", l);
    write(fdTabla, escritura, BUFSIZ);
}


/****************************************************
 *													*
 *					Automatas						*
 *													*
 ***************************************************/

/**
 * Automata para numeros reales
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataNumerosReales(automata *a , char c) {
	if (isComment == 1 || isString == 1) {(*a).estado = -1;}
	//fprintf(stdout, "reales estado: %d",(*a).estado);
	//fprintf(stdout, "Realesestado:%d char:%c\n",(*a).estado,c);
	switch ((*a).estado) {
		case 0:
			if (c == '+' || c == '-') {
				// Imprime en buffer
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				// Automatas regresan 0 si no es estado final
				return 0;
			}
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			if (c == '.') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 3;
				return 0;
			}
			return 0;
			break;
		case 1:
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			}
			if (c == '.') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 3;
				return 0;
			}
			return 0;
			break;
		case 2:
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			
			return 0;
			break;
		case 3:
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 4;
				return 0;
			}
			return 0;
			break;
		case 4:
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			} else {
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Numero Real");
				lexema((*a).buffer, "Numero Real");
				// Como leo un caracter despues y no es digito, regreso el apuntador
				fseek(fuente, -1, SEEK_CUR);
				return 1;
			}
			break;
		default:
			return 0;
			break;
	}
}

/**
 * Automata para numeros naturales
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataNumeros(automata *a , char c) {
	if (isComment == 1 || isString == 1) {(*a).estado = -1;}
	//fprintf(stdout, "Numerosestado:%d char:%c\n",(*a).estado,c);
	switch ((*a).estado) {
		case 0:
			if (c == '+' || c == '-') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				return 0;
			}
			return 0;
			break;
		case 1:
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				return 0;
			}
			// Si el siguiente valor no es digito entonces ya no es un num. natural
			(*a).estado = -1;

			return 0;
			break;
		case 2:
			if (isdigit(c)) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			} else if (c != '.'){
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Numero Natural");
				lexema((*a).buffer, "Numero Natural");
				fseek(fuente, -1, SEEK_CUR);
				return 1;
			} else {
				// Posiblemente un real
				(*a).estado = -1;
				return 0;
			}

			return 0;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * Automata para identificadores
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataIdentificadores(automata *a , char c) {
	//fprintf(stdout, "Idestado:%d char:%c\n",(*a).estado,c);
	if (isComment == 1 || isString == 1) {(*a).estado = -1;}
	switch ((*a).estado) {
		case 0:
			if (isalpha(c) || c == '_') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			if (c == ' ' || c == '\n') {
				return 0;
			}
			// Si comienza con algo mas 
			(*a).estado = -1;
			return 0;
			break;
		case 1:
			if (isdigit(c) || isalpha(c) || c == '_') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			} else {
				if (isKeyword((*a).buffer)) {
					fprintf(stdout, "%s\t%s\n",(*a).buffer,"Palabra reservada");
					lexema((*a).buffer, "Palabra reservada");
				} else if (isDataType((*a).buffer)) {
					fprintf(stdout, "%s\t%s\n",(*a).buffer,"Palabra reservada - Tipo de dato");
					lexema((*a).buffer, "Palabra reservada - Tipo de dato");
				} else {
					fprintf(stdout, "%s\t%s\n",(*a).buffer,"Identificador");
					lexema((*a).buffer, "Identificador");
				}
				tabla((*a).buffer);
				fseek(fuente, -1, SEEK_CUR);
				return 1;
			}
			return 0;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * Automata para comentarios
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataComentarios(automata *a, char c){
	//fprintf(stdout, "Comentestado:%d char:%c\n",(*a).estado,c);
	switch((*a).estado){
        case 0:
            if(c == '/'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
                (*a).estado = 1;
                return 0;
            }
        	return 0;
        	break;
    	case 1:
			if(c == '/'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
    		 	(*a).estado = 2;
				isComment = 1;
    		 	return 0;
    		}if(c == '*'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
    			(*a).estado = 3;
				isComment = 1;
    			return 0;
    		}
            return 0;
            break; 
        case 2:
        	if(((int)c) != 10){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				return 0;
        	} else {
	      		//Estado final de comentario corto
      		    fprintf(stdout, "%s\t%s\n",(*a).buffer,"Comentario");
				lexema((*a).buffer, "Comentario");
				return 1;
      		}
			return 0;
			break;
      	case 3:
      		if(c == '*'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
      			(*a).estado = 4;
      			return 0;
      		} else {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
      			(*a).estado = 3;
      			return 0;
      		}
			return 0;
			break;
      	case 4:
			if(c == '*'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
      			(*a).estado = 4;
      			return 0;
      		}
			if (c == '/') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Comentario Largo");
				lexema((*a).buffer, "Comentario Largo");
				return 1;
			} else {
				printf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
      			(*a).estado = 3;
      			return 0;
			}

			return 0;
			break;
        default:
			return 0;
			break;
	}
}

/**
 * Automata para operadores de puntuacion
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataPuntuacion(automata *a , char c){
	if (isComment == 1 || isString == 1) {(*a).estado = -1;}
	//fprintf(stdout, "Puntgestado:%d char:%c\n",(*a).estado,c);
    switch((*a).estado){
        case 0:
            if(c == ';' || c == '.' || c== ','){
                sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado=1;
                return 0;
            }
			if (c == ' ' || c == '\n') {
				return 0;
			}
            return 0;
            break;
        case 1:
            if(((int)c)>=48 && ((int)c)<=57){
                //noEsPunto=1;
                return 0;
            }else{
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Signo de puntuacion");
				lexema((*a).buffer, "Signo de puntuacion");
				fseek(fuente, -1, SEEK_CUR);
				return 1;
            } 
            return 0;
            break;   
		default:
			return 0;
			break;
    }
	return 0;
} 

/**
 * Automata para operadores de asignacion (= += -= *= /= %=)
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataOperadoresAsignacion(automata *a, char c){
	if (isComment == 1) {(*a).estado = -1;}
	//fprintf(stdout, "Asigestado:%d char:%c\n",(*a).estado,c);
    switch((*a).estado){
		case 0:
			if(c == '*' || c == '/' || c == '%'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			if(c == '='){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				return 0;
			}
			if (c == '+') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 3;
				return 0;
			}
			if (c == '-') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 4;
				return 0;
			}
			if (c == ' ' || c == '\n') {
				return 0;
			}
			(*a).estado = -1;
			break;
		case 1:
			if(c == '='){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de asignacion");
				lexema((*a).buffer, "Operador de asignacion");
				return 1;
			}
			(*a).estado = -1;
			return 0;
			break;
		case 2:
			fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de asignacion");
			lexema((*a).buffer, "Operador de asignacion");
			fseek(fuente, -1, SEEK_CUR);
			return 1;
			break;
		case 3:
			if(c == '+'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de asignacion");
				lexema((*a).buffer, "Operador de asignacion");
				return 1;
			}
			(*a).estado = -1;
			return 0;
			break;
		case 4:
			if(c == '-'){
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de asignacion");
				lexema((*a).buffer, "Operador de asignacion");
				return 1;
			}
			(*a).estado = -1;
			return 0;
			break;
	}
	return 0;
}

/**
 * Automata para operadores de agrupacion
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataOperadoresAgrupacion(automata *a , char c) {
	if (isComment == 1) {(*a).estado = -1;}
	switch ((*a).estado) {
		case 0:
			if ((c == '(') || (c == ')') || (c == '[') || (c == ']') || (c == '{') || (c == '}')) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de Agrupacion");
				lexema((*a).buffer, "Operador de Agrupacion");
				return 1;
			}
			return 0;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * Automata para operadores aritmeticos
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataAritmetico(automata *a , char c) {
	if (isComment == 1 || isString == 1) {(*a).estado = -1;}
	//fprintf(stdout, "Arigestado:%d char:%c\n",(*a).estado,c);
	switch ((*a).estado) {
		case 0:
			if (((c == '+') || (c == '-') || (c == '/') || (c == '%')) && isComment == 0) {
				(*a).estado = 1;
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			}
			if (c == '*') {
				(*a).estado = 2;
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			}
			return 0;
		case 1:
			if ((isdigit(c) || c == ' ' || isalpha(c)) && isComment == 0) {
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador Aritmetico");
				lexema((*a).buffer, "Operador Aritmetico");
				fseek(fuente, -1, SEEK_CUR);
				return 1;
			}
			return 0;
			break;
		case 2:
			if ((isdigit(c) || c == ' ' || isalpha(c)) && isComment == 0) {
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador Aritmetico o Apuntador");
				lexema((*a).buffer, "Operador Aritmetico o Apuntador");
				fseek(fuente, -1, SEEK_CUR);
				return 1;
			}
			return 0;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * Automata para cadenas de caracteres
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataCadenaCaracteres(automata *a , char c) {
	//fprintf(stdout, "Cadenaestado:%d char:%c\n",(*a).estado,c);
	if (isComment == 1) {(*a).estado = -1;}
	switch ((*a).estado) {
		case 0:
			// Checa " y '
			if ((int)c == 34) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				isString = 1;
				return 0;
			}
			if ((int)c == 39) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				isString = 1;
				return 0;
			}
			if (c == ' ' || c == '\n') {
				return 0;
			}
			// Si comienza con algo mas invalido automata
			(*a).estado = -1;
			break;
		case 1:
			if ((int)c != 34) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			} else {
				// Si es " que cierra
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Cadena de caracteres");
				lexema((*a).buffer, "Cadena de caracteres");
				return 1;
			}
			return 0;
			break;
		case 2:
			if ((int)c != 39) {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				return 0;
			} else {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Char");
				lexema((*a).buffer, "Char");
				return 1;
			}
			return 0;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * Automata para operadores de comparacion (<,>,>=,<=,==,!=)
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataOperadoresComparacion(automata *a , char c) {
	if (isComment == 1) {(*a).estado = -1;}
	//fprintf(stdout, "Asigestado:%d char:%c\n",(*a).estado,c);
	switch ((*a).estado) {
		case 0:
			if (c == '=') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			if (c == '!') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			if (c == '>') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				return 0;
			}
			if (c == '<') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				return 0;
			}
			if (c == ' ' || c == '\n') {
				return 0;
			}
			// Si comienza con algo mas 
			(*a).estado = -1;
			return 0;
			break;
		case 1:
			if (c == '=') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de comparacion");
				lexema((*a).buffer, "Operador de comparacion");
				return 1;
			}
			(*a).estado = -1;
			return 0;
			break;
		case 2:
			if (c == '=') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de comparacion");
				lexema((*a).buffer, "Operador de comparacion");
				return 1;
			} else {
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador de comparacion");
				lexema((*a).buffer, "Operador de comparacion");
				fseek(fuente, -1, SEEK_CUR);
				return 1;
			}
			return 0;
			break;
		default:
			return 0;
			break;
	}
}

/**
 * Automata para operadores logicos (&&,||,!)
 *
 * @param automata
 * @param char
 * @return int
 **/
int automataOperadoresLogicos(automata *a , char c) {
	//fprintf(stdout, "Loggestado:%d char:%c\n",(*a).estado,c);
	if (isComment == 1) {(*a).estado = -1;}
	switch ((*a).estado) {
		case 0:
			if (c == '&') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 1;
				return 0;
			}
			if (c == '|') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				(*a).estado = 2;
				return 0;
			}
			if (c == '!') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador logico");
				lexema((*a).buffer, "Operador logico");
				return 1;
			}
			if (c == ' ' || c == '\n') {
				return 0;
			}
			// Si comienza con algo mas 
			(*a).estado = -1;
			return 0;
			break;
		case 1:
			if (c == '&') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador logico");
				lexema((*a).buffer, "Operador logico");
				return 1;
			}
			(*a).estado = -1;
			return 0;
			break;
		case 2:
			if (c == '|') {
				sprintf((*a).buffer+getLastIndex((*a).buffer), "%c", c);
				fprintf(stdout, "%s\t%s\n",(*a).buffer,"Operador logico");
				lexema((*a).buffer, "Operador logico");
				return 1;
			}
			(*a).estado = -1;
			return 0;
			break;
		default:
			return 0;
			break;
	}
}
