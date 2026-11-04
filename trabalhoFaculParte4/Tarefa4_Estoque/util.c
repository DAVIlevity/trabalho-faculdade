#include <stdio.h>
#include <ctype.h>
#include "util.h"

int validarTexto(char texto[]) {
    int i;
    for (i = 0; texto[i] != '\0'; i++)
        if (!isspace((unsigned char)texto[i])) return 1;
    return 0;
}

void limparEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* limpa o restante da linha */
    }
}
