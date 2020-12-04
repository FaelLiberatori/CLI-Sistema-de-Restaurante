#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pratos.h"
#include "garcons.h"
#include "pedidos.h"
#include "mesas.h"
#include "tela.h"

float tratarInputNum (char input[4]);
char *tratarInputString (char input[20]);
void listaMesa(struct mesasNo **mesas, struct garconsNo **garcons, struct pratosNo **pratos);
void listaGarcom(struct garconsNo **garcons, char arquivoGarcons[]);
void listaPratos(struct pratosNo **pratos, char arquivoPratos[]);
void listaPedido(struct mesasNo **mesas);
void addMesa(struct mesasNo **mesas, struct garconsNo **garcons);
void fecharMesa(struct mesasNo **mesas);
void addPedido(struct mesasNo **mesas, struct pratosNo **pratos);
void addGarcom(struct garconsNo **garcons, char diretorioGarcons[]);
void excluirGarcom (struct garconsNo **garcons, char diretorioGarcons[]);
void addPrato(struct pratosNo **pratos, char arquivoPratos[]);
void excluirPrato(struct pratosNo **pratos, char arquivoPratos[]);

int main()
{
    struct mesasNo *mesas = NULL;
    struct garconsNo *garcons = NULL;
    struct pratosNo *pratos = NULL;
    char diretorioGarcons[] = "./garcons.bin";
    char diretorioPratos[] = "./pratos.bin";

    // Se não existir arquivo ou ele for vazio, carrega alguns garções para facilitar
    if (carregaGarcons(&garcons, diretorioGarcons) == 0)
    {
        insereGarcom(&garcons, "Zeca Pagodinho", diretorioGarcons);
        insereGarcom(&garcons, "Ronald McDonald", diretorioGarcons);
    }

    // Se não existir arquivo ou ele for vazio, carrega alguns pratos para facilitar
    if (carregaPratos(&pratos, diretorioPratos) == 0)
    {
        inserePrato(&pratos, "Arroz com feijão", 14.50, diretorioPratos);
        inserePrato(&pratos, "Refrigerante", 5.90, diretorioPratos);
    }

    char oi[4]; // Permite selecionar até 999 opções de cada iten.
    int input = 0;
    int erro = 0;
    int rodandoMain= 1;

    while (rodandoMain)
    {
        telaInicio(erro);
        erro = 0;

        switch (input = tratarInputNum(fgets(oi, sizeof(char)*4, stdin)))
        {
        case 1: // Consultar Mesa
            listaMesa(&mesas, &garcons, &pratos);
            break;

        case 2: // Consultar Garçons
            listaGarcom(&garcons, diretorioGarcons);
            break;
        case 3: // Consultar Prato
            listaPratos(&pratos, diretorioPratos);
            break;
        case 9: // Sair
            printf("\n\n\n      Obrigado por usar o Sistema Restaurante!\n\n      Saindo...\n\n\n\n");
            rodandoMain = 0;
            break;
        default:
            erro = 1;
            break;
        }

        //printf("%i", i);
    }

    return 0;
}

float tratarInputNum (char input[4])
{
    while (input == NULL) // Se overflow, limpa o stdin
    {
        scanf("%*[^\n]");
        scanf("%*c");
    }

    strtok(input, "\n");

    return atof(input);
}

char *tratarInputString (char input[20])
{
    char aux[20];

    while (input == NULL) // Se overflow, limpa o stdin
    {
        scanf("%*[^\n]");
        scanf("%*c");
    }

    snprintf(aux, sizeof (aux), "%s", input);
    strtok(aux, "\n");
    strcpy(input, aux);

    return input;
}

void listaMesa(struct mesasNo **mesas, struct garconsNo **garcons, struct pratosNo **pratos)
{
    int rodando = 1;
    int descer = 0;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaConsultaMesa((*mesas), &descer, erro);
        erro = 0;

        switch (input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin)))
        {
        case 1: // Adicionar
            addMesa(&(*mesas), &(*garcons));
            break;
        case 2: // Excluir
            fecharMesa(&(*mesas));
            break;
        case 3: // Adicionar Pedido
            addPedido(&(*mesas), &(*pratos));
            break;
        case 4: // Consultar Pedido
            listaPedido(&(*mesas));
            break;
        case 7: // Subir
            descer--;
            break;
        case 8: // Descer
            descer++;
            break;
        case 9: // Voltar
            rodando = 0;
            break;
        default:
            erro = 1;
            break;
        }
    }
}

