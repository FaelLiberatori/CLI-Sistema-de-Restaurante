#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tela.h"
#include "mesas.h"
#include "garcons.h"
#include "pratos.h"
#define TAMANHO_COLUNA 22
#define TAMANHO_LINHA 68
#define TAMANHO_TELA 1518 // TAMANHO_LINHAS * TAMANHO_COLUNAS + TAMANHO_COLUNAS= 1518

static void limparTela()
{
    #ifdef _WIN32 // Windows
        system("cls");
    #elif __unix__ // Linux, Mac, BSD, etc.
        system("clear");
    #else // ???
        printf("\n\nDesculpe, sua tela não pode ser limpa pois não conseguimos detectar seus sistema operacional.\n\n");
    #endif
}

static int contaCaracterDuplo (char frase[])
{
    // Caracteres acentuadas não fazem parte da tabela ASCII padrão e, por isso,
    // ocupam duas variáveis char. Esta função retorna a quantidade desses caracter.
    // Necessário "corrigir" a contagem printf, já que ele contaria 2 espaços usados.

    int tamanho = 0;
    char especial[] = "ÁÉÍÓÚáéíóúÂÊÔâêôÀàÜüÇçÑñÃÕãõª°";
    char * pch = strpbrk (frase, especial);

    while (pch != NULL)
    {
        tamanho ++;
        pch = strpbrk (pch+2, especial);
    }

    return tamanho;
}

static void centralizaTexto (char appendPara[], char centraliza[], int pontilhado)
{
    // Asumme que texto tenha o tamanho de, no mínimo, duas linhas convencional.
    // Adiciona ao final do appendPara uma linha padrão com a palavra centraliza
    // centralizada no meio. A linha será preenchida com - se pontilhado for true
    // ou espaço ' ' se pontilhado for false.

    int tamanhoExtra = contaCaracterDuplo(centraliza);
    int tamanho = strlen(centraliza) - tamanhoExtra;
    char aux[TAMANHO_LINHA*2];

    snprintf (aux, sizeof (char)*(TAMANHO_LINHA*2), "|%*s%*s", (TAMANHO_LINHA+tamanhoExtra*2)/2+(tamanho-1)/2,
              centraliza, TAMANHO_LINHA/2-(tamanho-1)/2, " |\n");

    if (pontilhado)
    {
        int tamAux = strlen(aux);

        for (int i = 1; i < tamAux-1; i++)
            if ((aux[i-1] == ' ' || aux[i-1] == '-') && aux[i] == ' '
                    && (aux[i+1] == ' ' || aux[i+1] == '-'))
                aux[i] = '-';
    }

    strcat (appendPara, aux);
}

void imprimirTela (char tituloTela[], char telaInfo[], char tituloMenu[], char menuInfo[], char inputInfo[])
{
    char tela[TAMANHO_TELA+2] = "";
    char telaInfoLocal[TAMANHO_TELA+2]; // É preciso uma variável local para usar o strtok
    strcpy(telaInfoLocal, telaInfo);

    centralizaTexto (tela, "Restaurante", 1);
    centralizaTexto (tela, tituloTela, 1);

    int tamanhoTelaInfo = strlen(telaInfo);
    int linhasTelaInfo = 0;

    for (int i = 0; i < tamanhoTelaInfo; i++)
        if (telaInfo[i] == '\n')
            linhasTelaInfo++;

    char *linhaAtual = strtok(telaInfoLocal, "\n");

    for (int i = 0; i < TAMANHO_COLUNA-9; i++)
    {
        if (i > (TAMANHO_COLUNA-8)/2 - (linhasTelaInfo+1)/2 && linhasTelaInfo > 0 && linhaAtual != NULL)
        {
            centralizaTexto(tela, linhaAtual, 0);
            linhaAtual = strtok(NULL, "\n");
            linhasTelaInfo--;
        }
        else
            centralizaTexto(tela, " ", 0);
    }

    centralizaTexto(tela, "-", 1);
    centralizaTexto(tela, tituloMenu, 0);
    centralizaTexto(tela, "-", 1);
    strcpy(telaInfoLocal, menuInfo);
    linhaAtual = strtok(telaInfoLocal, "\n");

    if (strcmp(linhaAtual, menuInfo) == 0)
    {
        centralizaTexto(tela, menuInfo, 0);
        centralizaTexto(tela, " ", 0);
    }
    else
    {
        char aux[TAMANHO_LINHA*2];
        strcpy(aux, linhaAtual);
        centralizaTexto(tela, aux, 0);
        linhaAtual = strtok(NULL, "\n");
        strcpy(aux, linhaAtual);
        centralizaTexto(tela, aux, 0);
    }

    centralizaTexto (tela, "-", 1);
    strcat (tela, inputInfo);

    limparTela();
    fputs (tela, stdout);
}

