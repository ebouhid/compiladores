#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "codigo_intermediario.h"

static int temp_count = 0;
static int label_count = 0;

char *novo_temp() {
    char buffer_temp[MAXLEXEME];
    snprintf(buffer_temp, MAXLEXEME, "_t%d", temp_count++);
    char *nome_alocado = strdup(buffer_temp);
    if (nome_alocado == NULL) {
        fprintf(stderr, "Erro GCI: Falha ao alocar memória para novo_temp.\n");
    }
    return nome_alocado;
}

char *novo_label() {
    char buffer_temp[MAXLEXEME];
    snprintf(buffer_temp, MAXLEXEME, "_L%d", label_count++);
    char *nome_alocado = strdup(buffer_temp);
    if (nome_alocado == NULL) {
        fprintf(stderr, "Erro GCI: Falha ao alocar memória para novo_label.\n");
    }
    return nome_alocado;
}

const char *operacao_para_string(OperacaoTac op) {
    switch (op) {
        case OP_ADD: return "ADD";
        case OP_SUB: return "SUB";
        case OP_MUL: return "MUL";
        case OP_DIV: return "DIV";
        case OP_MOD: return "MOD";
        case OP_LT: return "LT";
        case OP_LE: return "LE";
        case OP_GT: return "GT";
        case OP_GE: return "GE";
        case OP_EQ: return "EQ";
        case OP_NE: return "NE";
        case OP_ASSIGN: return "ASSIGN";
        case OP_LOAD: return "LOAD";
        case OP_STORE: return "STORE";
        case OP_GOTO: return "GOTO";
        case OP_IFF: return "IFF";
        case OP_FUN: return "FUN";
        case OP_END: return "END";
        case OP_FORMAL_PARAM: return "FORMAL_PARAM";
        case OP_DECL_VAR: return "DECL_VAR";
        case OP_DECL_ARR: return "DECL_ARR";
        case OP_ARG: return "ARG";
        case OP_CALL: return "CALL";
        case OP_RET: return "RET";
        case OP_LAB: return "LAB";
        case OP_HALT: return "HALT";
        default: return "UNKNOWN_OP";
    }
}

Tac *criarTac() {
    Tac *estrutura_tac = malloc(sizeof(Tac));
    if (!estrutura_tac) {
        fprintf(stderr, "Erro Fatal: Falha ao alocar memória para Tac.\n");
        exit(EXIT_FAILURE);
    }
    estrutura_tac->qtdNos = 0;
    estrutura_tac->fim = NULL;
    estrutura_tac->inicio = NULL;
    return estrutura_tac;
}

