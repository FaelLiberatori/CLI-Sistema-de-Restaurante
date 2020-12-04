#include <stdlib.h>
#include <string.h>
#include "mesas.h"
#include "pratos.h"
#include "pedidos.h"

void inserePedido (struct pedidosNo **pedidos, struct pratosNo *prato, int quantidade)
{
    // Assume pedidos uma lista simplesmente encadeada.
    // Insere um novo pedido no final da lista.

    static int cont = 0;
    cont++;
    if (cont == 9)
        cont++;

    struct pedidosNo *novoPedido = (struct pedidosNo*) malloc(sizeof (struct pedidosNo));
    novoPedido->id = cont;
    novoPedido->proximo = (*pedidos);
    novoPedido->prato = prato;
    novoPedido->quantidade = quantidade;
    (*pedidos ) = novoPedido;
}

struct pedidosNo *consultaPedido (struct pedidosNo **pedidos, int id)
{
    // Assume pedidos uma lista simplesmente encadeada.
    // Retorna o ponteiro do pedido em questão ou NULL caso não exita.

    if ((*pedidos) == NULL)
        return NULL;
    else if ((*pedidos)->id == id)
        return (*pedidos);
    else
        return consultaPedido(&(*pedidos)->proximo, id);
}

int delecaoPedido (struct pedidosNo **pedidos, int id)
{
    // Assume pedidos uma lista duplamente encadeada aterrada ou não vazia.
    // Retorna true se um pedido com mesmo id passado existir, senão false.
    // Se existir, o pedido é deletado.

    struct pedidosNo **excluir = &(*pedidos);
    struct pedidosNo *anterior = NULL;

    while ((*excluir) != NULL && (*excluir)->id != id)
    {
        anterior = (*excluir);
        (*excluir) = (*excluir)->proximo;
    }

    if ((*excluir)!= NULL)
    {
        if (anterior != NULL)
        {
            anterior->proximo = (*excluir)->proximo;
            free((*excluir));
        }
        else
            (*excluir) = NULL;

        return 1;
    }
    else
        return 0;
}

void liberaPedidos (struct pedidosNo **pedidos)
{
    // Remove todos os nós da memória

    struct pedidosNo *aux = (*pedidos);

    while ((*pedidos) != NULL)
    {
        aux = (*pedidos)->proximo;
        free((*pedidos));
        (*pedidos) = aux;
    }
}

int contaPedidos (struct pedidosNo *pedidos)
{
    if (pedidos == NULL)
        return 0;
    else
        return contaPedidos(pedidos->proximo) + 1;
}



