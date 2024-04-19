#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struct que ira conter os dados dos alunos */
typedef struct
{
    char nome[26];
    char telefone[15];
    char curso[20];
    float nota1;
    float nota2;
}Aluno;

/* Função que irá avaliar os alunos e escrever no arquivo de saída */
void avaliacao(FILE *entrada, FILE *saida);

int main(void)
{
    FILE *entrada = fopen("DadosEntrada.csv", "r");
    FILE *saida = fopen("SituacaoFinal.csv", "w");

    /* Checagem se conseguiu abrir o arquivo de leitura com exito. Não existe um para o arquivo de saida pois caso ele não exista, ele sera criado pelo parametro 'w' do fopen */
    if(entrada == NULL)
    {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }

    /* Cabeçalho do arquivo .csv de saída para melhor organização */
    char cabecalho[] = "Nome,Media,Situacao\n";
    fprintf(saida, "%s", cabecalho);

    /* Chamada da função para avaliação */
    avaliacao(entrada, saida);

    fclose(entrada);
    fclose(saida);
    return 0;
}

/* Função que irá avaliar os alunos e escrever no arquivo de saída */
void avaliacao(FILE *entrada, FILE *saida)
{
    /* Irá ler de linha por linha, um aluno de cada vez, fazendo os calculos e escrevendo-os no documento de saída. */
    char linha[100];
    float media;
    char situacao[10], ap[10] = "APROVADO", rp[10] = "REPROVADO";
    Aluno aluno;

    /* Loop que irá passar por cada linha/aluno */
    while(fgets(linha, sizeof(linha), entrada) != NULL)
    {
        /* Lê o arquivo e salva os dados lidos na struct */
        fscanf(entrada, "%25[^,],%14[^,],%19[^,],%f,%f", aluno.nome, aluno.telefone, aluno.curso, &aluno.nota1, &aluno.nota2);

        /* Calcula a media do aluno */
        media = (aluno.nota1 + aluno.nota2) / 2;
        if(media >= 7)
        {
            strcpy(situacao, ap);
        }
        else
        {
            strcpy(situacao, rp);
        }

        /* Escreve no arquivo de saída os dados do aluno */
        fprintf(saida, "%s,%.2f,%s\n", aluno.nome, media, situacao);
    }
}