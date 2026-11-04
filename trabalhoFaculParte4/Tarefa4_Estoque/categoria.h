#ifndef CATEGORIA_H
#define CATEGORIA_H

struct Categoria {
    int codigo;
    char nome[30];
};

int buscarCategoriaPorCodigo(struct Categoria categorias[], int total, int codigo);
int cadastrarCategoria(struct Categoria categorias[], int total);
void listarCategorias(struct Categoria categorias[], int total);
int carregarCategorias(struct Categoria categorias[]);
void salvarCategorias(struct Categoria categorias[], int total);

#endif
