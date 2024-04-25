#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float X;
    float Y;
}Ponto;

/* Função que divide o poligono indicado no arquivo texto em triangulos. */
void divisaoTriangulos(FILE *entrada);

/* Função que calcula a area de cada triangulo e retorna o valor deste. */
float areaTriangulo(Ponto A, Ponto B, Ponto C);

int main(void)
{
    /* Abre o arquivo contendo o numero de vertices e o valor de X e Y dos mesmos. Caso não exista, criar ele na pasta 'output'. */
    FILE *entrada = fopen("Poligono.txt", "r");

    /* Checagem se conseguiu abrir o arquivo com exito*/
    if(entrada == NULL)
    {
        printf("Erro ao abrir o arquivo. Verifique se o mesmo existe com o nome 'Poligono.txt'.");
        exit(1);
    }

    /* Chamada da função que irá dividir o poligono em triangulos */
    divisaoTriangulos(entrada);

    fclose(entrada);
    return 0;
}

/* Função que divide o poligono indicado no arquivo texto em triangulos. */
void divisaoTriangulos(FILE *entrada)
{
    char linha[10];
    Ponto pontos[100];
    int i = 0;
    float area, areaPoligono = 0;

    /* Lê o arquivo e armazena cada ponto dentro do array 'pontos'. */
    while(fgets(linha, sizeof(linha), entrada) != NULL)
    {
        fscanf(entrada, "%f %f", &pontos[i].X, &pontos[i].Y);
        i++;
    }

    /* Calcula cada triangulo do poligono e armazena o valor na variavel 'areaPoligono' */
    for(int j = 3; j <= i; j++)
    {
        /* Chamada da função que irá calcular a área de cada triangulo */
        area = areaTriangulo(pontos[0], pontos[j - 2], pontos[j - 1]);
        areaPoligono += area;
    }

    printf("A area do poligono e: %.2f", areaPoligono);
}

/* Função que calcula a area de cada triangulo e retorna o valor deste. */
float areaTriangulo(Ponto A, Ponto B, Ponto C)
{
    float principal, secundaria;

    principal = A.X * B.Y + A.Y * C.X + B.X * C.Y;
    secundaria = C.X * B.Y + C.Y * A.X + B.X * A.Y;

    return fabs(principal - secundaria) / 2;
}