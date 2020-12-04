#ifndef MESAS_H
#define MESAS_H
#include "garcons.h"
#include "pedidos.h"

struct mesasNo
{
    int id;
    float valorTotal;
    struct garconsNo *garcon;
    struct pedidosNo *pedidos;
    struct mesasNo *proximo, *anterior;
};

void insereMesa (struct mesasNo **mesas, struct garconsNo *garcon);
struct mesasNo *consultaMesa (struct mesasNo **mesas, int id);
struct mesasNo *primeiraMesa (struct mesasNo **mesas);
int delecaoMesa (struct mesasNo **mesa, int id);
int contaMesas (struct mesasNo *mesas);
void atualizarValorTotal (struct mesasNo **mesa);

#endif // MESAS_H
