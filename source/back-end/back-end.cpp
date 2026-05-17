#include "../../headers/back-end/back-end.h"

#define aa(...) printf(__VA_ARGS__)
#define $(...)  fprintf(file_asm, __VA_ARGS__)
#define ERROR(...) fprintf(stderr, BOLD_RED  __VA_ARGS__); fprintf(stderr, RESET);

int node_is_op(CompNode_t* node, Operator_val_t val);
int get_index_var(CompNode_t* node, StackString_t* variables, Function_t* func);
Flag_t node_is_logical(CompNode_t* node);
// брать название из ключей компилляции
FILE* file_asm = fopen("./compile_files/file_asm.s", "w");

// ============================================== Rules =================================================
// В относительной адресации func->middle эквивалентен rbp  
// Все результаты выражений и возвращаемые значения функций лежат в rax 
// Для ассемблирования части кода важно из какой функции она была вызвана для регулирования областей видимости
// main обрабатывается точно также только метка будет _start
// сохранять в стеке результаты промежуточных вычислений, потому чтобы регистры могут сломаться
// ------------------------------------------------------------------------------------------------------
// Возможно у if будет счетчик, который увеличивается, когда есть if, и уменьшается, когда тело if заканчивается
// Во-первых при обращении к параметрам в стеке надо + поменять на -, во вторых из-за stdcall надо менять арифметику расчета адреса. Наверное проще всего переменные тоже пушить в обратном поярдке в стек, если это параметры
// ======================================================================================================
int count_label = 0;
int count_func  = 0;

void MakeAsmCode(CompNode_t* root, StackString_t* variables, StackFunc_t* functions)
{
    PrintStdLib();    
    $("section .text\n"); 
    $("global _start\n"); 
    // в первой версии можно просто печатать в файл print.s 
    MakeAsmNode(root,variables, functions, NULL);
    $("mov rax, 0x3c\n");
    $("xor rdx, rdx\n");
    $("syscall\n");
}

void MakeAsmNode(CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func)
{
    if (!node) return ;

    switch (node->type)
    {
        case FUNC_INIT:
        {
            count_func++;
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
            func_init->middle   = variables->size - 1;
            // aa("middle_params = [%d]\n", func_init->middle);
            int count_var = GetCountVariables(node->right, 0);
            func_init->end_vars = func_init->middle + count_var;
            $("sub rsp, %d * 8\n", count_var);

            MakeAsmNode(node->right, variables, functions, func_init);            
            // StackPrint(variables);
            $("END_FUNC%d:\n", count_func);
            $("add rsp, %d * 8\n", count_var);
            // aa("end_params = [%d]\n", func_init->end_vars);
            
            $("pop rbp\n");
            if (strcmp(func_init->name, "_start") != 0) {$("ret\n");}
            break;
        }
        case FUNC: 
        {
            int index_func = node->value.index_var;
            Function_t* func_init = &(functions->data[index_func]);

            PushFuncArgs(node->left, variables, func); //11111

           $("call %s\n", func_init->name);
           $("add rsp, %d * 8\n", func_init->middle - func_init->begin_params - 2 + 1); // -2 _CALL_ADR_ _PUSH_RBP_
            break;
        }

        case PROTO:
            break;

        case OP:
            MakeAsmOper(node, variables, functions, func);
            break;

        case VAR_INIT:
            // func->end_vars += 1;
            // STRING_PUSH(variables, node->value.var);
            [[fallthrough]]
        case VAR:
        {
            int relative_index = get_index_var(node, variables, func) - func->middle;
            $("mov rax, [rbp - (%d) * 8]\n", relative_index);
            break;
        }

        case NUM:
            $("mov rax, %d\n", node->value.num);
            break;

        default:
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
                    if (node->right) ParsingParams(node->right, variables, func);
                    if (node->left ) ParsingParams(node->left,  variables, func);
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
            // PRINT_ERR("func_name = %s\n", func->name);
            $("push %d\n", node->value.num);
            break;

        case VAR:
        {
            int index = get_index_var(node, variables, func);
            int relative_index_var = index - func->middle;
            $("mov rax, [rbp - (%d) * 8]\n", relative_index_var); // хранить в таблице имен с переменными их размеры
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
            $("mov [rbp - (%d) * 8], rax\n", relative_index);
            break;
//  Дописать!!!!==========================================================================
        }
        
        case ADD:
            MakeAsmNode(node->right, variables, functions, func);
            // $("mov rbx, rax\n");
            $("push rax\n");
            MakeAsmNode(node->left, variables, functions, func);
            $("pop rbx\n");
            $("add rax, rbx\n");
            break;

        case SUB:
            // $("mov rbx, rax\n");
            MakeAsmNode(node->right, variables, functions, func);
            $("push rax\n");
            MakeAsmNode(node->left, variables, functions, func);
            $("pop rbx\n");
            $("sub rax, rbx\n");
            break;
       
        case MUL:
            MakeAsmNode(node->right, variables, functions, func);
            // $("mov rbx, rax\n");
            $("push rax\n");
            MakeAsmNode(node->left, variables, functions, func);
            $("pop rbx\n");
            $("imul rbx\n");
            $("and rdx, 1 << 63\n");
            $("or rax,  rdx\n");
            break;
        
        case DIV:
            $("xor rdx, rdx\n");
            MakeAsmNode(node->right, variables, functions, func);
            // $("mov rbx, rax\n");
            $("push rax\n");
            MakeAsmNode(node->left, variables, functions, func);
            $("pop rbx\n");
            $("cqo\n");
            $("idiv rbx\n");
            break;

        case RETURN:
            MakeAsmNode(node->left, variables, functions, func);
            $("jmp END_FUNC%d\n", count_func); // возможно могут быть одинаковые миддлы
            break;
   
        case SQRT:
            MakeAsmNode(node->left, variables, functions, func);
            $("cvtsi2ss xmm0, rax\n");
            $("sqrtss xmm0, xmm0\n");
            $("cvttss2si rax, xmm0\n");
            break;
            
        case IF:
            MakeAsmIf(node, variables, functions, func);
            break;

        case WHILE:
            MakeAsmWhile(node, variables, functions, func);
            break;

// проверка на то, что там переменная, число или выражение  
        default:
            break;
    }
}

