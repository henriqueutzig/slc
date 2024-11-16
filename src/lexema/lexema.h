#ifndef _LEXEMA_H_
#define _LEXEMA_H_

typedef enum {
    LITERAL,
    IDENTIFICADOR
} tipo;

typedef struct 
{
    int linha;
    tipo tipo;
    char *valor;
} lexema;

lexema *create_lexema(int linha, char *valor, tipo tipo);
#endif