void telaInicio (int erro)
{
    char menuInfo[TAMANHO_LINHA];

    if (erro)
        strcpy (menuInfo, "ERRO: a opção digitada não existe!");
    else
        strcpy (menuInfo, "Opções Iniciais");

    imprimirTela("Início", "Bem-Vindo ao Sistema do Restautante!\n \nO que você deseja fazer?\n",
                 menuInfo, "(1) Consultar Mesas (2) Consultar Garçons (3) Consultar Pratos\n(9) Sair", "|  Digite o número da opção desejada: ");
}

void telaConsultaMesa (struct mesasNo *mesas, int *descer, int erro)
{
    char tela[TAMANHO_TELA] = "";
    char linha[TAMANHO_LINHA*2] = "";
    char menuInfo[TAMANHO_LINHA];
    int tabulacao = 20;
    int mesaCount = 0;
    int mesasQuant = 0;
    int descerLocal = *descer;

    mesas = primeiraMesa(&mesas);
    mesasQuant = contaMesas(mesas);

    if (mesas != NULL)
        snprintf(linha, sizeof(linha), "%s%*s%*s", "Número", tabulacao-1, "Garçon    ", tabulacao+1, "Valor Total");
    else
        snprintf(linha, sizeof(linha), "Não existe nenhuma mesa ocupada.\n \n"
                                       "Note que você pode adicionar uma mesa através do menu abaixo.");
    strcat(tela, linha);
    strcat(tela, "\n \n");

    if (descerLocal < 0)
    {
        strcpy (menuInfo, "ERRO: não há mais mesas para subir!");
        descerLocal = *descer = 0;
    }
    else if (erro)
        strcpy (menuInfo, "ERRO: a opção digitada não existe!");
    else
        strcpy (menuInfo, "Opções Mesa");

    while (mesas != NULL && mesaCount < 9)
    {
        if (descerLocal <= 0)
        {
            char id[4] = "";
            char valor[10] = "";
            int tamanhoExtra = contaCaracterDuplo(mesas->garcon->nome);
            snprintf(id, sizeof (id), "%i", mesas->id);
            snprintf(valor, sizeof (valor), "R$ %.2f", mesas->valorTotal);

            snprintf(linha, sizeof(linha), "%s%*s", id, tabulacao+tamanhoExtra, mesas->garcon->nome);
            strcat(tela, linha);
            snprintf(linha, sizeof(linha), "%*s", tabulacao, valor);
            strcat(tela, linha);
            strcat(tela, "\n");
            mesas = mesas->proximo;
            mesaCount++;
        }
        else
        {
            mesas = mesas->proximo;

            if (mesas != NULL && mesasQuant >= 9 + descerLocal)
                descerLocal -= 1;
            else
            {
                strcpy (menuInfo, "ERRO: não há mais mesas para descer!");
                mesas = mesas->anterior;
                *descer = descerLocal -= 1;
            }
        }
    }


    imprimirTela("Mesas", tela, menuInfo, "(1) Adicionar (2) Fechar (3) Add. Pedido (4) Consult. Pedido\n"
                                          "(7) Subir (8) Descer (9) Voltar", "|  Digite o número da opção desejada: ");
}

