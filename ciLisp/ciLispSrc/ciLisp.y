%{
    #include "ciLisp.h"
%}

%union {
    double dval;
    char *sval;
    struct ast_node *astNode;
    struct symbol_table_node *symbolNode
};

%token <sval> FUNC SYMBOL
%token <dval> NUMBER INTEGER_NUM REAL_NUM TYPE
%token LPAREN RPAREN LET EOL QUIT COND LAMBDA

%type <astNode> s_expr s_expr_list
%type <symbolNode> let_elem let_section let_list arg_list

%%

program:
    s_expr EOL {
        fprintf(stderr, "yacc: program ::= s_expr EOL\n");
        eval($1);
        freeNode($1);
    };

s_expr:
    INTEGER_NUM {
        fprintf(stderr, "yacc: s_expr ::= INTEGER\n");
        $$ = number($1);
    }
    | REAL_NUM {
        fprintf(stderr, "yacc: s_expr ::= REAL\n");
        $$ = number($1);
    }
    | LPAREN FUNC s_expr_list RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN FUNC s_expr_list RPAREN\n");
        $$ = function($2, $3);
    }
    | LPAREN SYMBOL s_expr_list RPAREN {
         fprintf(stderr, "yacc: s_expr ::= LPAREN SYMBOL s_expr_list RPAREN\n");
         $$ = function($2, $3);
    }
    | SYMBOL {
        fprintf(stderr, "yacc: s_expr ::= SYMBOL \n");
        $$ = symbol($1);
    }
    | LPAREN let_section s_expr RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN let_section s_expr RPAREN \n");
        $$ = setSymbolTable($2, $3);
    }
    | LPAREN COND s_expr s_expr s_expr RPAREN {
        fprintf(stderr, "yacc: s_expr ::= LPAREN COND s_expr s_expr s_expr RPAREN \n");
        $$ = condition($3, $4, $5);
    }
    | QUIT {
        fprintf(stderr, "yacc: s_expr ::= QUIT\n");
        exit(EXIT_SUCCESS);
    }
    | error {
        fprintf(stderr, "yacc: s_expr ::= error\n");
        yyerror("unexpected token");
        $$ = NULL;
    };

s_expr_list:
    s_expr s_expr_list {
        fprintf(stderr, "yacc: s_expr_list ::= s_expr s_expr_list\n");
        $$ = addToOpList($1, $2);
    }
    | s_expr {
        fprintf(stderr, "yacc: s_expr_list ::= s_expr\n");
        $$ = createNewOpList($1);
    }
    | {
        $$ = NULL;  //EMPTY
    };

let_section:
    LPAREN let_list RPAREN {
        fprintf(stderr, "yacc: let_section ::= LPAREN let_list RPAREN\n");
        $$ = $2;
    }
    | {
        $$ = NULL; //EMPTY
    };

let_list:
    let_list let_elem {
        fprintf(stderr, "yacc: let_list ::= let_list let_elem\n");
        $$ = addSymbolToList($1, $2);
    }
    | LET let_elem {
        fprintf(stderr, "yacc: let_list ::= LET let_elem\n");
        $$ = $2;
    };

let_elem:
    LPAREN TYPE SYMBOL s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN TYPE SYMBOL s_expr RPAREN\n");
        $$ = createSymbol ($2, $3, $4, VARIABLE_TYPE, NULL);
    }
    | LPAREN SYMBOL s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL s_expr RPAREN\n");
        $$ = createSymbol (NO_TYPE, $2, $3, VARIABLE_TYPE, NULL);
    }
    | LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN TYPE SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN\n");
        $$ = createSymbol ($2, $3, $8, LAMBDA_TYPE, $6);
    }
    | LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN {
        fprintf(stderr, "yacc: let_elem ::= LPAREN SYMBOL LAMBDA LPAREN arg_list RPAREN s_expr RPAREN\n");
        $$ = createSymbol (NO_TYPE, $2, $7, LAMBDA_TYPE, $5);
    };

arg_list:
    SYMBOL arg_list {
        fprintf(stderr, "yacc: arg_list ::= SYMBOL arg_list\n");
        $$ = createSymbol (REAL_TYPE, $1, NULL, ARG_TYPE, $2);
    }
    | SYMBOL {
        fprintf(stderr, "yacc: arg_list ::= SYMBOL\n");
        $$ = createSymbol (REAL_TYPE, $1, NULL, ARG_TYPE, NULL);
    }
%%

