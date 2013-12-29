%{
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "gel-parser.h"
%}
%option noyywrap
%%
"function"     return FUNCTION;
"returns"      return RETURNS;
"return"       return RETURN;
"integer"      return INTEGER;
"float"        return FLOAT;
L?\"(\\.|[^\\"])*\" yylval.string = xstrdup(yytext); return STRING_LITERAL;
[ \n\t]         ;                   // white space 
[A-Za-z_]+      {yylval.name = xstrdup(yytext); return NAME;} // names
[0-9]+          {yylval.ival = atoi(yytext); return NUM;}   // integers
"+"             {return '+';}
"-"             {return '-';}
"*"             {return '*';}
"/"             {return '/';}
";"             {return ';';}
"("             {return '(';}
")"             {return ')';}
"="             {return '=';}
"{"             {return '{';}
"}"             {return '}';}
","             {return ',';}
%%