void telaConsultaGarcons (struct garconsNo *garcons, int *descer, int erro)
{
    char tela[TAMANHO_TELA] = "";
    char linha[TAMANHO_LINHA*2] = "";
    char menuInfo[TAMANHO_LINHA];
    int tabulacao = 20;
    int garconCount = 0;
    int garconQuant = 0;
    int descerLocal = *descer;

    garcons = primeiroGarcom(&garcons);
    garconQuant = contaGarcons(garcons);

    if (garcons != NULL)
        snprintf(linha, sizeof(linha), "%s%*s", "Número", tabulacao-1, "Nome       ");
    else
        snprintf(linha, sizeof(linha), "Não existe nenhuma garçon cadastrado.\n \n"
                                       "Note que você pode adicionar um garçon através do menu abaixo.");
    strcat(tela, linha);
    strcat(tela, "\n \n");

    if (descerLocal < 0)
    {
        strcpy (menuInfo, "ERRO: não há mais garçons para subir!");
        descerLocal = *descer = 0;
    }
    else if (erro)
        strcpy (menuInfo, "ERRO: a opção digitada não existe!");
    else
        strcpy (menuInfo, "Opções Mesa");

    while (garcons != NULL && garconCount < 9)
    {
        if (descerLocal <= 0)
        {
            char id[4] = "";
            int tamanhoExtra = contaCaracterDuplo(garcons->nome);
            snprintf(id, sizeof (id), "%i", garcons->id);

            snprintf(linha, sizeof(linha), "%s%*s", id, tabulacao+tamanhoExtra, garcons->nome);
            strcat(tela, linha);
            strcat(tela, "\n");
            garcons = garcons->proximo;
            garconCount++;
        }
        else
        {
            garcons = garcons->proximo;

            if (garcons != NULL && garconQuant >= 9 + descerLocal)
                descerLocal -= 1;
            else
            {
                strcpy (menuInfo, "ERRO: não há mais garçons para descer!");
                garcons = garcons->anterior;
                *descer = descerLocal -= 1;
            }
        }
    }


    imprimirTela("Garçons", tela, menuInfo, "(1) Adicionar (2) Excluir\n"
                                          "(7) Subir (8) Descer (9) Voltar", "|  Digite o número da opção desejada: ");
}

void telaConsultaPratos (struct pratosNo *pratos, int *descer, int erro)
{
    char tela[TAMANHO_TELA] = "";
    char linha[TAMANHO_LINHA*2] = "";
    char menuInfo[TAMANHO_LINHA];
    int tabulacao = 20;
    int pratoCount = 0;
    int pratoQuant = 0;
    int descerLocal = *descer;

    pratos = primeiroPratos(&pratos);
    pratoQuant = contaPratos(pratos);

    if (pratos != NULL)
        snprintf(linha, sizeof(linha), "%s%*s%*s", "Número", tabulacao-1, "Prato   ", tabulacao-2, "Preço");
    else
        snprintf(linha, sizeof(linha), "Não existe nenhum prato cadastrado.\n \n"
                                       "Note que você pode adicionar um prato através do menu abaixo.");
    strcat(tela, linha);
    strcat(tela, "\n \n");

    if (descerLocal < 0)
    {
        strcpy (menuInfo, "ERRO: não há mais mesas para subir!");
        descerLocal = *descer = 0;
    }
    else if (erro)
        strcpy (menuInfo, "ERRO: a opção digitada não existe!");
    else
        strcpy (menuInfo, "Opções Mesa");

    while (pratos != NULL && pratoCount < 9)
    {
        if (descerLocal <= 0)
        {
            char id[4] = "";
            char valor[10] = "";
            int tamanhoExtra = contaCaracterDuplo(pratos->nome);
            snprintf(id, sizeof (id), "%i", pratos->id);
            snprintf(valor, sizeof (valor), "R$ %.2f", pratos->preco);

            snprintf(linha, sizeof(linha), "%s%*s", id, tabulacao+tamanhoExtra, pratos->nome);
            strcat(tela, linha);
            snprintf(linha, sizeof(linha), "%*s", tabulacao, valor);
            strcat(tela, linha);
            strcat(tela, "\n");
            pratos = pratos->proximo;
            pratoCount++;
        }
        else
        {
            pratos = pratos->proximo;

            if (pratos != NULL && pratoQuant >= 9 + descerLocal)
                descerLocal -= 1;
            else
            {
                strcpy (menuInfo, "ERRO: não há mais mesas para descer!");
                pratos = pratos->anterior;
                *descer = descerLocal -= 1;
            }
        }
    }


    imprimirTela("Mesas", tela, menuInfo, "(1) Adicionar (2) Excluir\n"
                                          "(7) Subir (8) Descer (9) Voltar", "|  Digite o número da opção desejada: ");
}