void MakeAsmIf(CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func)
{
// cmp and jump_condition 
// должен быть массив соответствий jump_cond и условных операторов
// есть два варианта: сравнивать разность с нулем или между собой,
    MakeAsmCondition(node->left, variables, functions, func); // condition 
    int label_num = count_label;
    MakeAsmNode(node->right, variables, functions, func);
    $("LBLEND%d:\n\n", label_num);
}

void MakeAsmWhile(CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func)
{
    MakeAsmCondition(node->left, variables, functions, func);
    int label_num = count_label; 
    MakeAsmNode(node->right, variables, functions, func);
    $("jmp LBLCOND%d\n", label_num);
    $("LBLEND%d:\n\n", label_num);
}

Flag_t node_is_logical(CompNode_t* node)
{
    if (node->type != OP) return NO;
    if ((LOGIC_BEGIN <= node->value.oper) && (node->value.oper <= LOGIC_END)) return YES;

    return NO;
}

void MakeAsmCondition(CompNode_t* node, StackString_t* variables, StackFunc_t* functions, Function_t* func)
{
    count_label++;
    $("\n\nLBLCOND%d:\n", count_label);

    if (node_is_logical(node))
    {
        MakeAsmNode(node->left, variables, functions, func);
        $("push rax\n");
        MakeAsmNode(node->right, variables, functions, func);
        $("pop rbx\n");
        
        $("cmp rbx, rax\n");

        $("%s LBLEND%d\n", arr_conditions[node->value.oper - LOGIC_BEGIN].jump_name, count_label); // возможно придется поменять JUMP на противоположные
    }

    else 
    {
        MakeAsmNode(node, variables, functions, func);
    
        $("cmp rax, 0\n");
        $("je LBLEND%d\n", count_label);
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

int GetCountVariables(CompNode_t* node, int count_now)
{
    int count = 0;
    
    if (!node) return 0;

    if (node->type == VAR_INIT) count++;

    count += GetCountVariables(node->left,  count);
    count += GetCountVariables(node->right, count);
    
    return count;
}

#define MAKE_STD(funcname)          const char* funcname ## _std = #funcname".s";                          \
                                    char* funcname ## _code = CreateBuffer(funcname ## _std).buff + 1;     \
                                    $("%s\n\n", funcname ## _code);                                        \
                                    free(funcname ## _code - 1);

void PrintStdLib(void)
{
    MAKE_STD(print)
    MAKE_STD(scan)
    MAKE_STD(data)
    MAKE_STD(printchar)
}
