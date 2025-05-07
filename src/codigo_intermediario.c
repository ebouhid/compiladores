#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "codigo_intermediario.h" // Inclui o .h com as definições

// --- Variáveis Globais Estáticas (para temporários e rótulos) ---
static int temp_count = 0;
static int label_count = 0;

// --- Funções Auxiliares ---

// Gera um novo nome de temporário (_t0, _t1, ...)
// NOTA: Retorna ponteiro para buffer estático - NÃO é thread-safe
//       e chamadas subsequentes sobrescrevem o buffer. Use com cuidado.
//       Idealmente, alocaria memória dinamicamente (ex: com strdup).
const char* novo_temp() {
    static char buffer[MAXLEXEME];
    snprintf(buffer, MAXLEXEME, "_t%d", temp_count++);
    return buffer;
}

// Gera um novo nome de rótulo (_L0, _L1, ...)
// NOTA: Retorna ponteiro para buffer estático - mesmas ressalvas do novo_temp()
const char* novo_label() {
    static char buffer[MAXLEXEME];
    snprintf(buffer, MAXLEXEME, "_L%d", label_count++);
    return buffer;
}

// Converte enum OperacaoTac para string (para impressão)
const char* operacao_para_string(OperacaoTac op) {
    switch (op) {
        case OP_ADD: return "ADD"; case OP_SUB: return "SUB"; case OP_MUL: return "MUL";
        case OP_DIV: return "DIV"; case OP_MOD: return "MOD"; case OP_UMINUS: return "UMINUS";
        case OP_LT: return "LT"; case OP_LE: return "LE"; case OP_GT: return "GT";
        case OP_GE: return "GE"; case OP_EQ: return "EQ"; case OP_NE: return "NE";
        case OP_ASSIGN: return "ASSIGN"; case OP_LOAD: return "LOAD"; case OP_STORE: return "STORE";
        case OP_GOTO: return "GOTO"; case OP_IFF: return "IFF"; case OP_FUN: return "FUN";
        case OP_END: return "END"; case OP_PARAM: return "PARAM"; case OP_CALL: return "CALL";
        case OP_RET: return "RET"; case OP_LAB: return "LAB"; case OP_ARG: return "ARG";
        case OP_HALT: return "HALT";
        default: return "UNKNOWN_OP";
    }
}


// --- Funções da Lista TAC (ligeiramente modificadas) ---

Tac *criarTac() { // Não precisa de argumento
    Tac *estrutura_tac = malloc(sizeof(Tac));
    if (!estrutura_tac) {
        fprintf(stderr, "Erro: Falha ao alocar memória para Tac.\n");
        exit(1);
    }
    estrutura_tac->qtdNos = 0;
    estrutura_tac->fim = NULL;
    estrutura_tac->inicio = NULL;
    return estrutura_tac;
}

// Modificado para usar const char* e OperacaoTac
Tac *criarNoTac(Tac *estrutura_tac, OperacaoTac operacao, const char *op1, const char *op2, const char *resultado) {
    TacNo* novoNo = malloc(sizeof(TacNo));
     if (!novoNo) {
        fprintf(stderr, "Erro: Falha ao alocar memória para TacNo.\n");
        exit(1);
    }
    // Preencher informações
    novoNo->operacao = operacao;
    strncpy(novoNo->op1, op1 ? op1 : "", MAXLEXEME - 1); // Copia segura
    novoNo->op1[MAXLEXEME - 1] = '\0';
    strncpy(novoNo->op2, op2 ? op2 : "", MAXLEXEME - 1);
    novoNo->op2[MAXLEXEME - 1] = '\0';
    strncpy(novoNo->resultado, resultado ? resultado : "", MAXLEXEME - 1);
    novoNo->resultado[MAXLEXEME - 1] = '\0';
    novoNo->proximo = NULL;

    // Alocar
    if (estrutura_tac->fim == NULL) {
        estrutura_tac->inicio = novoNo;
        estrutura_tac->fim = novoNo;
    } else {
        estrutura_tac->fim->proximo = novoNo;
        estrutura_tac->fim = novoNo;
    }
    estrutura_tac->qtdNos++;
    return estrutura_tac;
}

Tac *liberarTac(Tac *estrutura_tac) {
    if (estrutura_tac == NULL) {
        return NULL;
    }
    TacNo *atual = estrutura_tac->inicio;
    TacNo *proximo = NULL;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    free(estrutura_tac);
    return NULL;
}

