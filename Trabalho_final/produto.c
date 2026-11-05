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
        printf("Categoria: %s\n", categorias[pos].nome);
    else
        printf("Categoria: nao encontrada (codigo %d)\n", produto.codigoCategoria);
    printf("----------------------------------------\n");
}

int codigoRegistrado(struct Produto produtos[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].codigo == codigo) return 1;
    return 0;
}

int buscarPorCodigo(struct Produto produtos[], int total, int codigo) {
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].codigo == codigo && produtos[i].ativo == 1) return i;
    return -1;
}

int buscarPorNome(struct Produto produtos[], int total, char nome[]) {
    int i;
    for (i = 0; i < total; i++)
        if (produtos[i].ativo == 1 && strcmp(produtos[i].nome, nome) == 0) return i;
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
        if (!numeroPositivo(p.codigo) || codigoRegistrado(produtos, total, p.codigo))
            printf("Codigo invalido ou ja cadastrado.\n");
    } while (!numeroPositivo(p.codigo) || codigoRegistrado(produtos, total, p.codigo));

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
        if (!numeroNaoNegativo(p.quantidade)) printf("Quantidade invalida.\n");
    } while (!numeroNaoNegativo(p.quantidade));

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
    p.ativo = 1;
    produtos[total] = p;
    printf("Produto cadastrado!\n");
    exibirProduto(produtos[total], categorias, totalCategorias);
    return total + 1;
}

void listarProdutos(struct Produto produtos[], int total,
                    struct Categoria categorias[], int totalCategorias) {
    int i, encontrados = 0;
    for (i = 0; i < total; i++) {
        if (produtos[i].ativo == 1) {
            exibirProduto(produtos[i], categorias, totalCategorias);
            encontrados++;
        }
    }
    if (encontrados == 0) printf("Nenhum produto ativo cadastrado.\n");
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

void alterarEstoque(struct Produto produtos[], int total) {
    int codigo, pos, operacao, quantidade;
    printf("Codigo do produto: ");
    if (scanf("%d", &codigo) != 1) { limparEntrada(); printf("Codigo invalido.\n"); return; }
    limparEntrada();
    pos = buscarPorCodigo(produtos, total, codigo);
    if (pos == -1) { printf("Produto nao encontrado.\n"); return; }
    printf("Produto: %s | Estoque atual: %d\n", produtos[pos].nome, produtos[pos].quantidade);
    printf("1 - Entrada\n2 - Saida\nOpcao: ");
    if (scanf("%d", &operacao) != 1) { limparEntrada(); printf("Opcao invalida.\n"); return; }
    limparEntrada();
    if (operacao != 1 && operacao != 2) { printf("Opcao invalida.\n"); return; }
    printf("Quantidade: ");
    if (scanf("%d", &quantidade) != 1) { limparEntrada(); printf("Quantidade invalida.\n"); return; }
    limparEntrada();
    if (quantidade <= 0) { printf("A quantidade deve ser positiva.\n"); return; }
    if (operacao == 2 && quantidade > produtos[pos].quantidade) {
        printf("Estoque insuficiente. Operacao cancelada.\n"); return;
    }
    if (operacao == 1) {
        if (quantidade > 2147483647 - produtos[pos].quantidade) {
            printf("Quantidade excede o limite permitido.\n"); return;
        }
        produtos[pos].quantidade += quantidade;
    } else produtos[pos].quantidade -= quantidade;
    produtos[pos].disponivel = produtos[pos].quantidade > 0 ? 1 : 0;
    printf("Estoque atualizado: %d | Disponivel: %s\n",
           produtos[pos].quantidade, produtos[pos].disponivel ? "Sim" : "Nao");
}

void removerProduto(struct Produto produtos[], int total) {
    int codigo, pos;
    printf("Codigo do produto para remover: ");
    if (scanf("%d", &codigo) != 1) { limparEntrada(); printf("Codigo invalido.\n"); return; }
    limparEntrada();
    pos = buscarPorCodigo(produtos, total, codigo);
    if (pos == -1) { printf("Produto nao encontrado ou ja removido.\n"); return; }
    produtos[pos].ativo = 0;
    printf("Produto removido logicamente.\n");
}

int carregarProdutos(struct Produto produtos[]) {
    FILE *arquivo = fopen("produtos.txt", "r");
    char linha[256];
    int total = 0, campos;
    if (arquivo == NULL) return 0;
    while (total < 50 && fgets(linha, sizeof(linha), arquivo)) {
        campos = sscanf(linha, "%d;%49[^;];%c;%f;%d;%d;%d;%d",
                &produtos[total].codigo, produtos[total].nome,
                &produtos[total].prateleira, &produtos[total].preco,
                &produtos[total].quantidade, &produtos[total].disponivel,
                &produtos[total].codigoCategoria, &produtos[total].ativo);
        if (campos == 7 || campos == 8) {
            if (campos == 7) produtos[total].ativo = 1; 
            produtos[total].disponivel = produtos[total].quantidade > 0 ? 1 : 0;
            total++;
        }
    }
    fclose(arquivo);
    return total;
}

void salvarProdutos(struct Produto produtos[], int total) {
    FILE *arquivo = fopen("produtos.txt", "w");
    int i;
    if (arquivo == NULL) { printf("Erro ao salvar produtos.txt\n"); return; }
    for (i = 0; i < total; i++)
        fprintf(arquivo, "%d;%s;%c;%.2f;%d;%d;%d;%d\n",
                produtos[i].codigo, produtos[i].nome, produtos[i].prateleira,
                produtos[i].preco, produtos[i].quantidade,
                produtos[i].disponivel, produtos[i].codigoCategoria, produtos[i].ativo);
    fclose(arquivo);
}
