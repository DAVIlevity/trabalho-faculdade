#include <stdio.h>
#include <string.h>
#include "categoria.h"
#include "util.h"

int buscarCategoriaPorCodigo(struct Categoria categorias[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++)
        if (categorias[i].codigo == codigo) return i;
    return -1;
}

int cadastrarCategoria(struct Categoria categorias[], int total) {
    struct Categoria c;
    if (total >= 20) {
        printf("Limite de 20 categorias atingido.\n");
        return total;
    }

    do {
        printf("Codigo da categoria (positivo e unico): ");
        if (scanf("%d", &c.codigo) != 1) {
            limparEntrada(); c.codigo = -1;
        } else limparEntrada();
        if (c.codigo <= 0 || buscarCategoriaPorCodigo(categorias, total, c.codigo) != -1)
            printf("Codigo invalido ou ja utilizado.\n");
    } while (c.codigo <= 0 || buscarCategoriaPorCodigo(categorias, total, c.codigo) != -1);

    do {
        printf("Nome da categoria: ");
        if (fgets(c.nome, sizeof(c.nome), stdin) == NULL) c.nome[0] = '\0';
        c.nome[strcspn(c.nome, "\n")] = '\0';
        if (!validarTexto(c.nome)) printf("O nome nao pode ficar vazio.\n");
    } while (!validarTexto(c.nome));

    categorias[total] = c;
    printf("Categoria cadastrada!\n");
    return total + 1;
}

void listarCategorias(struct Categoria categorias[], int total) {
    int i;
    printf("\n--- CATEGORIAS ---\n");
    if (total == 0) {
        printf("Nenhuma categoria cadastrada.\n");
        return;
    }
    for (i = 0; i < total; i++)
        printf("Codigo: %d | Nome: %s\n", categorias[i].codigo, categorias[i].nome);
}

int carregarCategorias(struct Categoria categorias[]) {
    FILE *arquivo = fopen("categorias.txt", "r");
    int total = 0;
    if (arquivo == NULL) return 0;

    while (total < 20 &&
           fscanf(arquivo, "%d;%29[^\n]\n", &categorias[total].codigo,
                  categorias[total].nome) == 2) {
        total++;
    }
    fclose(arquivo);
    return total;
}

void salvarCategorias(struct Categoria categorias[], int total) {
    FILE *arquivo = fopen("categorias.txt", "w");
    int i;
    if (arquivo == NULL) {
        printf("Nao foi possivel salvar categorias.txt\n");
        return;
    }
    for (i = 0; i < total; i++)
        fprintf(arquivo, "%d;%s\n", categorias[i].codigo, categorias[i].nome);
    fclose(arquivo);
}