// Modificado para imprimir strings das operações e ir para FILE*
void imprimirTac(FILE* outfile, Tac *tac) {
    if (tac == NULL) {
        fprintf(outfile, ";; Estrutura TAC nula.\n");
        return;
    }
    if (tac->qtdNos == 0) {
        fprintf(outfile, ";; Estrutura TAC vazia.\n");
        return;
    }

    fprintf(outfile, ";; Código Intermediário Gerado (%d instruções)\n", tac->qtdNos);
    TacNo *percorre = tac->inicio;
    int contador = 0;
    while (percorre != NULL) {
        fprintf(outfile, "(%d)\t(%s, %s, %s, %s)\n",
                contador++,
                operacao_para_string(percorre->operacao),
                percorre->op1,
                percorre->op2,
                percorre->resultado);
        percorre = percorre->proximo;
    }
     fprintf(outfile, ";; Fim do Código Intermediário\n");
}


// --- Função Recursiva Principal de Geração ---
// Retorna o nome do registrador/variável/constante que contém o resultado da expressão,
// ou NULL para statements/declarações.
// NOTA: A responsabilidade de alocar/liberar a string retornada é complexa.
//       Usando `strdup` para simplificar, mas idealmente teria um gerenciamento melhor.
char* gerar_codigo_no(No *no, Tac *tac_list, HashTable *symbol_table) {
    if (no == NULL) {
        return NULL;
    }

    char *res = NULL, *op1_loc = NULL, *op2_loc = NULL, *op3_loc = NULL;
    char *temp_res = NULL;
    char *label1 = NULL, *label2 = NULL; // Para rótulos em if/while

    switch (no->kind_node) {
        case declaration_k:
            switch (no->kind_union.decl) {
                case fun_k:
                    // nó tipo_especificador (filho 0)
                    // nó f_id (filho 1)
                    //   - nó params (filho 0 do f_id) -> processar irmãos
                    //   - nó composto_decl (filho 1 do f_id) -> processar irmãos (local_decl, stmt_list)

                    temp_count = 0; // Reinicia contagem de temporários para cada função
                    criarNoTac(tac_list, OP_FUN, no->filho[1]->lexmema, NULL, NULL); // Marca início da função

                    // Processa parâmetros (filho 0 do nó da função ID)
                    No* paramNode = no->filho[1]->filho[0];
                    while(paramNode != NULL) {
                        if (strcmp(paramNode->lexmema, "void") != 0) {
                             // O nó do parâmetro é o tipo, o filho dele é o ID
                            criarNoTac(tac_list, OP_PARAM, paramNode->filho[0]->lexmema, NULL, NULL);
                        }
                        paramNode = paramNode->irmao;
                    }

                    // Processa corpo da função (filho 1 do nó da função ID)
                    gerar_codigo_no(no->filho[1]->filho[1], tac_list, symbol_table);

                    criarNoTac(tac_list, OP_END, no->filho[1]->lexmema, NULL, NULL); // Marca fim da função
                    break;
                case var_k:
                case arr_k:
                    // Declarações de variáveis globais/locais não geram código TAC diretamente aqui
                    // (alocação é implícita ou feita na geração final)
                    break;
                default:
                     fprintf(stderr, "Erro GCI: Kind de declaração desconhecido: %d\n", no->kind_union.decl);
                     break;
            }
            break; // Fim de declaration_k

        case statement_k:
            switch (no->kind_union.stmt) {
                case if_k:
                    // filho[0]: condição
                    // filho[1]: then-block
                    // filho[2]: else-block (se existir)

                    label1 = strdup(novo_label()); // Rótulo para o else/fim
                    op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table); // Gera código da condição

                    criarNoTac(tac_list, OP_IFF, op1_loc, NULL, label1); // Se condição for falsa, salta para label1

                    free(op1_loc); // Libera resultado da condição (se alocado)

                    gerar_codigo_no(no->filho[1], tac_list, symbol_table); // Gera código do then-block

                    if (no->filho[2] != NULL) { // Tem else
                        label2 = strdup(novo_label()); // Rótulo para o fim do if
                        criarNoTac(tac_list, OP_GOTO, NULL, NULL, label2); // Salta para o fim após o then
                        criarNoTac(tac_list, OP_LAB, NULL, NULL, label1); // Define o rótulo do else
                        gerar_codigo_no(no->filho[2], tac_list, symbol_table); // Gera código do else-block
                        criarNoTac(tac_list, OP_LAB, NULL, NULL, label2); // Define o rótulo do fim
                        free(label2);
                    } else { // Não tem else
                        criarNoTac(tac_list, OP_LAB, NULL, NULL, label1); // Define o rótulo do fim
                    }
                    free(label1);
                    break;

                case while_k:
                    // filho[0]: condição
                    // filho[1]: corpo do loop

                    label1 = strdup(novo_label()); // Rótulo início do loop (condição)
                    label2 = strdup(novo_label()); // Rótulo fim do loop

                    criarNoTac(tac_list, OP_LAB, NULL, NULL, label1); // Define o rótulo de início
                    op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table); // Gera código da condição
                    criarNoTac(tac_list, OP_IFF, op1_loc, NULL, label2); // Se falso, salta para o fim
                    free(op1_loc);

                    gerar_codigo_no(no->filho[1], tac_list, symbol_table); // Gera código do corpo

                    criarNoTac(tac_list, OP_GOTO, NULL, NULL, label1); // Volta para o início (condição)
                    criarNoTac(tac_list, OP_LAB, NULL, NULL, label2); // Define o rótulo do fim
                    free(label1);
                    free(label2);
                    break;

                case return_k:
                    if (no->filho[0] != NULL) { // Tem expressão de retorno
                        op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table);
                        criarNoTac(tac_list, OP_RET, op1_loc, NULL, NULL);
                        free(op1_loc);
                    } else { // return; (void)
                        criarNoTac(tac_list, OP_RET, NULL, NULL, NULL);
                    }
                    break;

                // case expression_statement_k: // Não existe na sua enum, mas é o caso 'expressao_decl'
                //     op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table); // Gera código da expressão
                //     free(op1_loc); // Descarta o resultado
                //     break;

                default:
                    fprintf(stderr, "Erro GCI: Kind de statement desconhecido: %d\n", no->kind_union.stmt);
                    break;
            }
            // Statements não retornam um 'lugar' de resultado
            break; // Fim de statement_k

        case expression_k:
            switch (no->kind_union.expr) {
                case op_k:
                    // filho[0]: operando esquerdo
                    // filho[1]: operando direito
                    op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table);
                    op2_loc = gerar_codigo_no(no->filho[1], tac_list, symbol_table);
                    temp_res = strdup(novo_temp());

                    OperacaoTac op;
                    if (strcmp(no->lexmema, "+") == 0) op = OP_ADD;
                    else if (strcmp(no->lexmema, "-") == 0) op = OP_SUB;
                    else if (strcmp(no->lexmema, "*") == 0) op = OP_MUL;
                    else if (strcmp(no->lexmema, "/") == 0) op = OP_DIV;
                    else if (strcmp(no->lexmema, "%") == 0) op = OP_MOD;
                    else if (strcmp(no->lexmema, "<") == 0) op = OP_LT;
                    else if (strcmp(no->lexmema, "<=") == 0) op = OP_LE;
                    else if (strcmp(no->lexmema, ">") == 0) op = OP_GT;
                    else if (strcmp(no->lexmema, ">=") == 0) op = OP_GE;
                    else if (strcmp(no->lexmema, "==") == 0) op = OP_EQ;
                    else if (strcmp(no->lexmema, "!=") == 0) op = OP_NE;
                    else {
                         fprintf(stderr, "Erro GCI: Operador binário desconhecido: %s\n", no->lexmema);
                         op = OP_ADD; // Evita erro de compilação, mas está errado
                    }
                    criarNoTac(tac_list, op, op1_loc, op2_loc, temp_res);
                    free(op1_loc);
                    free(op2_loc);
                    res = temp_res; // Retorna o temporário onde o resultado foi armazenado
                    break;

                case constant_k:
                    res = strdup(no->lexmema); // Retorna a própria constante como string
                    break;

                case id_k:
                    res = strdup(no->lexmema); // Retorna o nome do ID
                    break;

                 case assign_k: // Tratado na regra 'expressao' do Bison
                    // filho[0]: LValue (var ou var[expr])
                    // filho[1]: RValue (expressao)
                    op2_loc = gerar_codigo_no(no->filho[1], tac_list, symbol_table); // Gera código do lado direito

                    if (no->filho[0]->kind_union.expr == id_k) { // Atribuição simples: id = expr
                        op1_loc = strdup(no->filho[0]->lexmema); // Nome da variável destino
                        criarNoTac(tac_list, OP_ASSIGN, op2_loc, NULL, op1_loc);
                    }
                    else if (no->filho[0]->kind_union.expr == arr_k) { // Atribuição em array: id[expr1] = expr2
                        // filho[0] do nó 'assign' é o nó 'arr_k' (ID do array)
                        // filho[0] do nó 'arr_k' é a expressão do índice
                        op1_loc = strdup(no->filho[0]->lexmema); // Nome do array base
                        op3_loc = gerar_codigo_no(no->filho[0]->filho[0], tac_list, symbol_table); // Gera código do índice
                        // Gera: STORE valor(op2_loc), base(op1_loc), indice(op3_loc)
                        // O campo 'resultado' do STORE é usado para o nome do array base
                        criarNoTac(tac_list, OP_STORE, op2_loc, op3_loc, op1_loc);
                        free(op3_loc);
                    } else {
                         fprintf(stderr, "Erro GCI: LValue inválido para atribuição.\n");
                    }

                    // A expressão de atribuição retorna o valor atribuído
                    // No nosso TAC simples, podemos retornar o op2_loc (o valor que foi calculado/atribuído)
                    // ou o op1_loc (o destino). Vamos retornar o valor (op2_loc)
                    // já que o op1_loc foi liberado no caso de ID simples
                    if(op1_loc) free(op1_loc); // Libera se foi usado
                    res = op2_loc; // Retorna o valor que foi atribuído
                    // IMPORTANTE: O ponteiro op2_loc agora pertence a 'res', não liberar aqui.
                    break;

                case arr_k: // Acesso a array: id[expr]
                    // 'no' é o nó do ID do array
                    // filho[0] é a expressão do índice
                    op1_loc = strdup(no->lexmema); // Nome do array base
                    op2_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table); // Gera código do índice
                    temp_res = strdup(novo_temp());
                    // Gera: LOAD resultado(temp_res), base(op1_loc), indice(op2_loc)
                    criarNoTac(tac_list, OP_LOAD, op1_loc, op2_loc, temp_res);
                    free(op1_loc);
                    free(op2_loc);
                    res = temp_res; // Retorna o temporário com o valor carregado
                    break;

                case ativ_k: // Chamada de função: id(args)
                    // 'no' é o nó do ID da função
                    // filho[0] é o primeiro argumento (ou NULL se não houver args)

                    // 1. Processar argumentos (percorrer irmãos do filho[0])
                    No* argNode = no->filho[0];
                    int arg_count = 0;
                    // Precisamos gerar os argumentos em ordem reversa para muitas convenções,
                    // ou gerar e depois emitir os PARAM/ARG. Vamos gerar e emitir ARG.
                    // Para simplificar, vamos gerar na ordem e emitir ARG.
                    char* arg_locations[32]; // Limite arbitrário de argumentos
                    while (argNode != NULL) {
                         if(arg_count >= 32) {
                            fprintf(stderr, "Erro GCI: Número máximo de argumentos excedido.\n");
                            break; // Evita estourar o buffer
                         }
                        arg_locations[arg_count] = gerar_codigo_no(argNode, tac_list, symbol_table);
                        arg_count++;
                        argNode = argNode->irmao;
                    }
                    // Emitir ARG para cada argumento calculado
                    for(int i=0; i < arg_count; ++i) {
                         criarNoTac(tac_list, OP_ARG, arg_locations[i], NULL, NULL);
                         free(arg_locations[i]); // Libera o local do argumento
                    }

                    // 2. Gerar chamada
                    temp_res = strdup(novo_temp()); // Temporário para o valor de retorno
                    char arg_count_str[5];
                    snprintf(arg_count_str, 5, "%d", arg_count); // Passa a contagem como string no op2
                    criarNoTac(tac_list, OP_CALL, no->lexmema, arg_count_str, temp_res);
                    res = temp_res; // Retorna o temporário do resultado
                    break;

                case type_k: // Nó de tipo (ex: 'void' em params), não gera código
                    break;

                default:
                    fprintf(stderr, "Erro GCI: Kind de expressão desconhecido: %d (%s)\n", no->kind_union.expr, no->lexmema);
                    break;
            }
            break; // Fim de expression_k

        default:
             fprintf(stderr, "Erro GCI: Kind de nó desconhecido: %d\n", no->kind_node);
             break;
    }

    // Processa irmãos (listas de declarações, statements, parametros, argumentos)
    if (no && no->irmao) {
        // O resultado de uma lista geralmente não é usado diretamente,
        // então podemos descartar o que a chamada recursiva para o irmão retornar.
        char* irmao_res = gerar_codigo_no(no->irmao, tac_list, symbol_table);
        free(irmao_res); // Libera o resultado do irmão, se houver
    }

    return res; // Retorna o local do resultado da expressão (ou NULL)
}


// --- Função Principal de Geração ---
Tac *gerar_codigo_intermediario(No *raizArvore, HashTable *symbol_table) {
    if (raizArvore == NULL) {
        fprintf(stderr, "Erro GCI: Árvore sintática vazia!\n");
        return NULL;
    }

    Tac *codigo_final = criarTac();
    temp_count = 0; // Zera contadores globais
    label_count = 0;

    // Inicia a geração recursiva
    char* final_res = gerar_codigo_no(raizArvore, codigo_final, symbol_table);
    free(final_res); // Descarta resultado final da raiz (geralmente NULL)

    // Adiciona um HALT no final se a última instrução não for um RET ou GOTO incondicional
    // (Simplificação: sempre adiciona HALT por enquanto, pode ser otimizado depois)
     if(codigo_final->fim == NULL || (codigo_final->fim->operacao != OP_RET && codigo_final->fim->operacao != OP_GOTO && codigo_final->fim->operacao != OP_END)) {
         criarNoTac(codigo_final, OP_HALT, NULL, NULL, NULL);
     }


    return codigo_final;
}

// Remover a função main de teste que existia aqui antes
/*
int main(){
    // ... código de teste removido ...
    return 0;
}
*/