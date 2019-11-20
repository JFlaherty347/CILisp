/**
* Name: Joseph Flaherty
* Project: CILisp
* Date: 5/8/19
**/
#ifndef __cilisp_h_
#define __cilisp_h_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "ciLispParser.h"

//enums
typedef enum oper { // must be in sync with funcs in resolveFunc()
    NEG_OPER, // 0
    ABS_OPER, // 1
    EXP_OPER, // 2
    SQRT_OPER, // 3
    ADD_OPER, // 4
    SUB_OPER, // 5
    MULT_OPER, // 6
    DIV_OPER, // 7
    REMAINDER_OPER, // 8
    LOG_OPER, // 9
    POW_OPER, // 10
    MAX_OPER, // 11
    MIN_OPER, // 12
    EXP2_OPER, // 13
    CBRT_OPER, // 14
    HYPOT_OPER, // 15
    PRINT_OPER, //16
    READ_OPER, //17
    RAND_OPER, //18
    EQUAL_OPER, //19
    SMALLER_OPER, //20
    LARGER_OPER, //21
    CUSTOM_FUNC=255
} OPER_TYPE;

typedef enum { NO_TYPE, INTEGER_TYPE, REAL_TYPE } DATA_TYPE;

typedef enum { VARIABLE_TYPE, LAMBDA_TYPE, ARG_TYPE } SYMBOL_TYPE;

typedef enum { NUM_TYPE, FUNC_TYPE, SYMB_TYPE, COND_TYPE } AST_NODE_TYPE;

//structs
typedef struct return_value {
    DATA_TYPE type;
    double value;
} RETURN_VALUE;

typedef struct stack_node {
    struct ast_node *value;
    struct stack_node *next;
} STACK_NODE;

typedef struct {
    DATA_TYPE valueType;
    double value;
} NUMBER_AST_NODE;

typedef struct {
    char *name;
    struct ast_node *opList;
} FUNCTION_AST_NODE;

typedef struct {
    char *name;
    DATA_TYPE valueType;
} SYMBOL_AST_NODE;

typedef struct {
    struct ast_node *condition;
    struct ast_node *trueCase;
    struct ast_node *falseCase;
} COND_AST_NODE;

typedef struct symbol_table_node {
    SYMBOL_TYPE symbolType;
    DATA_TYPE valueType;
    char *name;
    struct ast_node *value;
    STACK_NODE *stack;
    struct symbol_table_node *next;
} SYMBOL_TABLE_NODE;

typedef struct ast_node {
    AST_NODE_TYPE type;
    SYMBOL_TABLE_NODE *symbolTable;
    struct ast_node *parent;
    union {
        NUMBER_AST_NODE number;
        FUNCTION_AST_NODE function;
        SYMBOL_AST_NODE symbol;
        COND_AST_NODE condition;
    } data;
    struct ast_node *next;
} AST_NODE;


//declarations; please do these :>
AST_NODE *number(double value);
AST_NODE *function(char *funcName, AST_NODE *opList);
void freeNode(AST_NODE *p);

RETURN_VALUE *eval(AST_NODE *p);
SYMBOL_TABLE_NODE *addSymbolToList(SYMBOL_TABLE_NODE *letList, SYMBOL_TABLE_NODE *symbolToAdd);
AST_NODE *setSymbolTable(SYMBOL_TABLE_NODE *symbolTable, AST_NODE *s_expr);
SYMBOL_TABLE_NODE *createSymbol(DATA_TYPE dataType, char *symbolName, AST_NODE *s_expr, SYMBOL_TYPE symbolType, SYMBOL_TABLE_NODE *arg_list);

AST_NODE *symbol(char *symbolName);
AST_NODE *addToOpList(AST_NODE *s_expr, AST_NODE *s_expr_list);
AST_NODE *createNewOpList(AST_NODE *s_expr);
AST_NODE *condition(AST_NODE *condition, AST_NODE *trueCase, AST_NODE *falseCase);

STACK_NODE *createStackNode(AST_NODE *value);
SYMBOL_TABLE_NODE *resolveSymbolValue(AST_NODE *currentScope, SYMBOL_TABLE_NODE *currentNode, char *symbolName);
OPER_TYPE resolveFunc(char *funcName);

int yyparse(void);
int yylex(void);
void yyerror(char *);

#endif