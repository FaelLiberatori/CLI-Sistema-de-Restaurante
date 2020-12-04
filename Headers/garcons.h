#ifndef GARCONS_H
#define GARCONS_H

struct garconsNo
{
    int id;
    char nome[30];
    struct garconsNo *proximo, *anterior;
};

int carregaGarcons (struct garconsNo **garcons, char arquivoDiretorio[]);
int insereGarcom (struct garconsNo **garcons, char nome[30], char arquivoDiretorio[]);
struct garconsNo *consultaGarcom (struct garconsNo **garcons, int id);
struct garconsNo *primeiroGarcom (struct garconsNo **garcons);
int delecaoGarcon (struct garconsNo **garcon, int id, char arquivoDiretorio[]);
int contaGarcons (struct garconsNo *garcons);

#endif // GARCONS_H
