%{
#include "9cc.h"
%}

%%

"+" return '+';
"-" return '-';
"*" return '*';
"/" return '/';
"(" return '(';
")" return ')';
";" return ';';
"!=" return TK_NE;
"==" return TK_EQ;
"=" return '=';
[0-9][0-9]* {
    int tmp;
    sscanf(yytext, "%d", &tmp);
    yylval.val = tmp;
    return TK_NUM;
}
[a-zA-Z][a-zA-Z0-9]* {
    strcpy(yylval.name, yytext); 
    return TK_IDENT;
} 

%%
