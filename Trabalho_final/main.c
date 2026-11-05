#include <stdio.h>
#include "produto.h"
#include "categoria.h"
#include "util.h"

int main(void) {
    struct Produto produtos[50];
    struct Categoria categorias[20];
    int totalProdutos = carregarProdutos(produtos);
    int totalCategorias = carregarCategorias(categorias);
    int opcao, opcaoCategoria;

    do {
        printf("\n========== SISTEMA DE ESTOQUE ==========\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Listar Produtos\n");
        printf("3 - Pesquisar Produto\n");
        printf("4 - Alterar Estoque de um Produto\n");
        printf("5 - Remover Produto\n");
        printf("6 - Categorias\n");
        printf("7 - Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            limparEntrada();
            opcao = 0;
            continue;
        }
        limparEntrada();

        switch (opcao) {
            case 1:
                totalProdutos = cadastrarProduto(produtos, totalProdutos,
                                                  categorias, totalCategorias);
                break;
            case 2:
                listarProdutos(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 3:
                pesquisarProduto(produtos, totalProdutos, categorias, totalCategorias);
                break;
            case 4:
                alterarEstoque(produtos, totalProdutos);
                break;
            case 5:
                removerProduto(produtos, totalProdutos);
                break;
            case 6:
                do {
                    printf("\n========== CATEGORIAS ==========\n");
                    printf("1 - Cadastrar Categoria\n");
                    printf("2 - Listar Categorias\n");
                    printf("3 - Voltar\n");
                    printf("Opcao: ");
                    if (scanf("%d", &opcaoCategoria) != 1) {
                        printf("Opcao invalida.\n");
                        limparEntrada();
                        opcaoCategoria = 0;
                        continue;
                    }
                    limparEntrada();

                    if (opcaoCategoria == 1) {
                        totalCategorias = cadastrarCategoria(categorias, totalCategorias);
                    } else if (opcaoCategoria == 2) {
                        listarCategorias(categorias, totalCategorias);
                    } else if (opcaoCategoria != 3) {
                        printf("Escolha entre 1 e 3.\n");
                    }
                } while (opcaoCategoria != 3);
                break;
            case 7:
                salvarProdutos(produtos, totalProdutos);
                salvarCategorias(categorias, totalCategorias);
                printf("Dados salvos. Encerrando...\n");
                break;
            default:
                printf("Escolha uma opcao entre 1 e 7.\n");
        }
    } while (opcao != 7);

    return 0;
}