void telaAdicionarMesa(int erro)
{
    char menuInfo[TAMANHO_LINHA];

    if (erro)
        strcpy (menuInfo, "ERRO: o número digitado não pertece a nenhum garçon!");
    else
        strcpy (menuInfo, "Opções Adicionar Mesa");

    imprimirTela("Adicionar Mesa", "Você está adicionando uma Mesa.\n \nInsira o número do garçon responsável pela mesa.\n \n"
                                     "Se você não souber, consulte em \"Consultar Garçons\".\n", menuInfo,
                 "(1-999) Número do garçom\n(9) Cancelar e Voltar", "|  Digite o número da opção ou do garçom: ");
}

void telaRemoverMesa(struct mesasNo *mesa, int erro)
{
    char menuInfo[TAMANHO_LINHA];

    if (erro)
        strcpy (menuInfo, "ERRO: o número digitado não pertece a nenhuma mesa!");
    else
        strcpy (menuInfo, "Opções Fechamento de Conta da Mesa");

    if (mesa == NULL)
    imprimirTela("Fechamento de Conta da Mesa", "Você está fechando a conta de uma Mesa.\n \nInsira o número da mesa.\n \n"
                                     "Se você não souber, consulte em \"Consultar mesa\".\n", menuInfo,
                 "(1-999) Número da Mesa\n(9) Cancelar e Voltar", "|  Digite o número da opção ou da mesa: ");
    else
    {
        char aux[TAMANHO_LINHA*2];
        snprintf(aux, sizeof(aux), "Conta da mesa %i fechada com sucesso!\n \ngarçom: %s\n \n"
                                   "Valor total gasto: R$ %.2f\n",
                  mesa->id, mesa->garcon->nome, mesa->valorTotal);
        imprimirTela("Fechamento de Conta da Mesa", aux, "Opções para Conta Fechada",
                     "(1) Ver todos os pratos pedidos detalhadamente\n(9) Voltar", "|   Digite o número da opção: ");
    }

}

void telaAdicionarPedido(int mesaID, struct pratosNo *prato, int quantidade, int erro)
{
    char menuInfo[TAMANHO_LINHA] = "Opções Adicionar Pedido";

    if (mesaID == 0)
        imprimirTela("Adicionar Pedido", "Você está adicionando um pedido.\n \n \nMesa N°: ...\n \nPrato: ... \n \n"
                                         "Quantidade: ...\n \nDigite o número da mesa a qual o pedido será adicionado.\n"
                                         "Se você não souber, consulte em \"Consultar Mesas\".\n", menuInfo,
                 "(1-999) Número da Mesa\n(9) Cancelar e Voltar", "|  Digite o número da opção ou da mesa: ");
    else if(prato == NULL)
    {
        char aux[TAMANHO_LINHA*2];

        if (erro)
            strcpy (menuInfo, "ERRO: o número informado não corresponde a nenhum prato!");

        snprintf(aux, sizeof(aux), "Você está adicionando um pedido.\n \n \nMesa N°: %i\n \nPrato: ... \n \n"
                 "Quantidade: ...\n \nDigite o número do prato a ser adicionado.\n"
                 "Se você não souber, consulte em \"Consultar Pratos\".\n", mesaID);
        imprimirTela("Adicionar Pedido", aux, menuInfo,
                 "(1-999) Número do Prato\n(9) Cancelar e Voltar", "|  Digite o número da opção ou do prato: ");
    }
    else if (quantidade == 0)
    {
        char aux[TAMANHO_LINHA*2];

        if (erro)
            strcpy (menuInfo, "ERRO: a quantidade do pedido não pode ser menor que 1!");
        else
            strcpy (menuInfo, "Opções Adicionar Pedido");

        snprintf(aux, sizeof(aux), "Você está adicionando um pedido.\n \n \nMesa N°: %i\n \nPrato: %s \n \n"
                 "Quantidade: ...\n \n \n \nDigite a quantidade do prato a ser adicionado.\n",
                  mesaID, prato->nome);
        imprimirTela("Adicionar Pedido", aux, menuInfo,
                 "(1-999) Número da Mesa\n(9) Cancelar e Voltar", "|  Digite o número da opção ou da quantidade: ");
    }
    else
    {
        char aux[TAMANHO_LINHA*2];
        snprintf(aux, sizeof(aux), "Você adicionou um pedido com sucesso!\n \n \nMesa N°: %i\n"
                                   " \nPrato: %s \n \nQuantidade: %i\n", mesaID, prato->nome, quantidade);
        imprimirTela("Adicionar Pedido", aux, "Pressione Enter para Voltar",
                 "(Enter) Voltar", "|   Aguardando Enter para voltar...");
    }
}

