%option noyywrap

%{
#include "calc.tab.h"  
%}

%%
\( {return LPAREN;} 
\) {return RPAREN;}
"+"|"-" {yylval.op = yytext[0]; return ADDOP;}
"*"|"/" {yylval.op = yytext[0]; return MULOP;}
[0-9]+|[0-9]+\.[0-9]*|[0-9]*\.[0-9]+ { yylval.num = atof(yytext); return NUMBER; }
" "|\t { }
\r\n|\n|\r {return RET;}
%%