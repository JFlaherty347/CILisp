%option noyywrap
%option nounput
%option noinput

%{
    #include "ciLisp.h"
%}

digit [0-9]
real_num [-]?{digit}+\.{digit}+?
integer_num [-]?{digit}+

func "neg"|"abs"|"exp"|"sqrt"|"add"|"sub"|"mult"|"div"|"remainder"|"log"|"pow"|"max"|"min"|"exp2"|"cbrt"|"hypot"|"print"|"read"|"rand"|"equal"|"smaller"|"larger"
quit "quit"|"exit"|"/ff"|"altf4"|"LRAST"|"lrast"

letter [a-zA-Z]
symbol {letter}+

%%

{integer_num} {
    yylval.dval = (int)strtod(yytext, NULL);
    fprintf(stderr, "lex: INTEGER dval = %lf\n", yylval.dval);
    return INTEGER_NUM;
    }

{real_num} {
    yylval.dval = strtod(yytext, NULL);
    fprintf(stderr, "lex: REAL dval = %lf\n", yylval.dval);
    return REAL_NUM;
    }

"integer" {
        yylval.dval = INTEGER_TYPE;
        fprintf(stderr, "lex: TYPE(int) dval = %lf\n", yylval.dval);
        return TYPE;
    }

"real" {
        yylval.dval = REAL_TYPE;
        fprintf(stderr, "lex: TYPE(real) dval = %lf\n", yylval.dval);
        return TYPE;
    }

{quit}  {
            fprintf(stderr, "lex: QUIT\n");
            return QUIT;
        }

"let"   {
            fprintf(stderr, "lex: LET\n");
            return LET;
        }

"cond"  {
            fprintf(stderr, "lex: COND\n");
            return COND;
        }

"lambda" {
            fprintf(stderr, "lex: LAMBDA\n");
            return LAMBDA;
         }

{func} {
    yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char) + 1);
    strcpy(yylval.sval, yytext);
    fprintf(stderr, "lex: FUNC sval = %s\n", yylval.sval);
    return FUNC;
    }

{symbol} {
    yylval.sval = (char *) malloc(strlen(yytext)*sizeof(char) + 1);
    strcpy(yylval.sval, yytext);
    fprintf(stderr, "lex: SYMBOL sval = %s\n", yylval.sval);
    return SYMBOL;
    }

"(" {
    fprintf(stderr, "lex: LPAREN\n");
    return LPAREN;
    }

")" {
    fprintf(stderr, "lex: RPAREN\n");
    return RPAREN;
    }

[\n] {
    fprintf(stderr, "lex: EOL\n");
    YY_FLUSH_BUFFER;
    return EOL;
    }

[ \t] {}; /* skip whitespace */

. {
   printf("ERROR: invalid character: >>%s<<\n", yytext);
}

%%

/*
 * DO NOT CHANGE THE FOLLOWING CODE CRITICAL FOR REPL!
 */
int main(void) {

    //   freopen("/dev/null", "w", stderr); // except for this line that can be uncommented to throw away debug printouts

    char *s_expr_str = NULL;
    size_t s_expr_str_len = 0;
    YY_BUFFER_STATE buffer;
    while (true) {
        printf("\n> ");
        getline(&s_expr_str, &s_expr_str_len, stdin);
        s_expr_str[s_expr_str_len++] = '\0';
        s_expr_str[s_expr_str_len++] = '\0';
        buffer = yy_scan_buffer(s_expr_str, s_expr_str_len);
        yyparse();
        yy_delete_buffer(buffer);
    }
}
