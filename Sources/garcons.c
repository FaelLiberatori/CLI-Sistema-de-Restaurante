#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "garcons.h"

static int gravaGarcon (struct garconsNo *garcon, char arquivoDiretorio[])
{
    // Função auxiliar para uso interno.
    // Grava o garcon no arquivo.

    FILE *arquivo = fopen(arquivoDiretorio, "ab");
    if (arquivo != NULL)
    {
        fwrite(garcon, sizeof (struct garconsNo), 1, arquivo);
        fclose(arquivo);
        return 1;
    }
    else
        return 0;
}

static int regravaGarcons (struct garconsNo *garcons, char arquivoDiretorio[])
{
    // Função auxiliar para uso interno.
    // Apaga o arquivo passado, cria um novo e grava todos os garcons nele.

    if(garcons != NULL)
    {
        remove (arquivoDiretorio);

        while (garcons->anterior != NULL)
            garcons = garcons->anterior;

        while (garcons != NULL)
        {
            if (!gravaGarcon(garcons, arquivoDiretorio))
                return 0;

            garcons = garcons->proximo;
        }

        return 1;
    }
    else
        return 0;
}

int carregaGarcons (struct garconsNo **garcons, char arquivoDiretorio[])
{
    // Assume garcons uma lista duplamente encadeada aterrada e vazia.
    // Retorna a quantidade de elementos carregados do arquivo.
    // Se o arquivo for vazio, incompleto ou o garcons não cumprir
    // os requesitos, a função retornará 0, já que 0 elementos foram carregados.

    int quant = 0;
    FILE *arquivo = fopen(arquivoDiretorio, "rb");

    if (arquivo != NULL && (*garcons) == NULL)
    {
        fseek (arquivo, 0, SEEK_END);

        if (ftell (arquivo) >= (signed) sizeof (struct garconsNo))
        {
            rewind(arquivo);
            (*garcons) = (struct garconsNo*) malloc(sizeof(struct garconsNo));
            struct garconsNo *aux = NULL;

            while (fread((*garcons), sizeof(struct garconsNo), 1, arquivo) == 1)
            {
                quant++;
                (*garcons)->proximo = (struct garconsNo*) malloc(sizeof(struct garconsNo));
                (*garcons)->anterior = aux;
                aux = (*garcons);
                (*garcons) = (*garcons)->proximo;
                (*garcons)->anterior = aux;
            }

            fclose (arquivo);
            free ((*garcons));
            (*garcons) = aux;
            (*garcons)->proximo = NULL;

            return quant;
        }
        else
        {
            fclose (arquivo);
            return 0;
        }
    }
    else if (arquivo != NULL)
    {
        fclose(arquivo);
        return 0;
    }
    else
        return 0;
}

int insereGarcom (struct garconsNo **garcons, char nome[30], char arquivoDiretorio[])
{
    // Assume garcons uma lista duplamente encadeada aterrada ou não vazia.
    // Insere um novo garcon no final da lista.

    struct garconsNo *aux = NULL;

    while ((*garcons) != NULL)
    {
        aux = (*garcons);
        (*garcons) = (*garcons)->proximo;
    }

    (*garcons) = (struct garconsNo*) malloc(sizeof (struct garconsNo));
    (*garcons)->proximo = NULL;
    (*garcons)->anterior = aux;

    if ((*garcons)->anterior == NULL)
        (*garcons)->id = 1;
    else if ((*garcons)->anterior->id == 8)
    {
        (*garcons)->id = 10;
        aux->proximo = (*garcons);
    }
    else
    {
        (*garcons)->id = (*garcons)->anterior->id + 1;
        aux->proximo = (*garcons);
    }

    strcpy((*garcons)->nome, nome);

    return gravaGarcon ((*garcons), arquivoDiretorio);
}

static struct garconsNo *internoConsultaGarcom (struct garconsNo **garcons, int id, int lado)
{
    // Função auxiliar para uso interno

    if ((*garcons) == NULL)
        return NULL;
    else if ((*garcons)->id == id)
        return (*garcons);
    else
    {
        if(lado >= 1)
            return internoConsultaGarcom(&(*garcons)->proximo, id, 1);
        else
            return internoConsultaGarcom(&(*garcons)->anterior, id, -1);
    }
}

struct garconsNo *consultaGarcom (struct garconsNo **garcons, int id)
{
    // Assume garcons uma lista duplamente encadeada aterrada ou não vazia.
    // Retorna o ponteiro do garcon em questão ou NULL caso não exita.

    struct garconsNo *direita = internoConsultaGarcom(&(*garcons), id, 1);

    if (direita != NULL)
        return direita;
    else
    {
        struct garconsNo *esquerda = internoConsultaGarcom(&(*garcons), id, -1);
        return esquerda; // Se não existir, vai ser NULL
    }
}

struct garconsNo *primeiroGarcom (struct garconsNo **garcons)
{
    // Retorna o primeiro garçon da lista

    if ((*garcons) == NULL || (*garcons)->anterior == NULL)
        return (*garcons);
    else
        return primeiroGarcom(&(*garcons)->anterior);
}

int delecaoGarcon (struct garconsNo **garcons, int id, char arquivoDiretorio[])
{
    // Assume garcons uma lista duplamente encadeada aterrada ou não vazia.
    // Retorna true se um garcon com mesmo id passado existir, senão false.
    // Se existir, o garcon é deletado.

    struct garconsNo *excluir = consultaGarcom(&(*garcons), id);

    if (excluir != NULL)
    {
        struct garconsNo *aux = NULL;

        if (excluir->proximo != NULL && excluir->anterior != NULL)
        {
            excluir->anterior->proximo = excluir->proximo;
            aux = excluir->proximo->anterior = excluir->anterior;
        }
        else if (excluir->anterior != NULL)
        {
            excluir->anterior->proximo = NULL;
            aux = excluir->anterior;
        }
        else if (excluir->proximo != NULL)
        {
            excluir->proximo->anterior = NULL;
            aux = excluir->proximo;
        }

        free(excluir);
        (*garcons) = aux;

        if (!regravaGarcons((*garcons), arquivoDiretorio))
            return 0;

        return 1;
    }
    else
        return 0;
}

int InternoContaGarcons (struct garconsNo *garcons)
{
    // Para uso interno

    if (garcons == NULL)
        return 0;
    else
        return InternoContaGarcons(garcons->proximo) + 1;
}

int contaGarcons (struct garconsNo *garcons)
{
    // Conta a quantidade de garçons na lista.

    garcons = primeiroGarcom(&garcons);

    return InternoContaGarcons(garcons);
}


