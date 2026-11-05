#ifndef PRODUTO_H
#define PRODUTO_H

#include "categoria.h"

struct Produto {
    int codigo;
    char nome[50];
    char prateleira;
    float preco;
    int quantidade;
    int disponivel;
    int codigoCategoria;
    int ativo;
};

int codigoRegistrado(struct Produto produtos[], int total, int codigo);
int buscarPorCodigo(struct Produto produtos[], int total, int codigo);
int buscarPorNome(struct Produto produtos[], int total, char nome[]);
int cadastrarProduto(struct Produto produtos[], int total,
                     struct Categoria categorias[], int totalCategorias);
void exibirProduto(struct Produto produto,
                   struct Categoria categorias[], int totalCategorias);
void listarProdutos(struct Produto produtos[], int total,
                    struct Categoria categorias[], int totalCategorias);
void pesquisarProduto(struct Produto produtos[], int total,
                      struct Categoria categorias[], int totalCategorias);
void alterarEstoque(struct Produto produtos[], int total);
void removerProduto(struct Produto produtos[], int total);
int carregarProdutos(struct Produto produtos[]);
void salvarProdutos(struct Produto produtos[], int total);

#endif
