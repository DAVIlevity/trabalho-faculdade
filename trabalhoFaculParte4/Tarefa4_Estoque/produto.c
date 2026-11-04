#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "produto.h"
#include "util.h"

void exibirProduto(struct Produto produto, struct Categoria categorias[], int totalCategorias) {
    int pos = buscarCategoriaPorCodigo(categorias, totalCategorias, produto.codigoCategoria);

    printf("\n----------------------------------------\n");
    printf("Codigo: %d\n", produto.codigo);
    printf("Nome: %s\n", produto.nome);
    printf("Prateleira: %c\n", produto.prateleira);
    printf("Preco unitario: R$ %.2f\n", produto.preco);
    printf("Quantidade: %d\n", produto.quantidade);
    printf("Disponivel: %s\n", produto.disponivel ? "Sim" : "Nao");
    if (pos != -1)
        printf("Categoria: %s (codigo %d)\n", categorias[pos].nome, produto.codigoCategoria);
    else
        printf("Categoria: nao encontrada (codigo %d)\n", produto.codigoCategoria);
    printf("----------------------------------------\n");
}

int buscarPorCodigo(struct Produto produtos[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].codigo == codigo) return i;
    return -1;
}

int buscarPorNome(struct Produto produtos[], int total, char nome[]) {
    int i;
    for (i = 0; i < total; i++)
        if (strcmp(produtos[i].nome, nome) == 0) return i;
    return -1;
}

int cadastrarProduto(struct Produto produtos[], int total,
                     struct Categoria categorias[], int totalCategorias) {
    struct Produto p;
    int codigoCategoria;

    if (total >= 50) {
        printf("Limite de produtos atingido.\n");
        return total;
    }
    if (totalCategorias == 0) {
        printf("Cadastre uma categoria antes de cadastrar produtos.\n");
        return total;
    }

    printf("\n--- CADASTRO DE PRODUTO ---\n");
    do {
        printf("Codigo do produto (positivo): ");
        if (scanf("%d", &p.codigo) != 1) {
            limparEntrada();
            p.codigo = -1;
        } else limparEntrada();
        if (p.codigo <= 0 || buscarPorCodigo(produtos, total, p.codigo) != -1)
            printf("Codigo invalido ou ja cadastrado.\n");
    } while (p.codigo <= 0 || buscarPorCodigo(produtos, total, p.codigo) != -1);

    do {
        printf("Nome do produto: ");
        if (fgets(p.nome, sizeof(p.nome), stdin) == NULL) p.nome[0] = '\0';
        p.nome[strcspn(p.nome, "\n")] = '\0';
        if (!validarTexto(p.nome)) printf("O nome nao pode ficar vazio.\n");
    } while (!validarTexto(p.nome));

    do {
        printf("Prateleira (A, B, C ou D): ");
        if (scanf(" %c", &p.prateleira) != 1) p.prateleira = '\0';
        limparEntrada();
        p.prateleira = (char)toupper((unsigned char)p.prateleira);
        if (p.prateleira < 'A' || p.prateleira > 'D')
            printf("Escolha A, B, C ou D.\n");
    } while (p.prateleira < 'A' || p.prateleira > 'D');

    do {
        printf("Preco maior que zero: ");
        if (scanf("%f", &p.preco) != 1) {
            limparEntrada(); p.preco = -1;
        } else limparEntrada();
        if (p.preco <= 0) printf("Preco invalido.\n");
    } while (p.preco <= 0);

    do {
        printf("Quantidade (zero ou maior): ");
        if (scanf("%d", &p.quantidade) != 1) {
            limparEntrada(); p.quantidade = -1;
        } else limparEntrada();
        if (p.quantidade < 0) printf("Quantidade invalida.\n");
    } while (p.quantidade < 0);

    listarCategorias(categorias, totalCategorias);
    do {
        printf("Codigo da categoria: ");
        if (scanf("%d", &codigoCategoria) != 1) {
            limparEntrada(); codigoCategoria = -1;
        } else limparEntrada();
        if (buscarCategoriaPorCodigo(categorias, totalCategorias, codigoCategoria) == -1)
            printf("Categoria nao existe. Escolha um codigo listado.\n");
    } while (buscarCategoriaPorCodigo(categorias, totalCategorias, codigoCategoria) == -1);

    p.codigoCategoria = codigoCategoria;
    p.disponivel = p.quantidade > 0 ? 1 : 0;
    produtos[total] = p;
    printf("Produto cadastrado!\n");
    exibirProduto(produtos[total], categorias, totalCategorias);
    return total + 1;
}

void listarProdutos(struct Produto produtos[], int total,
                    struct Categoria categorias[], int totalCategorias) {
    int i;
    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    for (i = 0; i < total; i++)
        exibirProduto(produtos[i], categorias, totalCategorias);
}

void pesquisarProduto(struct Produto produtos[], int total,
                      struct Categoria categorias[], int totalCategorias) {
    int tipo, codigo, pos;
    char nome[50];

    printf("1 - Pesquisar por codigo\n2 - Pesquisar por nome\nOpcao: ");
    if (scanf("%d", &tipo) != 1) {
        limparEntrada(); printf("Opcao invalida.\n"); return;
    }
    limparEntrada();

    if (tipo == 1) {
        printf("Codigo: ");
        if (scanf("%d", &codigo) != 1) {
            limparEntrada(); printf("Codigo invalido.\n"); return;
        }
        limparEntrada();
        pos = buscarPorCodigo(produtos, total, codigo);
    } else if (tipo == 2) {
        printf("Nome: ");
        if (fgets(nome, sizeof(nome), stdin) == NULL) nome[0] = '\0';
        nome[strcspn(nome, "\n")] = '\0';
        pos = buscarPorNome(produtos, total, nome);
    } else {
        printf("Opcao invalida.\n");
        return;
    }

    if (pos == -1) printf("Produto nao encontrado.\n");
    else exibirProduto(produtos[pos], categorias, totalCategorias);
}

int carregarProdutos(struct Produto produtos[]) {
    FILE *arquivo = fopen("produtos.txt", "r");
    int total = 0;
    if (arquivo == NULL) return 0;

    while (total < 50 &&
           fscanf(arquivo, "%d;%49[^;];%c;%f;%d;%d;%d\n",
                  &produtos[total].codigo, produtos[total].nome,
                  &produtos[total].prateleira, &produtos[total].preco,
                  &produtos[total].quantidade, &produtos[total].disponivel,
                  &produtos[total].codigoCategoria) == 7) {
        total++;
    }
    fclose(arquivo);
    return total;
}

void salvarProdutos(struct Produto produtos[], int total) {
    FILE *arquivo = fopen("produtos.txt", "w");
    int i;
    if (arquivo == NULL) {
        printf("Nao foi possivel salvar produtos.txt\n");
        return;
    }
    for (i = 0; i < total; i++)
        fprintf(arquivo, "%d;%s;%c;%.2f;%d;%d;%d\n",
                produtos[i].codigo, produtos[i].nome, produtos[i].prateleira,
                produtos[i].preco, produtos[i].quantidade,
                produtos[i].disponivel, produtos[i].codigoCategoria);
    fclose(arquivo);
}
