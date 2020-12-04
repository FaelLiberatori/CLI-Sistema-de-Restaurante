#ifndef PEDIDOS_H
#define PEDIDOS_H
#include "pratos.h"

struct pedidosNo
{
    int id;
    int quantidade;
    struct pratosNo * prato;
    struct pedidosNo *proximo;
};

void inserePedido (struct pedidosNo **pedidos, struct pratosNo *prato, int quantidade);
struct pedidosNo *consultaPedido (struct pedidosNo **pedidos, int id);
int delecaoPedido (struct pedidosNo **pedidos, int id);
void liberaPedidos (struct pedidosNo **pedidos);
int contaPedidos (struct pedidosNo *pedidos);

#endif // PEDIDOS_H
