/**
* Name: Joseph Flaherty
* Project: CILisp
* Date: 5/8/19
**/
#include "ciLisp.h"

void yyerror(char *s)
{
    fprintf(stderr, "\nERROR: %s\n", s);
    // note stderr that normally defaults to stdout, but can be redirected: ./src 2> src.log
    // CLion will display stderr in a different color from stdin and stdout
}

//
// find out which function it is
//
char *func[] = {"neg",//0
                "abs", //1
                "exp", //2
                "sqrt", //3
                "add", //4
                "sub", //5
                "mult", //6
                "div", //7
                "remainder", //8
                "log", //9
                "pow", //10
                "max", //11
                "min", //12
                "exp2", //13
                "cbrt", //14
                "hypot", //15
                "print", //16
                "read", //17
                "rand", //18
                "equal", //19
                "smaller", //20
                "larger", //21
                ""};

//
//return an enum value from name of function
//
OPER_TYPE resolveFunc(char *funcName)
{
    if (funcName != NULL)
    {
        int i = 0;
        while (func[i][0] != '\0')
        {
            if (strcmp(func[i], funcName) == 0)
                return i;

            i++;
        }
    }

    return CUSTOM_FUNC;
}

//
//performs symbol table lookup based on a symbol name/
//
SYMBOL_TABLE_NODE *resolveSymbolValue(AST_NODE *currentScope, SYMBOL_TABLE_NODE *currentNode, char *symbolName)
{
    SYMBOL_TABLE_NODE *symbol = NULL;

    if (currentNode != NULL)
    {
        if (strcmp(currentNode->name, symbolName) == 0)
        {
            symbol = currentNode;
        }
        else
        {
            if (currentNode->next != NULL) //search all nodes in scope
                symbol = resolveSymbolValue(currentScope, currentNode->next, symbolName);
            else if (currentScope != NULL && currentScope->parent != NULL) //then move up to next scope
                symbol = resolveSymbolValue(currentScope->parent, currentScope->parent->symbolTable, symbolName);
            else //otherwise it was not found
                yyerror("Undefined symbol!");
        }

    }
    else
    {
        //for when symbol table is empty but symbol exists in higher scope
        if (currentScope != NULL && currentScope->parent != NULL)
            symbol = resolveSymbolValue(currentScope->parent, currentScope->parent->symbolTable, symbolName);
        else
            yyerror("Undefined symbol!");
    }

    return symbol;
}

//
// create a node for a number
//
AST_NODE *number(double value)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(NUMBER_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = NUM_TYPE;
    p->data.number.valueType = REAL_TYPE;
    p->data.number.value = value;

    return p;
}

//
// create a node for a function
//
AST_NODE *function(char *funcName, AST_NODE *opList)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(FUNCTION_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = FUNC_TYPE;
    p->data.function.name = funcName;
    p->data.function.opList = opList;
    p->parent = NULL;

    AST_NODE *currentNode = opList;
    while (currentNode != NULL)
    {
        currentNode->parent = p;

        currentNode = currentNode->next;
    }

    p->symbolTable = NULL;
    p->next = NULL;

    return p;
}

//
// free a node :)
//
void freeNode(AST_NODE *p)
{
    if (!p)
        return;

    free(p);
}

