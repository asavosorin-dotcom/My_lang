#include "../../headers/back-end/back-end.h"

#define aa(...) printf(__VA_ARGS__)
#define $(...) aa(__VA_ARGS__); fprintf(file_asm, __VA_ARGS__)
#define ERROR(...) fprintf(stderr, BOLD_RED  __VA_ARGS__); fprintf(stderr, RESET);

int node_is_op(CompNode_t* node, Operator_val_t val);
int get_index_var(CompNode_t* node, StackString_t* variables, Function_t* func);
// брать название из ключей компилляции
FILE* file_asm = fopen("./compile_files/file_asm.s", "w");

// ============================================== Rules =================================================
// Все результаты выражений и возвращаемые значения функций лежат в rax 
// Для ассемблирования части кода важно из какой функции она была вызвана для регулирования областей видимости
// ======================================================================================================

void MakeAsmCode(CompNode_t* root, StackFunc_t* functions, StackString_t* variables)
{
    // MakeAsmNode(root, functions, variables);
}

void MakeAsmNode(CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func)
{
    if (!node) return ;

    switch (node->type)
    {
        case FUNC_INIT:
        {
            // тут должна быть метка на функцию    
            int index_func = node->value.index_var;
            Function_t* func_init = &(functions->data[index_func]);

            $("%s:\n", func_init->name);
            $("push rbp\n");
            $("mov rbp, rsp\n");

            // Function_t func = {};
            // func.name       = node->value.var;
            
            // StackPrint(variables);

            func_init->begin_params = variables->size;
            // aa("begin_params = [%d]\n", func_init->begin_params);
            ParsingParams(node->left, variables, func_init);
            STRING_PUSH(variables, strdup("__CALL_RET__")); // нужен для удобного обращения к параметрам и аргументам в функции
            STRING_PUSH(variables, strdup("__PUSH_RBP__")); // нужен для удобного обращения к параметрам и аргументам в функции
            // StackPrint(variables);
            func_init->middle   = variables->size;
            // aa("middle_params = [%d]\n", func_init->middle);
            func_init->end_vars = variables->size;

            MakeAsmNode(node->right, variables, functions, func_init);            
            // StackPrint(variables);
            // aa("end_params = [%d]\n", func_init->end_vars);
            
            $("pop rbp\n");
            $("ret\n");
            break;
        }
        case FUNC: 
        {
            int index_func = node->value.index_var;
            Function_t* func_init = &(functions->data[index_func]);

            PushFuncArgs(node->left, variables, func_init);

           $("call %s\n", func_init->name);
           $("add rsp, %d * 8\n", func_init->middle - func_init->begin_params - 2); // -2 _CALL_ADR_ _PUSH_RBP_
            break;
        }

        case OP:
            MakeAsmOper(node, variables, functions, func);
            break;

        case VAR_INIT:
            func->end_vars += 1;
            STRING_PUSH(variables, node->value.var);
            [[fallthrough]]
        case VAR:
        {
            int relative_index = get_index_var(node, variables, func) - func->middle;
            $("mov rax, [rbp + (%d) * 8]\n", relative_index);
            break;
        }

        case NUM:
            $("mov rax, %d\n", node->value.num);
            break;
    }
}

void ParsingParams(CompNode_t* node, StackString_t* variables, Function_t* func)
{
    if (node == NULL) return ;

    switch (node->type)
    {
        case VAR_INIT:
            STRING_PUSH(variables, node->value.var);
            break;

        case OP:
            if (node->value.oper == COMMA)
            {
                    if (node->left ) ParsingParams(node->left,  variables, func);
                    if (node->right) ParsingParams(node->right, variables, func);
            }

            else 
            {
//              сделать обработку ошибок    
                ERROR("Syntax error params in func [%s] unknown oper [%s]\n", func->name, arr_operators[node->value.oper].name)
                return ; 
            }

            break;

        default:
            // ERROR("")
            return ; 
    }
    
    return ;
}

void PushFuncArgs(CompNode_t* node, StackString_t* variables, Function_t* func)
{
    if (!node) return ;

    switch (node->type)
    {
        case NUM:
            $("push %d\n", node->value.num);
            break;

        case VAR:
        {
            int index = get_index_var(node, variables, func);
            int relative_index_var = index - func->middle;
            $("mov rax, [rbp + (%d) * 8]\n", relative_index_var); // хранить в таблице имен с переменными их размеры
            $("push rax\n");
            break;
        }

        default:
            if (!node_is_op(node, COMMA)) return; // обработка ошибок

            PushFuncArgs(node->right, variables, func); // удостовериться, что в обратном пояркде push идёт  
            PushFuncArgs(node->left , variables, func);
            break;
    }
}

int node_is_op(CompNode_t* node, Operator_val_t val)
{
    if (node == NULL) return NO;
    
    if (node->type != OP) return NO;

    if (node->value.oper != val) return NO;

    return YES;
}

void MakeAsmOper(CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func)
{
    if (!node) return ;

    switch (node->value.oper)
    {
        case SEP:   
            MakeAsmNode(node->left , variables, functions, func);
            MakeAsmNode(node->right, variables, functions, func);
            break;

        case EQ:
        {
            if (node->left->type == VAR_INIT) STRING_PUSH(variables, node->left->value.var); func->end_vars++;

            int relative_index = get_index_var(node->left, variables, func) - func->middle; 
            MakeAsmNode(node->right, variables, functions, func);
            $("mov [rbp + (%d) * 8], rax\n", relative_index);
            break;
//  Дописать!!!!==========================================================================
        }
        
        case ADD:
        {
            MakeAsmNode(node->right, variables, functions, func);
            $("mov rbx, rax\n");
            MakeAsmNode(node->left, variables, functions, func);
            $("add rax, rbx\n");
        }

        case RETURN:
            MakeAsmNode(node->left, variables, functions, func);
            break;

// проверка на то, что там переменная, число или выражение  
        default:
            break;
    }
}

int get_index_var(CompNode_t* node, StackString_t* variables, Function_t* func)
{
    if (!node) return -1;

    int var_size = variables->size;
    char* name_var = node->value.var;
    // aa("=========================================\n");
    // printf("name_var: [%s]\n\n", name_var);
    // aa("begin_params = [%d]\n", func->begin_params);
    // aa("middle       = [%d]\n", func->middle);
    // aa("end_vars     = [%d]\n", func->end_vars);

    for (int index = func->begin_params; index < func->end_vars; index++)
    {
        // aa("var_data = [%s]\n", variables->data[index]);
        if (strcmp(name_var, variables->data[index]) == 0) return index;
    }

    // printf("Я ничего не нашел\n\n");

    // aa("=========================================\n");
    return -1;
}