void telaConsultaPedidos (struct pedidosNo *pedidos, int mesaID, int morto, int *descer, int erro)
{
    char tela[TAMANHO_TELA] = "";
    char linha[TAMANHO_LINHA*2] = "";
    char menuInfo[TAMANHO_LINHA];
    char titulo[TAMANHO_LINHA];
    int tabulacao = 20;
    int pedidosCount = 0;
    int pedidosQuant = 0;
    int descerLocal = *descer;
    pedidosQuant = contaPedidos(pedidos);

    snprintf(titulo, sizeof(titulo), "Pedidos da Mesa %i", mesaID);


    if (pedidos != NULL || morto == 1)
        snprintf(linha, sizeof(linha), "%s%*s%*s%*s", "Número", tabulacao-1, "Prato   ", tabulacao-2, "Quantidade", tabulacao, "Preço");
    else
        snprintf(linha, sizeof(linha), "Não existe nenhum pedido cadastrado.\n \n"
                                       "Note que você pode adicionar um através do \"Consultar Mesas\".");
    strcat(tela, linha);
    strcat(tela, "\n \n");

    if (descerLocal < 0)
    {
        strcpy (menuInfo, "ERRO: não há mais pedido para subir!");
        descerLocal = *descer = 0;
    }
    else if (erro)
        strcpy (menuInfo, "ERRO: a opção digitada não existe!");
    else
        strcpy (menuInfo, "Opções pedido");

    while (pedidos != NULL && pedidosCount < 9)
    {
        if (descerLocal <= 0)
        {
            char id[4] = "";
            char valor[10] = "";
            char quantidade[4] = "";
            int tamanhoExtra = contaCaracterDuplo(pedidos->prato->nome);
            snprintf(id, sizeof (id), "%i", pedidos->id);
            snprintf(quantidade, sizeof (quantidade), "%i", pedidos->quantidade);
            snprintf(valor, sizeof (valor), "R$ %.2f", pedidos->prato->preco);

            snprintf(linha, sizeof(linha), "%s%*s", id, tabulacao+tamanhoExtra, pedidos->prato->nome);
            strcat(tela, linha);
            snprintf(linha, sizeof(linha), "%*s", tabulacao, valor);
            strcat(tela, linha);
            snprintf(linha, sizeof(linha), "%*s", tabulacao-4, quantidade);
            strcat(tela, linha);
            strcat(tela, "\n");
            pedidos = pedidos->proximo;
            pedidosCount++;
        }
        else
        {
            struct pedidosNo *aux = pedidos;
            pedidos = pedidos->proximo;

            if (pedidos != NULL && pedidosQuant >= 9 + descerLocal)
                descerLocal -= 1;
            else
            {
                strcpy (menuInfo, "ERRO: não há mais pedidos para descer!");
                pedidos = aux;
                *descer = descerLocal -= 1;
            }
        }
    }

    if (!morto)
        imprimirTela(titulo, tela, menuInfo, "(1) Excluir\n"
                                             "(7) Subir (8) Descer (9) Voltar", "|  Digite o número da opção desejada: ");
    else
        imprimirTela(titulo, tela, menuInfo, "Não se pode modificar pedidos de contas fechadas.\n(7) Subir (8) Descer (9) Voltar", "|  Digite o número da opção desejada: ");
}

void telaPerguntaMesa (int erro)
{
    char menuInfo[TAMANHO_LINHA];

    if (erro)
        strcpy (menuInfo, "ERRO: o número digitado não pertece a nenhuma mesa!");
    else
        strcpy (menuInfo, "Opções Consulta Pedidos");

    imprimirTela("Consulta Pedidos", "Você está consultando pedidos de uma mesa.\n \nInsira o número da mesa.\n \n"
                                     "Se você não souber, volte para a tela anterior.\n", menuInfo,
                 "(1-999) Número da Mesa\n(9) Cancelar e Voltar", "|  Digite o número da opção ou da mesa: ");
}