//
// evaluate an abstract syntax tree
// p points to the root
//
RETURN_VALUE *eval(AST_NODE *p)
{
    RETURN_VALUE *resultValue = malloc(sizeof(RETURN_VALUE));
    RETURN_VALUE *op1 = NULL;
    RETURN_VALUE *op2 = NULL;


    resultValue->type = NO_TYPE; //default value; should change

    if (!p)
        printf("Null Node!\n"); //keep result as NaN
    else
    {
        switch (p->type)
        {
            case NUM_TYPE:
                resultValue->value = p->data.number.value;
                resultValue->type = p->data.number.valueType;
                break;

            case FUNC_TYPE:


                switch (resolveFunc(p->data.function.name))
                {
                    case NEG_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = -(op1->value);
                            resultValue->type = op1->type;

                            if (p->data.function.opList->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next = NULL;
                            }
                        }
                        break;

                    case ABS_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = fabs(op1->value);
                            resultValue->type = op1->type;

                            if (p->data.function.opList->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next = NULL;
                            }
                        }
                        break;

                    case EXP_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = exp(op1->value);
                            resultValue->type = op1->type;

                            if (p->data.function.opList->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next = NULL;
                            }
                        }
                        break;

                    case SQRT_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = sqrt(op1->value);
                            resultValue->type = op1->type;

                            if (p->data.function.opList->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next = NULL;
                            }
                        }
                        break;

                    case ADD_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = (op1->value);
                            resultValue->type = (op1->type);

                            AST_NODE *currentOperand = p->data.function.opList->next;
                            while (currentOperand != NULL)
                            {
                                op2 = eval(currentOperand);

                                resultValue->value += (op2->value);
                                resultValue->type = (resultValue->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                    ? INTEGER_TYPE : REAL_TYPE;

                                currentOperand = currentOperand->next;
                            }
                        }
                        break;

                    case SUB_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = (op1->value) - (op2->value);
                            resultValue->type = (op1->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                ? INTEGER_TYPE : REAL_TYPE;

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case MULT_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = (op1->value);
                            resultValue->type = (op1->type);

                            AST_NODE *currentOperand = p->data.function.opList->next;
                            while (currentOperand != NULL)
                            {
                                op2 = eval(currentOperand);

                                resultValue->value *= (op2->value);
                                resultValue->type = (resultValue->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                    ? INTEGER_TYPE : REAL_TYPE;

                                currentOperand = currentOperand->next;
                            }
                        }
                        break;

                    case DIV_OPER:;
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            //calculate division if op2 isn't 0
                            if (op2->value != 0)
                                resultValue->value = (op1->value) / (op2->value);
                            else
                            {
                                yyerror("ERROR: Cannot divide by 0! Returning NaN.");
                                resultValue->value = NAN;
                            }
                            resultValue->type = (op1->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                ? INTEGER_TYPE : REAL_TYPE;


                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case REMAINDER_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = fmod(op1->value, op2->value);
                            resultValue->type = (op1->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                ? INTEGER_TYPE : REAL_TYPE;

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case LOG_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = log(op1->value);
                            resultValue->type = op1->type;

                            if (p->data.function.opList->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next = NULL;
                            }
                        }
                        break;

                    case POW_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = pow(op1->value, op2->value);
                            resultValue->type = (op1->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                ? INTEGER_TYPE : REAL_TYPE;

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case MAX_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = fmax(op1->value, op2->value);
                            resultValue->type = (op1->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                ? INTEGER_TYPE : REAL_TYPE;

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case MIN_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = fmin(op1->value, op2->value);
                            resultValue->type = (op1->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                ? INTEGER_TYPE : REAL_TYPE;

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case EXP2_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = exp2(op1->value);
                            resultValue->type = op1->type;

                            if (p->data.function.opList->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next = NULL;
                            }
                        }
                        break;

                    case CBRT_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);

                            resultValue->value = cbrt(op1->value);
                            resultValue->type = op1->type;

                            if (p->data.function.opList->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next = NULL;
                            }
                        }
                        break;

                    case HYPOT_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = hypot(op1->value, op2->value);
                            resultValue->type = (op1->type == INTEGER_TYPE && op2->type == INTEGER_TYPE)
                                                ? INTEGER_TYPE : REAL_TYPE;

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case PRINT_OPER:
                        if (p->data.function.opList == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            AST_NODE *currentOperand = p->data.function.opList;
                            while (currentOperand != NULL)
                            {
                                op1 = eval(p->data.function.opList);

                                //only printing, so return value is just s_expr
                                resultValue->value = op1->value;
                                resultValue->type = op1->type;

                                op1 = eval(currentOperand);

                                if (op1 != NULL)
                                {
                                    if (op1->type == INTEGER_TYPE)
                                    {
                                        printf("%.0lf ", op1->value);
                                        fflush(stdout);
                                    }
                                    else
                                    {
                                        printf("%.2lf ", op1->value);
                                        fflush(stdout);
                                    }

                                }
                                else
                                    printf(">:( ");

                                currentOperand = currentOperand->next;

                            }

                            printf("\n"); //newline after one print statement

                        }
                        break;

                    case READ_OPER:;
                        double input = 0.0;
                        char *buffer = malloc(32 * sizeof(char)); //buffer of 32 chars after num

                        printf("read := ");

                        scanf("%lf", &input);
                        fgets(buffer, 32, stdin); //get up to 32 extra chars
                        free(buffer); //buffer no longer needed

                        //convert from string, if non-number written, will return regular error value of 0.0;
                        resultValue->value = input;

                        if (fmod(input, 1) != 0)
                            resultValue->type = REAL_TYPE;
                        else
                            resultValue->type = INTEGER_TYPE;

                        p->type = NUM_TYPE;
                        p->data.number.value = resultValue->value;
                        p->data.number.valueType = resultValue->type;
                        break;

                    case RAND_OPER:
                        resultValue->value =  ((double)rand()/rand());
                        resultValue->type = REAL_TYPE;
                        break;

                    case EQUAL_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = (op1->value == op2->value) ? 1.0 : 0.0;
                            resultValue->type = REAL_TYPE; //boolean :)

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case SMALLER_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = (op1->value < op2->value) ? 1.0 : 0.0;
                            resultValue->type = REAL_TYPE; //boolean :)

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case LARGER_OPER:
                        if (p->data.function.opList == NULL || p->data.function.opList->next == NULL)
                        {
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            op1 = eval(p->data.function.opList);
                            op2 = eval(p->data.function.opList->next);

                            resultValue->value = (op1->value > op2->value) ? 1.0 : 0.0;
                            resultValue->type = REAL_TYPE; //boolean :)

                            if (p->data.function.opList->next->next != NULL)
                            {
                                yyerror("WARNING: Too many operands! Ignoring surplus operands.");
                                p->data.function.opList->next->next = NULL;
                            }
                        }
                        break;

                    case CUSTOM_FUNC:;
                        //***Setup up lambda info***
                        //get lambda definition
                        SYMBOL_TABLE_NODE *customFunction = resolveSymbolValue(p, p->symbolTable,
                                                                               p->data.function.name);
                        //get actual parameters from current AST_NODE
                        AST_NODE *currentActualParameter = p->data.function.opList;
                        //get formal parameters from lambda definition
                        SYMBOL_TABLE_NODE *currentFormalParameter = customFunction->value->symbolTable;
                        STACK_NODE *newStackEntry;
                        STACK_NODE *stackEntryToRemove;

                        //***add actual parameters to formal parameter stacks***
                        while (currentActualParameter != NULL && currentFormalParameter != NULL)//here
                        {

                            //create new stack node and add it to the top of the stack
                            newStackEntry = createStackNode(currentActualParameter);
                            newStackEntry->next = currentFormalParameter->stack;
                            currentFormalParameter->stack = newStackEntry;

                            currentActualParameter = currentActualParameter->next;
                            currentFormalParameter = currentFormalParameter->next;
                        }

                        //***check parameter count and evaluate lambda expression***
                        if (currentActualParameter == NULL && currentFormalParameter != NULL)
                        {
                            //too little
                            //there are still formal parameters to fill but not enough actual parameters to fill them
                            yyerror("ERROR: Not enough operands! Returning 0.0.");
                            resultValue->value = 0.0;
                            resultValue->type = REAL_TYPE;
                        }
                        else
                        {
                            if (currentActualParameter != NULL && currentFormalParameter == NULL)//and here
                            {
                                //too many
                                //there are still actual parameters left but not enough formal parameters to place them in
                                yyerror("WARNING: Too many operands! Ignoring surplus operands. dummy :)");
                            }

                            resultValue = eval(customFunction->value);
                        }

                        //***Pop actual parameters from formal parameter stacks***
                        //Note that this will function regardless of if there are a correct amount of parameters used
                        currentActualParameter = p->data.function.opList;
                        currentFormalParameter = customFunction->value->symbolTable;
                        while (currentActualParameter != NULL && currentFormalParameter != NULL)
                        {
                            //take entry from stack and make its next element the new top of the stack
                            stackEntryToRemove = currentFormalParameter->stack;
                            currentFormalParameter->stack = stackEntryToRemove->next;
                            stackEntryToRemove = NULL;

                            currentActualParameter = currentActualParameter->next;
                            currentFormalParameter = currentFormalParameter->next;
                        }

                        break;
                }
                break;

            case SYMB_TYPE:;
                //resultValue = resolveSymbolValue(p->parent, p->parent->symbolTable, p->data.symbol.name);
                SYMBOL_TABLE_NODE *symbol = resolveSymbolValue(p, p->symbolTable, p->data.symbol.name);

                if (symbol->symbolType == VARIABLE_TYPE)
                {
                    //EXTRA CREDIT:
                    //if the symbol is to be filled by a read value, read here instead of in eval in
                    //order to print the symbol name while the user inputs the value :)
                    if((symbol->value->type == FUNC_TYPE) && (strcmp(symbol->value->data.function.name, "read") == 0))
                    {
                        double input = 0.0;
                        char *buffer = malloc(32 * sizeof(char)); //buffer of 32 chars after num

                        printf("%s := ", p->data.symbol.name);

                        scanf("%lf", &input);
                        fgets(buffer, 32, stdin); //get up to 32 extra chars
                        free(buffer); //buffer no longer needed

                        //convert from string, if non-number written, will return regular error value of 0.0;
                        resultValue->value = input;

                        if (fmod(input, 1) != 0)
                            resultValue->type = REAL_TYPE;
                        else
                            resultValue->type = INTEGER_TYPE;

                        p->type = NUM_TYPE;
                        p->data.number.value = resultValue->value;
                        p->data.number.valueType = resultValue->type;
                    }
                    else
                        resultValue = eval(symbol->value);
                }
                else if (symbol->symbolType == ARG_TYPE)
                    resultValue = eval(symbol->stack->value);
                else
                    printf("Something went horribly awry, but like with symbols...\n");


                if (resultValue->type == INTEGER_TYPE)
                {
                    if (fmod(resultValue->value, 1) != 0)
                        yyerror("WARNING: Loss of precision!\n");

                    resultValue->value = round(resultValue->value);
                }


                break;

            case COND_TYPE:
                resultValue = (eval(p->data.condition.condition)->value != 0.0) ?
                              eval(p->data.condition.trueCase) :
                              eval(p->data.condition.falseCase);
                break;

            default:
                //should never be called; only prints if an unrecognized node type is used
                printf("Something went horribly awry...\n");
                break;
        }


    }

    return resultValue;
}

