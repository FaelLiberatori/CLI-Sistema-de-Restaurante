#ifndef TELA_H
#define TELA_H
#include "mesas.h"
#include "garcons.h"
#include "pratos.h"

void telaInicio(int erro);
void telaConsultaMesa (struct mesasNo *mesas, int *telaNum, int erro);
void telaAdicionarMesa(int erro);
void telaConsultaPratos (struct pratosNo *pratos, int *descer, int erro);
void telaRemoverMesa(struct mesasNo *mesa, int erro);
void telaAdicionarPedido(int mesaID, struct pratosNo *prato, int quantidade, int erro);
void telaConsultaGarcons (struct garconsNo *garcon, int *telaNum, int erro);
void telaConsultaPedidos (struct pedidosNo *pedidos, int mesaID, int morto, int *descer, int erro);
void telaPerguntaMesa (int erro);
void telaRemoverPedidos (int erro);
void telaAdicionarGarcons (int erro);
void telaRemoverGarcons (int erro);
void telaAdicionarPrato (char nome[], float preco, int erro);
void telaRemoverPrato (int erro);

#endif // TELA_H
