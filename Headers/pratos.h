#ifndef PRATOS_H
#define PRATOS_H

struct pratosNo
{
    int id;
    char nome[30];
    float preco;
    struct pratosNo *proximo, *anterior;
};

int carregaPratos (struct pratosNo **pratos, char arquivoDiretorio[]);
int inserePrato (struct pratosNo **pratos, char nome[30], float preco, char arquivoDiretorio[]);
struct pratosNo *consultaPrato (struct pratosNo **pratos, int id);
struct pratosNo *primeiroPratos (struct pratosNo **pratos);
int delecaoPrato (struct pratosNo **pratos, int id, char arquivoDiretorio[]);
int contaPratos (struct pratosNo *pratos);

#endif // PRATOS_H