//
//create a symbol to keep record of value; called for let_elems, lambdas, and args
//
SYMBOL_TABLE_NODE *createSymbol(DATA_TYPE dataType, char *symbolName, AST_NODE *s_expr, SYMBOL_TYPE symbolType,
                                SYMBOL_TABLE_NODE *arg_list)
{
    SYMBOL_TABLE_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed sie and the variable part (union)
    nodeSize = sizeof(SYMBOL_TABLE_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->symbolType = symbolType;
    p->valueType = dataType;
    p->name = symbolName;
    p->value = s_expr;

    if (p->value != NULL)
    {

        if (symbolType == LAMBDA_TYPE)
            p->value->symbolTable = arg_list;

        if (p->value->type == SYMB_TYPE)
            p->value->data.symbol.valueType = dataType;
    }

    p->stack = NULL;

    if (arg_list != NULL)
        p->next = arg_list;
    else
        p->next = NULL;

    return p;
}

//
//adds one new symbol; called for letList
//
SYMBOL_TABLE_NODE *addSymbolToList(SYMBOL_TABLE_NODE *letList, SYMBOL_TABLE_NODE *symbolToAdd)
{
    if (symbolToAdd != NULL)
    {
        bool duplicateFound = false;

        SYMBOL_TABLE_NODE *currentNode = letList;
        while (currentNode != NULL && !duplicateFound)
        {
            if (strcmp(currentNode->name, symbolToAdd->name) == 0)
            {
                duplicateFound = true;
                yyerror("ERROR: Variable already declared");
                currentNode->value = symbolToAdd->value;
                symbolToAdd = letList; //return table due to duplicate
            }
            else
                currentNode = currentNode->next;
        }

        if (!duplicateFound)
            symbolToAdd->next = letList;

        if (symbolToAdd->value != NULL && letList->value != NULL)
            symbolToAdd->value->parent = letList->value->parent;
    }
    else
        symbolToAdd = letList; //return table if null


    return symbolToAdd;
}

//
//creates a symbol node to be evaluated in AST
//
AST_NODE *symbol(char *symbolName)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed size and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(SYMBOL_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = SYMB_TYPE;
    p->data.symbol.name = symbolName;
    p->data.symbol.valueType = NO_TYPE;
    p->symbolTable = NULL;
    p->parent = NULL;
    p->next = NULL;

    return p;
}

//
//create a condition AST_NODE
//
AST_NODE *condition(AST_NODE *condition, AST_NODE *trueCase, AST_NODE *falseCase)
{
    AST_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed size and the variable part (union)
    nodeSize = sizeof(AST_NODE) + sizeof(COND_AST_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->type = COND_TYPE;
    p->symbolTable = NULL;
    p->parent = NULL;

    p->data.condition.condition = condition;
    p->data.condition.trueCase = trueCase;
    p->data.condition.falseCase = falseCase;

    //set parents of condition and cases
    p->data.condition.condition->parent = p;
    p->data.condition.trueCase->parent = p;
    p->data.condition.falseCase->parent = p;

    p->next = NULL;

    return p;
}

//
//create a stackNode for use in lambdas statements
//
STACK_NODE *createStackNode(AST_NODE *value)
{
    STACK_NODE *p;
    size_t nodeSize;

    // allocate space for the fixed size and the variable part (union)
    nodeSize = sizeof(STACK_NODE);
    if ((p = malloc(nodeSize)) == NULL)
        yyerror("out of memory");

    p->value = value;
    p->next = NULL;

    return p;
}

//
// used to add an operand to an already existing op list
//
AST_NODE *addToOpList(AST_NODE *s_expr, AST_NODE *s_expr_list)
{
    s_expr->next = s_expr_list; //add new s_expr to list

    return s_expr; //return new head of list
}

//
// used to start a new operand list; sets its next to NULL so it doesn't point to random garbage data
//
AST_NODE *createNewOpList(AST_NODE *s_expr)
{
    s_expr->next = NULL; //new list with next as null

    return s_expr; //return new head of list
}

//
//create table for symbols; called for let
//
AST_NODE *setSymbolTable(SYMBOL_TABLE_NODE *symbolTable, AST_NODE *s_expr)
{

    if (s_expr != NULL)
        s_expr->symbolTable = symbolTable;

    SYMBOL_TABLE_NODE *currentNode = symbolTable;
    while (currentNode != NULL) //set parent for each member of symbol table
    {
        if (currentNode->value != NULL)
            currentNode->value->parent = s_expr;

        currentNode = currentNode->next;
    }

    return s_expr;
}