Tac *criarNoTac(Tac *estrutura_tac, OperacaoTac operacao, const char *op1, const char *op2, const char *resultado) {
    if (!estrutura_tac) {
         fprintf(stderr, "Erro: estrutura_tac nula em criarNoTac.\n");
         return NULL;
    }
    TacNo *novoNo = malloc(sizeof(TacNo));
    if (!novoNo) {
        fprintf(stderr, "Erro Fatal: Falha ao alocar memória para TacNo.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->operacao = operacao;
    snprintf(novoNo->op1, MAXLEXEME, "%s", op1 ? op1 : "");
    snprintf(novoNo->op2, MAXLEXEME, "%s", op2 ? op2 : "");
    snprintf(novoNo->resultado, MAXLEXEME, "%s", resultado ? resultado : "");
    novoNo->proximo = NULL;

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
    TacNo *proximo;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    free(estrutura_tac);
    return NULL;
}

void imprimirTac(FILE *outfile, Tac *tac) {
    if (outfile == NULL) {
        fprintf(stderr, ";; Arquivo de saída nulo para imprimirTac.\n");
        return;
    }
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
        char tabulation[5];
        if (contador < 10) {
            snprintf(tabulation, sizeof(tabulation), "\t\t");
        }
        else {
            snprintf(tabulation, sizeof(tabulation), "\t");
        }
        fprintf(outfile, "(%d)%s(%s, %s, %s, %s)\n",
                contador++,
                tabulation,
                operacao_para_string(percorre->operacao),
                percorre->op1,
                percorre->op2,
                percorre->resultado);
        percorre = percorre->proximo;
    }
    fprintf(outfile, ";; Fim do Código Intermediário\n");
}

char *gerar_codigo_no(No *no, Tac *tac_list, HashTable *symbol_table) {
    if (no == NULL) return NULL;
    if (tac_list == NULL) {
        fprintf(stderr, "Erro GCI: Lista TAC nula passada para gerar_codigo_no.\n");
        return NULL;
    }

    char *res = NULL, *op1_loc = NULL, *op2_loc = NULL, *op3_loc = NULL;
    char *label1 = NULL, *label2 = NULL;

    switch (no->kind_node) {
        case declaration_k:
            switch (no->kind_union.decl) {
                case fun_k:
                    if (no->filho[0] == NULL || no->filho[0]->lexmema == NULL) {
                        fprintf(stderr, "Erro GCI: Declaração de função sem nome ou ID.\n");
                        return NULL;
                    }
                    temp_count = 0;
                    criarNoTac(tac_list, OP_FUN, no->filho[0]->lexmema, no->lexmema, NULL);


                    if (no->filho[0]->filho[0] != NULL) {
                        No *paramNode = no->filho[0]->filho[0];
                        while (paramNode != NULL) {
                            if (paramNode->lexmema != NULL && strcmp(paramNode->lexmema, "void") != 0) {
                                if (paramNode->filho[0] != NULL && paramNode->filho[0]->lexmema != NULL) {
                                    criarNoTac(tac_list, OP_FORMAL_PARAM, paramNode->filho[0]->lexmema, NULL, NULL);
                                } else {
                                     fprintf(stderr, "Erro GCI: Parâmetro formal sem nome de ID para tipo %s.\n", paramNode->lexmema);
                                }
                            }
                            paramNode = paramNode->irmao;
                        }
                    }

                    if (no->filho[0]->filho[1] != NULL) {
                        char *body_res = gerar_codigo_no(no->filho[0]->filho[1], tac_list, symbol_table);
                        free(body_res);
                    }
                    criarNoTac(tac_list, OP_END, no->filho[0]->lexmema, NULL, NULL);
                    break;

                case var_k:
                    if (no->filho[0] != NULL && no->filho[0]->lexmema != NULL) {
                        criarNoTac(tac_list, OP_DECL_VAR, no->filho[0]->lexmema, NULL, NULL);
                    } else {
                        fprintf(stderr, "Erro GCI: Declaração de variável local sem ID.\n");
                    }
                    break;

                case arr_k:
                    if (no->filho[0] == NULL || no->filho[0]->lexmema == NULL) {
                        fprintf(stderr, "Erro GCI: Declaração de array local sem ID.\n");
                        break;
                    }
                    if (no->filho[0]->filho[0] != NULL && no->filho[0]->filho[0]->lexmema != NULL) {
                        criarNoTac(tac_list, OP_DECL_ARR, no->filho[0]->lexmema, no->filho[0]->filho[0]->lexmema, NULL);
                    } else {
                        fprintf(stderr, "Erro GCI: Array local '%s' AST não esperada para tamanho.\n", no->filho[0]->lexmema);
                    }
                    break;
                default:
                    fprintf(stderr, "Erro GCI: Kind de declaração desconhecido: %d\n", no->kind_union.decl);
                    break;
            }
            break;

        case statement_k:
            switch (no->kind_union.stmt) {
                case if_k:
                    if (no->filho[0] == NULL) {
                        fprintf(stderr, "Erro GCI: Condição do IF ausente.\n"); return NULL;
                    }
                    op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table);
                    if (op1_loc == NULL) return NULL;

                    label1 = novo_label();
                    if (label1 == NULL) { free(op1_loc); return NULL; }
                    criarNoTac(tac_list, OP_IFF, op1_loc, NULL, label1);
                    free(op1_loc); op1_loc = NULL;

                    if (no->filho[1] != NULL) {
                        char *then_res = gerar_codigo_no(no->filho[1], tac_list, symbol_table);
                        free(then_res);
                    }

                    if (no->filho[2] != NULL) {
                        label2 = novo_label();
                        if (label2 == NULL) { free(label1); return NULL; }
                        criarNoTac(tac_list, OP_GOTO, NULL, NULL, label2);
                        criarNoTac(tac_list, OP_LAB, NULL, NULL, label1);

                        char *else_res = gerar_codigo_no(no->filho[2], tac_list, symbol_table);
                        free(else_res);

                        criarNoTac(tac_list, OP_LAB, NULL, NULL, label2);
                        free(label2); label2 = NULL;
                    } else {
                        criarNoTac(tac_list, OP_LAB, NULL, NULL, label1);
                    }
                    free(label1); label1 = NULL;
                    break;

                case while_k:
                    if (no->filho[0] == NULL || no->filho[1] == NULL) {
                         fprintf(stderr, "Erro GCI: Nó WHILE incompleto.\n"); return NULL;
                    }
                    label1 = novo_label();
                    label2 = novo_label();
                    if (!label1 || !label2) { free(label1); free(label2); return NULL; }

                    criarNoTac(tac_list, OP_LAB, NULL, NULL, label1);
                    op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table);
                    if (op1_loc == NULL) { free(label1); free(label2); return NULL; }

                    criarNoTac(tac_list, OP_IFF, op1_loc, NULL, label2);
                    free(op1_loc); op1_loc = NULL;

                    char* while_body_res = gerar_codigo_no(no->filho[1], tac_list, symbol_table);
                    free(while_body_res);

                    criarNoTac(tac_list, OP_GOTO, NULL, NULL, label1);
                    criarNoTac(tac_list, OP_LAB, NULL, NULL, label2);
                    free(label1); label1 = NULL;
                    free(label2); label2 = NULL;
                    break;

                case return_k:
                    if (no->filho[0] != NULL) {
                        op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table);
                        if (op1_loc == NULL) return NULL;
                        if (no->filho[0]->kind_node == expression_k && no->filho[0]->kind_union.expr == id_k) {
                            char* temp_ret_val = novo_temp();
                            if(!temp_ret_val) { free(op1_loc); return NULL; }
                            criarNoTac(tac_list, OP_LOAD, op1_loc, NULL, temp_ret_val);
                            free(op1_loc);
                            op1_loc = temp_ret_val;
                        }
                        criarNoTac(tac_list, OP_RET, op1_loc, NULL, NULL);
                        free(op1_loc); op1_loc = NULL;
                    } else {
                        criarNoTac(tac_list, OP_RET, NULL, NULL, NULL);
                    }
                    break;
                default:
                    fprintf(stderr, "Erro GCI: Kind de statement desconhecido: %d\n", no->kind_union.stmt);
                    break;
            }
            break;

        case expression_k:
            switch (no->kind_union.expr) {
                case op_k:
                    if (!no->filho[0] || !no->filho[1] || !no->lexmema) {
                        fprintf(stderr, "Erro GCI: Nó de operação binária incompleto (lexema: %s).\n", no->lexmema ? no->lexmema : "N/A");
                        return NULL;
                    }

                    op1_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table);
                    if (!op1_loc) return NULL;
                    if (no->filho[0]->kind_node == expression_k && no->filho[0]->kind_union.expr == id_k) {
                        char *temp = novo_temp();
                        if (!temp) { free(op1_loc); return NULL; }
                        criarNoTac(tac_list, OP_LOAD, op1_loc, NULL, temp);
                        free(op1_loc);
                        op1_loc = temp;
                    }

                    op2_loc = gerar_codigo_no(no->filho[1], tac_list, symbol_table);
                    if (!op2_loc) { free(op1_loc); return NULL; }
                    if (no->filho[1]->kind_node == expression_k && no->filho[1]->kind_union.expr == id_k) {
                        char *temp = novo_temp();
                        if (!temp) { free(op1_loc); free(op2_loc); return NULL; }
                        criarNoTac(tac_list, OP_LOAD, op2_loc, NULL, temp);
                        free(op2_loc);
                        op2_loc = temp;
                    }

                    res = novo_temp();
                    if (!res) { free(op1_loc); free(op2_loc); return NULL; }

                    OperacaoTac op;
                    if (strcmp(no->lexmema, "+") == 0) op = OP_ADD;
                    else if (strcmp(no->lexmema, "-") == 0) op = OP_SUB;
                    else if (strcmp(no->lexmema, "*") == 0) op = OP_MUL;
                    else if (strcmp(no->lexmema, "/") == 0) op = OP_DIV;
                    else if (strcmp(no->lexmema, "%%") == 0) op = OP_MOD;
                    else if (strcmp(no->lexmema, "<") == 0) op = OP_LT;
                    else if (strcmp(no->lexmema, "<=") == 0) op = OP_LE;
                    else if (strcmp(no->lexmema, ">") == 0) op = OP_GT;
                    else if (strcmp(no->lexmema, ">=") == 0) op = OP_GE;
                    else if (strcmp(no->lexmema, "==") == 0) op = OP_EQ;
                    else if (strcmp(no->lexmema, "!=") == 0) op = OP_NE;
                    else {
                        fprintf(stderr, "Erro GCI: Operador binário desconhecido: %s\n", no->lexmema);
                        free(op1_loc); free(op2_loc); free(res);
                        return NULL;
                    }
                    criarNoTac(tac_list, op, op1_loc, op2_loc, res);
                    free(op1_loc); op1_loc = NULL;
                    free(op2_loc); op2_loc = NULL;
                    break;

                case constant_k:
                    if (no->lexmema == NULL) {
                        fprintf(stderr, "Erro GCI: Constante sem lexema.\n"); return NULL;
                    }
                    res = strdup(no->lexmema);
                    if (!res) fprintf(stderr, "Erro GCI: Falha ao alocar para constante.\n");
                    break;

                case id_k:
                    if (no->lexmema == NULL) {
                        fprintf(stderr, "Erro GCI: ID sem lexema.\n"); return NULL;
                    }
                    res = strdup(no->lexmema);
                    if (!res) fprintf(stderr, "Erro GCI: Falha ao alocar para ID.\n");
                    break;

                case assign_k:
                    if (!no->filho[0] || !no->filho[1]) {
                        fprintf(stderr, "Erro GCI: Atribuição incompleta.\n");
                        return NULL;
                    }

                    op2_loc = gerar_codigo_no(no->filho[1], tac_list, symbol_table);
                    if (!op2_loc) return NULL;

                    if (no->filho[1]->kind_node == expression_k && no->filho[1]->kind_union.expr == id_k) {
                        char *temp_rhs_val = novo_temp();
                        if (!temp_rhs_val) { free(op2_loc); return NULL; }
                        criarNoTac(tac_list, OP_LOAD, op2_loc, NULL, temp_rhs_val);
                        free(op2_loc);
                        op2_loc = temp_rhs_val;
                    }
                    
                    char *assigned_val_temp = novo_temp();
                    if (!assigned_val_temp) {
                        free(op2_loc);
                        return NULL;
                    }
                    criarNoTac(tac_list, OP_ASSIGN, op2_loc, NULL, assigned_val_temp);
                    if (no->filho[1]->kind_node == expression_k &&
                        (no->filho[1]->kind_union.expr == id_k || no->filho[1]->kind_union.expr == op_k ||
                         no->filho[1]->kind_union.expr == arr_k || no->filho[1]->kind_union.expr == ativ_k)) {
                        free(op2_loc);
                    } else {
                        free(op2_loc);
                    }
                    op2_loc = NULL;


                    No *lvalue = no->filho[0];
                    if (lvalue->kind_node == expression_k && lvalue->kind_union.expr == id_k) {
                        criarNoTac(tac_list, OP_STORE, assigned_val_temp, NULL, lvalue->lexmema);
                        res = assigned_val_temp;
                    } else if (lvalue->kind_node == expression_k && lvalue->kind_union.expr == arr_k) {
                        if (lvalue->lexmema == NULL || lvalue->filho[0] == NULL) {
                            fprintf(stderr, "Erro GCI: Atribuição a array com ID ou índice ausente.\n");
                            free(assigned_val_temp);
                            return NULL;
                        }
                        char *array_base_name = lvalue->lexmema;
                        op3_loc = gerar_codigo_no(lvalue->filho[0], tac_list, symbol_table);
                        if (!op3_loc) {
                            free(assigned_val_temp);
                            return NULL;
                        }

                        if (lvalue->filho[0]->kind_node == expression_k && lvalue->filho[0]->kind_union.expr == id_k) {
                            char *temp_idx_val = novo_temp();
                            if (!temp_idx_val) {
                                free(assigned_val_temp);
                                free(op3_loc);
                                return NULL;
                            }
                            criarNoTac(tac_list, OP_LOAD, op3_loc, NULL, temp_idx_val);
                            free(op3_loc); 
                            op3_loc = temp_idx_val; 
                        }
                        criarNoTac(tac_list, OP_STORE, assigned_val_temp, op3_loc, array_base_name);
                        free(op3_loc); op3_loc = NULL;
                        res = assigned_val_temp;
                    } else {
                        fprintf(stderr, "Erro GCI: LValue inválido para atribuição.\n");
                        free(assigned_val_temp);
                        return NULL;
                    }
                    break;
                case arr_k:
                    if (no->lexmema == NULL || no->filho[0] == NULL) {
                        fprintf(stderr, "Erro GCI: Acesso a array com ID ou índice ausente.\n"); return NULL;
                    }
                    char *arr_base_load = no->lexmema;
                    op2_loc = gerar_codigo_no(no->filho[0], tac_list, symbol_table);
                    if (!op2_loc) return NULL;

                    if (no->filho[0]->kind_node == expression_k && no->filho[0]->kind_union.expr == id_k) {
                        char *temp_idx_val = novo_temp();
                        if (!temp_idx_val) { free(op2_loc); return NULL; }
                        criarNoTac(tac_list, OP_LOAD, op2_loc, NULL, temp_idx_val);
                        free(op2_loc); 
                        op2_loc = temp_idx_val; 
                    }
                    res = novo_temp();
                    if (!res) { free(op2_loc); return NULL; }

                    criarNoTac(tac_list, OP_LOAD, arr_base_load, op2_loc, res);
                    free(op2_loc); op2_loc = NULL;
                    break;

                case ativ_k:
                    if (no->lexmema == NULL) {
                        fprintf(stderr, "Erro GCI: Chamada de função sem nome de ID.\n"); return NULL;
                    }
                    No *argNode = no->filho[0];
                    int arg_count = 0;
                    char *arg_values[MAX_ARGS_TEMP];
                    for(int i=0; i < MAX_ARGS_TEMP; ++i) arg_values[i] = NULL;

                    while (argNode != NULL && arg_count < MAX_ARGS_TEMP) {
                        arg_values[arg_count] = gerar_codigo_no(argNode, tac_list, symbol_table);
                        if (arg_values[arg_count] == NULL) {
                            for(int k=0; k < arg_count; ++k) free(arg_values[k]);
                            fprintf(stderr, "Erro GCI: Erro ao gerar argumento %d para %s.\n", arg_count+1, no->lexmema);
                            return NULL;
                        }

                        if (argNode->kind_node == expression_k && argNode->kind_union.expr == id_k) {
                            char *temp_arg_val = novo_temp();
                            if (!temp_arg_val) {
                                for(int k=0; k <= arg_count; ++k) free(arg_values[k]);
                                return NULL;
                            }
                            criarNoTac(tac_list, OP_LOAD, arg_values[arg_count], NULL, temp_arg_val);
                            free(arg_values[arg_count]);
                            arg_values[arg_count] = temp_arg_val;
                        }
                        arg_count++;
                        argNode = argNode->irmao;
                    }
                    if (argNode != NULL && arg_count == MAX_ARGS_TEMP) {
                        fprintf(stderr, "Aviso GCI: Número de argumentos para %s excedeu o limite de %d.\n", no->lexmema, MAX_ARGS_TEMP);
                    }

                    for (int i = 0; i < arg_count; i++) {
                        criarNoTac(tac_list, OP_ARG, arg_values[i], NULL, NULL);
                        free(arg_values[i]);
                    }

                    res = novo_temp();
                    if (!res) return NULL;

                    char arg_count_str[10];
                    snprintf(arg_count_str, sizeof(arg_count_str), "%d", arg_count);
                    criarNoTac(tac_list, OP_CALL, no->lexmema, arg_count_str, res);
                    break;

                case type_k:
                    break;

                default:
                    fprintf(stderr, "Erro GCI: Kind de expressão desconhecido: %d (lexema: %s)\n",
                            no->kind_union.expr, no->lexmema ? no->lexmema : "N/A");
                    break;
            }
            break;

        default:
            fprintf(stderr, "Erro GCI: Kind de nó desconhecido: %d\n", no->kind_node);
            break;
    }

    if (no && no->irmao) {
        char *irmao_res = gerar_codigo_no(no->irmao, tac_list, symbol_table);
        free(irmao_res);
    }
    return res;
}

Tac *gerar_codigo_intermediario(No *raizArvore, HashTable *symbol_table) {
    if (raizArvore == NULL) {
        fprintf(stderr, "Erro GCI: Árvore sintática raiz é nula!\n");
        return NULL;
    }
    Tac *codigo_final = criarTac();
    if (codigo_final == NULL) return NULL;

    temp_count = 0;
    label_count = 0;

    char *res_geral = gerar_codigo_no(raizArvore, codigo_final, symbol_table);
    free(res_geral);

    if (codigo_final->qtdNos > 0) {
        if (codigo_final->fim &&
            codigo_final->fim->operacao != OP_HALT &&
            codigo_final->fim->operacao != OP_RET &&
            codigo_final->fim->operacao != OP_GOTO) {
             criarNoTac(codigo_final, OP_HALT, NULL, NULL, NULL);
        } else if (codigo_final->fim == NULL) {
             criarNoTac(codigo_final, OP_HALT, NULL, NULL, NULL);
        }
    } else {
        criarNoTac(codigo_final, OP_HALT, NULL, NULL, NULL);
    }

    return codigo_final;
}