void listaGarcom(struct garconsNo **garcons, char arquivoGarcons[])
{
    int rodando = 1;
    int descer = 0;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while(rodando)
    {
        telaConsultaGarcons((*garcons), &descer, erro);
        erro = 0;
        switch(input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin)))
        {
        case 1: // Adicionar
            addGarcom(&(*garcons), arquivoGarcons);
            break;
        case 2: // Excluir
            excluirGarcom(&(*garcons), arquivoGarcons);
            break;
        case 7: // Subir
            descer--;
            break;
        case 8: // Descer
            descer++;
            break;
        case 9: // Voltar
            rodando = 0;
            break;
        default:
            erro = 1;
            break;
        }
    }
}

void listaPratos(struct pratosNo **pratos, char arquivoPratos[])
{
    int rodando = 1;
    int descer = 0;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaConsultaPratos((*pratos), &descer, erro);
        erro = 0;
        switch (input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin)))
        {
        case 1: // Adicionar
            addPrato(&(*pratos), arquivoPratos);
            break;
        case 2: // Excluir
            excluirPrato(&(*pratos), arquivoPratos);
            break;
        case 7: // Subir
            descer--;
            break;
        case 8: // Descer
            descer++;
            break;
        case 9: // Voltar
            rodando = 0;
            break;
        default:
            erro=1;
            break;
        }
    }
}

void listaPedido(struct mesasNo **mesas)
{
    int rodando = 1;
    int descer = 0;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaPerguntaMesa(erro);
        erro = 0;
        input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin));

        if (input == 9)
            rodando = 0;
        else if (!consultaMesa(&(*mesas), input))
            erro = 1;
        else
        {
            (*mesas) = consultaMesa(&(*mesas), input);
            int rodandConsPedido = 1;

            while (rodandConsPedido)
            {
                telaConsultaPedidos((*mesas)->pedidos, (*mesas)->id, 0, &descer, erro);
                erro = 0;
                switch (input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin)))
                {
                case 1: // Rmover
                    ;
                        telaRemoverPedidos(erro);
                        erro = 0;
                        input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin));

                        if (input == 9)
                            rodandConsPedido = 0;
                        else
                        {
                            if (delecaoPedido(&(*mesas)->pedidos, input))
                            {
                                atualizarValorTotal(&(*mesas));
                                rodandConsPedido = 0;
                            }
                            else
                                erro = 1;
                        }
                    break;
                case 7:
                    descer--;
                case 8:
                    descer++;
                case 9:
                    rodandConsPedido = 0;
                default:
                    erro = 1;
                }
            }
            rodando = 0;
        }
    }
}

void addMesa(struct mesasNo **mesas, struct garconsNo **garcons)
{
    int rodando = 1;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaAdicionarMesa(erro);
        erro = 0;
        input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin));
        if (input == 9)
            rodando = 0;
        else
        {
            struct garconsNo *aux = consultaGarcom(&(*garcons), input);

            if (aux == NULL)
                erro = 1;
            else
            {
                insereMesa(&(*mesas), aux);
                rodando = 0;
            }
        }
    }
}

void fecharMesa(struct mesasNo **mesas)
{
    int rodando = 1;
    int descer = 0;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaRemoverMesa(NULL, erro);
        erro = 0;
        input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin));
        if (input == 9)
            rodando = 0;
        else
        {
            if (consultaMesa(&(*mesas), input))
            {
                (*mesas) = consultaMesa(&(*mesas), input);
                int rodandoListaPedido = 1;
                while(rodandoListaPedido)
                {
                    telaRemoverMesa((*mesas), erro);
                    erro = 0;

                    switch(input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin)))
                    {
                    case 1:
                        ;
                        int rodandoMorto = 1;
                        while (rodandoMorto)
                        {
                            telaConsultaPedidos((*mesas)->pedidos, (*mesas)->id, 1, &descer, erro);
                            erro = 0;
                            switch (input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin)))
                            {
                            case 7:
                                descer--;
                            case 8:
                                descer++;
                            case 9:
                                rodandoMorto = 0;
                            default:
                                erro = 1;
                            }
                        }
                    case 9:
                        rodandoListaPedido = 0;
                        break;
                    default:
                        erro = 1;
                    }
                }
                delecaoMesa(&(*mesas), input);
                rodando = 0;
            }
            else
                erro = 1;
        }
    }
}

