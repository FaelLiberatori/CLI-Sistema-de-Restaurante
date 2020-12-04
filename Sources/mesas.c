#include <stdlib.h>
#include <string.h>
#include "mesas.h"
#include "pedidos.h"

void insereMesa (struct mesasNo **mesas, struct garconsNo *garcon)
{
    // Assume mesas uma lista duplamente encadeada aterrada ou não vazia.
    // Assume que o garcon passado exista.

    struct mesasNo *aux = NULL;

    while ((*mesas) != NULL)
    {
        aux = (*mesas);
        (*mesas) = (*mesas)->proximo;
    }

    (*mesas) = (struct mesasNo*) malloc(sizeof (struct mesasNo));
    (*mesas)->garcon = garcon;
    (*mesas)->valorTotal = 0;
    (*mesas)->pedidos = NULL;
    (*mesas)->proximo = NULL;
    (*mesas)->anterior = aux;

    if ((*mesas)->anterior == NULL)
        (*mesas)->id = 1;
    else if ((*mesas)->anterior->id == 8)
    {
        (*mesas)->id = 10;
        aux->proximo = (*mesas);
    }
    else
    {
        (*mesas)->id = (*mesas)->anterior->id + 1;
        aux->proximo = (*mesas);
    }
}

static struct mesasNo *internoConsultaMesa (struct mesasNo **mesas, int id, int lado)
{
    // Função auxiliar para uso interno

    if ((*mesas) == NULL)
        return NULL;
    else if ((*mesas)->id == id)
        return (*mesas);
    else
    {
        if(lado >= 1)
            return internoConsultaMesa(&(*mesas)->proximo, id, 1);
        else
            return internoConsultaMesa(&(*mesas)->anterior, id, -1);
    }
}

struct mesasNo *consultaMesa (struct mesasNo **mesas, int id)
{
    // Assume mesas uma lista duplamente encadeada aterrada ou não vazia.
    // Retorna o ponteiro do mesa em questão ou NULL caso não exita.

    struct mesasNo *direita = internoConsultaMesa(&(*mesas), id, 1);

    if (direita != NULL)
        return direita;
    else
    {
        struct mesasNo *esquerda = internoConsultaMesa(&(*mesas), id, -1);
        return esquerda; // Se não existir, vai ser NULL
    }
}

struct mesasNo *primeiraMesa (struct mesasNo **mesas)
{
    // Retorna a primeira mesa da lista.

    if ((*mesas) == NULL || (*mesas)->anterior == NULL)
        return (*mesas);
    else
        return primeiraMesa(&(*mesas)->anterior);
}

int delecaoMesa (struct mesasNo **mesas, int id)
{
    // Assume mesas uma lista duplamente encadeada aterrada ou não vazia.
    // Retorna true se um mesa com mesmo id passado existir, senão false.
    // Se existir, o mesa é deletado.

    struct mesasNo *excluir = consultaMesa(&(*mesas), id);

    if (excluir != NULL)
    {
        liberaPedidos(&(*mesas)->pedidos);
        struct mesasNo *aux = NULL;

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
        (*mesas) = aux;

        return 1;
    }
    else
        return 0;
}

static int internoContaMesas (struct mesasNo *mesas)
{
    // Função auxiliar para uso interno

    if (mesas == NULL)
        return 0;
    else
        return internoContaMesas(mesas->proximo) + 1;
}

int contaMesas (struct mesasNo *mesas)
{
    // Retorna a quantidade de mesa na lista.

    mesas = primeiraMesa(&mesas);

    return internoContaMesas(mesas);
}

void atualizarValorTotal (struct mesasNo **mesa)
{
    // Atualiza o valor total dos pedidos da mesa.

    struct pedidosNo *aux = (*mesa)->pedidos;
    (*mesa)->valorTotal = 0;

    while (aux != NULL)
    {
        (*mesa)->valorTotal += aux->prato->preco * aux->quantidade;
        if(aux == aux->proximo)
            aux->proximo = NULL;
        aux = aux->proximo;
    }
}


