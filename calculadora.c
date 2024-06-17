#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_EXPR_SIZE 100
#define MAX_TOKEN_SIZE 20
#define MAX_Pilha_SIZE 100

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double data[MAX_Pilha_SIZE];
    int topo;
} Pilha;

typedef struct {
    char *data[MAX_Pilha_SIZE];
    int topo;
} ExprPilha;

void inicializarPilha(Pilha *p);
void inicializarExprPilha(ExprPilha *p);
int estaVazia(Pilha *p);
int exprEstaVazia(ExprPilha *p);
int estaCheia(Pilha *p);
int exprEstaCheia(ExprPilha *p);
void empilha(Pilha *p, double valor);
void empilhaExpr(ExprPilha *p, char *valor);
double desempilha(Pilha *p);
char *desempilhaExpr(ExprPilha *p);
double topo(Pilha *p);
char *topoExpr(ExprPilha *p);
double aplicarOperador(char op, double a, double b);
double grausParaRadianos(double graus);
double aplicarFuncao(const char *func, double a);
double avaliarPosFixa(const char *expressao, char *infixa);

int main() {
    char expressao[MAX_EXPR_SIZE];
    char infixa[MAX_EXPR_SIZE] = "";

    printf("Digite a expressao em notacao pos-fixada: ");
    fgets(expressao, MAX_EXPR_SIZE, stdin);
    expressao[strcspn(expressao, "\n")] = '\0'; 

    double resultado = avaliarPosFixa(expressao, infixa);
    printf("Resultado: %.2f\n", resultado);
    printf("Expressao Infixa: %s\n", infixa);

    return 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

void inicializarExprPilha(ExprPilha *p) {
    p->topo = -1;
}

int estaVazia(Pilha *p) {
    return p->topo == -1;
}

int exprEstaVazia(ExprPilha *p) {
    return p->topo == -1;
}

int estaCheia(Pilha *p) {
    return p->topo == MAX_Pilha_SIZE - 1;
}

int exprEstaCheia(ExprPilha *p) {
    return p->topo == MAX_Pilha_SIZE - 1;
}

void empilha(Pilha *p, double valor) {
    if (!estaCheia(p)) {
        p->data[++p->topo] = valor;
    }
}

void empilhaExpr(ExprPilha *p, char *valor) {
    if (!exprEstaCheia(p)) {
        p->data[++p->topo] = valor;
    }
}

double desempilha(Pilha *p) {
    if (!estaVazia(p)) {
        return p->data[p->topo--];
    }
    return 0;
}

char *desempilhaExpr(ExprPilha *p) {
    if (!exprEstaVazia(p)) {
        return p->data[p->topo--];
    }
    return NULL;
}

double topo(Pilha *p) {
    if (!estaVazia(p)) {
        return p->data[p->topo];
    }
    return 0;
}

char *topoExpr(ExprPilha *p) {
    if (!exprEstaVazia(p)) {
        return p->data[p->topo];
    }
    return NULL;
}

double aplicarOperador(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

double grausParaRadianos(double graus) {
    return graus * M_PI / 180.0;
}

double aplicarFuncao(const char *func, double a) {
    if (strcmp(func, "raiz") == 0) return sqrt(a);
    if (strcmp(func, "sen") == 0) return sin(grausParaRadianos(a));
    if (strcmp(func, "cos") == 0) return cos(grausParaRadianos(a));
    if (strcmp(func, "tg") == 0) return tan(grausParaRadianos(a));
    if (strcmp(func, "log") == 0) return log10(a);
    return 0;
}

double avaliarPosFixa(const char *expressao, char *infixa) {
    Pilha p;
    ExprPilha ep;
    inicializarPilha(&p);
    inicializarExprPilha(&ep);

    char token[MAX_TOKEN_SIZE];
    int i = 0, j = 0;

    while (expressao[i] != '\0') {
        if (expressao[i] == ' ') {
            token[j] = '\0';
            if (j > 0) {
                if (strchr("+-*/^", token[0]) && j == 1) {
                    double b = desempilha(&p);
                    double a = desempilha(&p);
                    double resultado = aplicarOperador(token[0], a, b);
                    empilha(&p, resultado);

                    char *direita = desempilhaExpr(&ep);
                    char *esquerda = desempilhaExpr(&ep);
                    char *novaExpr = malloc(MAX_EXPR_SIZE);
                    snprintf(novaExpr, MAX_EXPR_SIZE, "(%s %c %s)", esquerda, token[0], direita);
                    empilhaExpr(&ep, novaExpr);
                    free(esquerda);
                    free(direita);

                } else if (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
                           strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
                           strcmp(token, "log") == 0) {
                    double a = desempilha(&p);
                    double resultado = aplicarFuncao(token, a);
                    empilha(&p, resultado);

                    char *operando = desempilhaExpr(&ep);
                    char *novaExpr = malloc(MAX_EXPR_SIZE);
                    snprintf(novaExpr, MAX_EXPR_SIZE, "%s(%s)", token, operando);
                    empilhaExpr(&ep, novaExpr);
                    free(operando);

                } else {
                    empilha(&p, atof(token));
                    char *operando = malloc(MAX_TOKEN_SIZE);
                    snprintf(operando, MAX_TOKEN_SIZE, "%s", token);
                    empilhaExpr(&ep, operando);
                }
                j = 0;
            }
        } else {
            token[j++] = expressao[i];
        }
        i++;
    }

    token[j] = '\0';
    if (j > 0) {
        if (strchr("+-*/^", token[0]) && j == 1) {
            double b = desempilha(&p);
            double a = desempilha(&p);
            double resultado = aplicarOperador(token[0], a, b);
            empilha(&p, resultado);

            char *direita = desempilhaExpr(&ep);
            char *esquerda = desempilhaExpr(&ep);
            char *novaExpr = malloc(MAX_EXPR_SIZE);
            snprintf(novaExpr, MAX_EXPR_SIZE, "(%s %c %s)", esquerda, token[0], direita);
            empilhaExpr(&ep, novaExpr);
            free(esquerda);
            free(direita);

        } else if (strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
                   strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 ||
                   strcmp(token, "log") == 0) {
            double a = desempilha(&p);
            double resultado = aplicarFuncao(token, a);
            empilha(&p, resultado);

            char *operando = desempilhaExpr(&ep);
            char *novaExpr = malloc(MAX_EXPR_SIZE);
            snprintf(novaExpr, MAX_EXPR_SIZE, "%s(%s)", token, operando);
            empilhaExpr(&ep, novaExpr);
            free(operando);

        } else {
            empilha(&p, atof(token));
            char *operando = malloc(MAX_TOKEN_SIZE);
            snprintf(operando, MAX_TOKEN_SIZE, "%s", token);
            empilhaExpr(&ep, operando);
        }
    }

    strcpy(infixa, desempilhaExpr(&ep));
    return desempilha(&p);
}