void addPedido(struct mesasNo **mesas, struct pratosNo **pratos)
{
    int rodando = 1;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaAdicionarPedido(0, NULL, 0, erro);
        erro = 0;
        input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin));
        if (input == 9)
            rodando = 0;
        else
        {
            struct mesasNo *mesaAux = consultaMesa(&(*mesas), input);

            if (mesaAux == NULL)
                erro = 1;
            else
            {
                int rodandoAddPedido = 1;
                int mesaID = input;

                while (rodandoAddPedido) // Prato
                {
                    telaAdicionarPedido(mesaID, NULL, 0, erro);
                    erro = 0;
                    input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin));

                    if (input == 9)
                        rodandoAddPedido = 0;
                    else
                    {
                        struct pratosNo *pratoAux = consultaPrato(&(*pratos), input);

                        if (pratoAux == NULL)
                            erro = 1;
                        else
                        {
                            int rodandoAddPedido3 = 1;
                            while (rodandoAddPedido3) // Quantidade
                            {
                                telaAdicionarPedido(mesaID, pratoAux, 0, erro);
                                erro = 0;
                                input = tratarInputNum(fgets(buffer, sizeof(char)*4, stdin));

                                if (input == 9)
                                    rodandoAddPedido3 = 0;
                                else if (input < 1)
                                    erro = 1;
                                else
                                {
                                    (*mesas) = consultaMesa(&(*mesas), mesaID);
                                    inserePedido(&(*mesas)->pedidos, pratoAux, input);
                                    atualizarValorTotal(&(*mesas));
                                    telaAdicionarPedido(mesaID, pratoAux, input, erro);
                                    rodandoAddPedido3 = 0;
                                    fgets(buffer, sizeof(char)*4, stdin);
                                }
                            }
                            rodandoAddPedido = 0;
                        }
                    }
                    rodando = 0;
                }
            }
        }
    }
}

void addGarcom (struct garconsNo **garcons, char diretorioGarcons[])
{
    int rodando = 1;
    int erro = 0;

    while (rodando)
    {
        telaAdicionarGarcons(erro);
        erro = 0;
        char inputString[20];

        if (tratarInputNum(fgets(inputString, sizeof(inputString), stdin)) != 0)
        {
            if (atoi(inputString) == 9)
                rodando = 0;
            else
                erro = 1;
        }
        else
        {
            strcpy(inputString, tratarInputString(inputString));
            insereGarcom(&(*garcons), inputString, diretorioGarcons);
            rodando = 0;
        }
    }
}

void excluirGarcom (struct garconsNo **garcons, char diretorioGarcons[])
{
    int rodando = 1;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaRemoverGarcons(erro);
        erro = 0;
        input = tratarInputNum(fgets(buffer, sizeof(input), stdin));

        if (input == 9)
            rodando = 0;
        else if (input == 0 || consultaGarcom(&(*garcons), input) == NULL)
                erro = 1;
        else
        {
            delecaoGarcon(&(*garcons), input, diretorioGarcons);
            rodando = 0;
        }
    }
}

void addPrato(struct pratosNo **pratos, char arquivoPratos[])
{
    int rodando = 1;
    int erro = 0;
    float inputNum = 0;
    char inputString[20];

    while (rodando)
    {
        telaAdicionarPrato("", 0, erro);
        erro = 0;
        inputNum = tratarInputNum(fgets(inputString, sizeof(inputString), stdin));
        strcpy(inputString, tratarInputString(inputString));

        if (inputNum == 9)
            rodando = 0;
        else if (inputNum == 0 && strcmp(inputString, "") != 0)
        {
            int rodandoAddPrato = 1;
            char nome[20];
            strcpy(nome, inputString);

            while (rodandoAddPrato)
            {
                telaAdicionarPrato(nome, 0, erro);
                erro = 0;
                inputNum = (float)tratarInputNum(fgets(inputString, sizeof(inputString), stdin));

                if (inputNum <= 0)
                    erro = 1;
                else
                {
                    telaAdicionarPrato(nome, inputNum, erro);
                    inserePrato(&(*pratos), nome, inputNum, arquivoPratos);
                    erro = 0;

                    fgets(inputString, sizeof(char)*4, stdin);
                    rodandoAddPrato = 0;
                }
            }
            rodando = 0;
        }
        else
            erro = 1;
    }
}

void excluirPrato(struct pratosNo **pratos, char arquivoPratos[])
{
    int rodando = 1;
    int erro = 0;
    int input = 0;
    char buffer[4];

    while (rodando)
    {
        telaRemoverPrato(erro);
        erro = 0;
        input = tratarInputNum(fgets(buffer, sizeof(input), stdin));

        if (input == 9)
            rodando = 0;
        else if (input == 0 || consultaPrato(&(*pratos), input) == NULL)
                erro = 1;
        else
        {
            delecaoPrato(&(*pratos), input, arquivoPratos);
            rodando = 0;
        }
    }
}
