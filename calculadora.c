#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_EXPR_SIZE 100
#define MAX_TOKEN_SIZE 20
#define MAX_STACK_SIZE 100

typedef struct {
    double data[MAX_STACK_SIZE];
    int top;
} Stack;

typedef struct {
    char *data[MAX_STACK_SIZE];
    int top;
} ExprStack;

void init_stack(Stack *s) {
    s->top = -1;
}

void init_expr_stack(ExprStack *s) {
    s->top = -1;
}

int is_empty(Stack *s) {
    return s->top == -1;
}

int is_expr_empty(ExprStack *s) {
    return s->top == -1;
}

int is_full(Stack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

int is_expr_full(ExprStack *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

void push(Stack *s, double value) {
    if (!is_full(s)) {
        s->data[++s->top] = value;
    }
}

void push_expr(ExprStack *s, char *value) {
    if (!is_expr_full(s)) {
        s->data[++s->top] = value;
    }
}

double pop(Stack *s) {
    if (!is_empty(s)) {
        return s->data[s->top--];
    }
    return 0;
}

char *pop_expr(ExprStack *s) {
    if (!is_expr_empty(s)) {
        return s->data[s->top--];
    }
    return NULL;
}

double top(Stack *s) {
    if (!is_empty(s)) {
        return s->data[s->top];
    }
    return 0;
}

char *top_expr(ExprStack *s) {
    if (!is_expr_empty(s)) {
        return s->data[s->top];
    }
    return NULL;
}

double apply_operator(char op, double a, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

double apply_function(const char *func, double a) {
    if (strcmp(func, "raiz") == 0) return sqrt(a);
    if (strcmp(func, "sen") == 0) return sin(a);
    if (strcmp(func, "cos") == 0) return cos(a);
    if (strcmp(func, "tg") == 0) return tan(a);
    if (strcmp(func, "log") == 0) return log10(a);
    return 0;
}

double evaluate_postfix(const char *expression, char *infix) {
    Stack s;
    ExprStack es;
    init_stack(&s);
    init_expr_stack(&es);

    char token[MAX_TOKEN_SIZE];
    int i = 0, j = 0;

    while (expression[i] != '\0') {
        if (expression[i] == ' ') {
            token[j] = '\0';
            if (j > 0) {
                if (strchr("+-*/^", token[0]) && j == 1) {
                    double b = pop(&s);
                    double a = pop(&s);
                    double result = apply_operator(token[0], a, b);
                    push(&s, result);

                    char *right = pop_expr(&es);
                    char *left = pop_expr(&es);
                    char *new_expr = malloc(MAX_EXPR_SIZE);
                    sprintf(new_expr, "(%s %c %s)", left, token[0], right);
                    push_expr(&es, new_expr);

                } else if (strstr("raiz sen cos tg log", token) != NULL) {
                    double a = pop(&s);
                    double result = apply_function(token, a);
                    push(&s, result);

                    char *operand = pop_expr(&es);
                    char *new_expr = malloc(MAX_EXPR_SIZE);
                    sprintf(new_expr, "%s(%s)", token, operand);
                    push_expr(&es, new_expr);

                } else {
                    push(&s, atof(token));
                    char *operand = malloc(MAX_TOKEN_SIZE);
                    strcpy(operand, token);
                    push_expr(&es, operand);
                }
                j = 0;
            }
        } else {
            token[j++] = expression[i];
        }
        i++;
    }

    token[j] = '\0';
    if (j > 0) {
        if (strchr("+-*/^", token[0]) && j == 1) {
            double b = pop(&s);
            double a = pop(&s);
            double result = apply_operator(token[0], a, b);
            push(&s, result);

            char *right = pop_expr(&es);
            char *left = pop_expr(&es);
            char *new_expr = malloc(MAX_EXPR_SIZE);
            sprintf(new_expr, "(%s %c %s)", left, token[0], right);
            push_expr(&es, new_expr);

        } else if (strstr("raiz sen cos tg log", token) != NULL) {
            double a = pop(&s);
            double result = apply_function(token, a);
            push(&s, result);

            char *operand = pop_expr(&es);
            char *new_expr = malloc(MAX_EXPR_SIZE);
            sprintf(new_expr, "%s(%s)", token, operand);
            push_expr(&es, new_expr);

        } else {
            push(&s, atof(token));
            char *operand = malloc(MAX_TOKEN_SIZE);
            strcpy(operand, token);
            push_expr(&es, operand);
        }
    }

    strcpy(infix, pop_expr(&es));
    return pop(&s);
}

int main() {
    char expression[MAX_EXPR_SIZE];
    char infix[MAX_EXPR_SIZE] = "";

    printf("Digite a expressão em notação pós-fixada: ");
    fgets(expression, MAX_EXPR_SIZE, stdin);
    expression[strcspn(expression, "\n")] = '\0';  // Remove newline character if exists

    double result = evaluate_postfix(expression, infix);
    printf("Resultado: %.2f\n", result);
    printf("Expressão Infixa: %s\n", infix);

    return 0;
}
