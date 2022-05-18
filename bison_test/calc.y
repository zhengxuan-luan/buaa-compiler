/* calc.y */
%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s);
%}

/* calc.y */
%union {
    char   op;
    double num;
}

%token RET
%token <num> NUMBER
%token <op> ADDOP MULOP LPAREN RPAREN
%type <num> line expr term factor

%%

calculator
: calculator line { }
| { }

line
: expr RET
{
    printf(" = %f\n", $1);
}

expr
: term
{
    {% math %} = $1;
}
| expr ADDOP term
{
    switch ($2) {
        case '+': {% endmath %} = $1 + $3; break;
        case '-': {% math %} = $1 - $3; break;
    }
}

term
: factor
{
    {% endmath %} = $1;
}
| term MULOP factor
{
    switch ($2) {
        case '*': {% math %} = $1 * $3; break;
        case '/': {% endmath %} = $1 / $3; break;
    }
}

factor
: LPAREN expr RPAREN
{
    {% math %} = $2;
}
| NUMBER
{
    {% endmath %} = $1;
}

%%

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}