void telaRemoverPedidos (int erro)
{
    char menuInfo[TAMANHO_LINHA];

    if (erro)
        strcpy (menuInfo, "ERRO: o número digitado não pertece a nenhum pedido!");
    else
        strcpy (menuInfo, "Opções Remover Pedido");

    imprimirTela("Remover Pedido", "Você está removendo um pedido.\n \nInsira o número do pedido a ser removido.\n \n"
                                     "Se você não souber, volte para a tela anterior.\n", menuInfo,
                 "(1-999) Número do Pedido\n(9) Cancelar e Voltar", "|  Digite o número da opção ou do pedido: ");
}

void telaAdicionarGarcons (int erro)
{
    if (!erro)
    imprimirTela("Adicionar Garçom", "Você está adicionando um garçom.\n \nInsira o nome do garçom.\n \n", "Opções Adicionar Garçom",
                 "(Texto) Nome do Garçom\n(9) Cancelar e Voltar", "|  Digite o número da opção ou nome do garçom: ");
    else
        imprimirTela("Adicionar Garçom", "Você está adicionando um garçom.\n \nInsira o nome do garçom.\n \n", "ERRO: garçom não pode ser um número!",
                     "(Texto) Nome do Garçom\n(9) Cancelar e Voltar", "|  Digite o número da opção ou nome do garçom: ");
}

void telaRemoverGarcons (int erro)
{
    if (!erro)
    imprimirTela("Remover Garçom", "Você está removendo um garçom.\n \nInsira o número do garçom a ser removido.\n \n"
                                   "Se você não souber, volte para a tela anterior.\n", "Opções Adicionar Garçom",
                 "(1-999) Número do garçom\n(9) Cancelar e Voltar", "|  Digite o número da opção ou número do garçom: ");
    else
        imprimirTela("Remover Garçom", "Você está removendo um garçom.\n \nInsira o número do garçoma ser removido.\n \n"
                                       "Se você não souber, volte para a tela anterior.", "ERRO: não existe nenhum garçom com esse número!",
                     "(1-999) Número do garçom\n(9) Cancelar e Voltar", "|  Digite o número da opção ou número do garçom: ");
}

void telaAdicionarPrato (char nome[], float preco, int erro)
{
    char menuInfo[TAMANHO_LINHA] = "Opções Adicionar Prato";

    if(erro)
        strcpy(menuInfo, "ERRO: o nome do prato não pode começar com um número!");

    if (strcmp(nome, "") == 0)
        imprimirTela("Adicionar Prato", "Você está adicionando um prato.\n \n \nNome: ...\n \nPreço: R$... \n \n"
                                         "Digite o nome do prato.\n", menuInfo,
                 "(Texto) Nome do Prato\n(9) Cancelar e Voltar", "|  Digite o número da opção ou nome do prato: ");
    else if(preco == 0)
    {
        char aux[TAMANHO_LINHA*2];

        if (erro)
            strcpy (menuInfo, "ERRO: não existe almoço grátis! Insira um número maior que 0.");

        snprintf(aux, sizeof(aux), "Você está adicionando um prato.\n \n \nNome: %s\n \nPreço: R$... \n \n"
                                                      "Digite o preço do prato. Ex: 9.00\n", nome);
        imprimirTela("Adicionar Prato", aux, menuInfo,
                 "(Número Inteiro ou Decimal) Preço do Prato\n \n", "|  Digite o número da opção ou o preço do prato: R$ ");
    }
    else
    {
        char aux[TAMANHO_LINHA*2];
        snprintf(aux, sizeof(aux), "Você adicionou um prato com sucesso!\n \nPrato: %s\n"
                                   " \nPreço: R$ %.2f \n \n", nome, preco);
        imprimirTela("Adicionar Prato", aux, "Pressione Enter para Voltar",
                 "(Enter) Voltar", "|   Aguardando Enter para voltar...");
    }
}

void telaRemoverPrato (int erro)
{
    char menuInfo[TAMANHO_LINHA];

    if (erro)
        strcpy (menuInfo, "ERRO: o número digitado não pertece a nenhum prato!");
    else
        strcpy (menuInfo, "Opções Remover Prato");

    imprimirTela("Remover Prato", "Você está removendo um prato.\n \nInsira o número do prato a ser removido.\n \n"
                                     "Se você não souber, volte para a tela anterior.\n", menuInfo,
                 "(1-999) Número do Prato\n(9) Cancelar e Voltar", "|  Digite o número da opção ou do prato: ");
}
