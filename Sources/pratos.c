#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pratos.h"

static int gravaPrato (struct pratosNo *prato, char arquivoDiretorio[])
{
    // Função auxiliar para uso interno.
    // Grava o prato no arquivo.

    FILE *arquivo = fopen(arquivoDiretorio, "ab");
    if (arquivo != NULL)
    {
        fwrite(prato, sizeof (struct pratosNo), 1, arquivo);
        fclose(arquivo);
        return 1;
    }
    else
        return 0;
}

static int regravaPratos (struct pratosNo *pratos, char arquivoDiretorio[])
{
    // Função auxiliar para uso interno.
    // Apaga o arquivo passado, cria um novo e grava todos os pratos nele.

    if(pratos != NULL)
    {
        remove (arquivoDiretorio);

        while (pratos->anterior != NULL)
            pratos = pratos->anterior;

        while (pratos != NULL)
        {
            if (!gravaPrato(pratos, arquivoDiretorio))
                return 0;

            pratos = pratos->proximo;
        }

        return 1;
    }
    else
        return 0;
}

int carregaPratos (struct pratosNo **pratos, char arquivoDiretorio[])
{
    // Assume pratos uma lista duplamente encadeada aterrada e vazia.
    // Retorna a quantidade de elementos carregados do arquivo.
    // Se o arquivo for vazio, incompleto ou o pratos não cumprir
    // os requesitos, a função retornará 0, já que 0 elementos foram carregados.

    int quant = 0;
    FILE *arquivo = fopen(arquivoDiretorio, "rb");

    if (arquivo != NULL && (*pratos) == NULL)
    {
        fseek (arquivo, 0, SEEK_END);

        if (ftell (arquivo) >= (signed) sizeof (struct pratosNo))
        {
            rewind(arquivo);
            (*pratos) = (struct pratosNo*) malloc(sizeof(struct pratosNo));
            struct pratosNo *aux = NULL;

            while (fread((*pratos), sizeof(struct pratosNo), 1, arquivo) == 1)
            {
                quant++;
                (*pratos)->proximo = (struct pratosNo*) malloc(sizeof(struct pratosNo));
                (*pratos)->anterior = aux;
                aux = (*pratos);
                (*pratos) = (*pratos)->proximo;
                (*pratos)->anterior = aux;
            }

            fclose (arquivo);
            free ((*pratos));
            (*pratos) = aux;
            (*pratos)->proximo = NULL;

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

int inserePrato (struct pratosNo **pratos, char nome[30], float preco, char arquivoDiretorio[])
{
    // Assume pratos uma lista duplamente encadeada aterrada ou não vazia.
    // Insere um novo prato no final da lista.

    struct pratosNo *aux = NULL;

    while ((*pratos) != NULL)
    {
        aux = (*pratos);
        (*pratos) = (*pratos)->proximo;
    }

    (*pratos) = (struct pratosNo*) malloc(sizeof (struct pratosNo));
    (*pratos)->proximo = NULL;
    (*pratos)->anterior = aux;

    if ((*pratos)->anterior == NULL)
        (*pratos)->id = 1;
    else if ((*pratos)->anterior->id == 8)
    {
        (*pratos)->id = 10;
        aux->proximo = (*pratos);
    }
    else
    {
        (*pratos)->id = (*pratos)->anterior->id + 1;
        aux->proximo = (*pratos);
    }

    (*pratos)->preco = preco;
    strcpy((*pratos)->nome, nome);

    return gravaPrato ((*pratos), arquivoDiretorio);
}

static struct pratosNo *internoConsultaPrato (struct pratosNo **pratos, int id, int lado)
{
    // Função auxiliar para uso interno

    if ((*pratos) == NULL)
        return NULL;
    else if ((*pratos)->id == id)
        return (*pratos);
    else
    {
        if(lado >= 1)
            return internoConsultaPrato(&(*pratos)->proximo, id, 1);
        else
            return internoConsultaPrato(&(*pratos)->anterior, id, -1);
    }
}

struct pratosNo *consultaPrato (struct pratosNo **pratos, int id)
{
    // Assume pratos uma lista duplamente encadeada aterrada ou não vazia.
    // Retorna o ponteiro do prato em questão ou NULL caso não exita.

    struct pratosNo *direita = internoConsultaPrato(&(*pratos), id, 1);

    if (direita != NULL)
        return direita;
    else
    {
        struct pratosNo *esquerda = internoConsultaPrato(&(*pratos), id, -1);
        return esquerda; // Se não existir, vai ser NULL
    }
}

struct pratosNo *primeiroPratos (struct pratosNo **pratos)
{
    if ((*pratos) == NULL | (*pratos)->anterior == NULL)
        return (*pratos);
    else
        return primeiroPratos(&(*pratos)->anterior);
}

int delecaoPrato (struct pratosNo **pratos, int id, char arquivoDiretorio[])
{
    // Assume pratos uma lista duplamente encadeada aterrada ou não vazia.
    // Retorna true se um prato com mesmo id passado existir, senão false.
    // Se existir, o prato é deletado.

    struct pratosNo *excluir = consultaPrato(&(*pratos), id);

    if (excluir != NULL)
    {
        struct pratosNo *aux = NULL;

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
        (*pratos) = aux;

        if (!regravaPratos((*pratos), arquivoDiretorio))
            return 0;

        return 1;
    }
    else
        return 0;
}

int internoContaPratos (struct pratosNo *pratos)
{
    // Para uso interno.

    if (pratos == NULL)
        return 0;
    else
        return internoContaPratos(pratos->proximo) + 1;
}

int contaPratos (struct pratosNo *pratos)
{
    // Retorna a quantidade de pratos na lista.

    pratos = primeiroPratos(&pratos);

    return internoContaPratos(pratos);
}


