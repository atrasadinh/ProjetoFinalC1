#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define Pacientes_maximos 50
#define Agendamentos_maximos 100
#define Nomes_maximos 50

typedef struct {
    char CPF[12];
    char nome[Nomes_maximos];
    char data_nascimento[9];
    int idade;
} Paciente;

typedef struct {
    char CPF_paciente[12];
    char data_consulta[9];
    int hora_consulta;
    int final_consulta;
} Agendamento;

typedef struct {
    Paciente pacientes[Pacientes_maximos];
    Agendamento Agendamento[Agendamentos_maximos];
    int total_pacientes;
    int total_agendamentos;
} Sistema;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void Maisculo(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int validar_CPF(const char *CPF) {
    if (strlen(CPF) != 11) {
        return 0;
    }
    for (int i = 0; i < 11; i++) {
        if (!isdigit(CPF[i])) {
            return 0;
        }
    }
    int todos_iguais = 1;
    for (int i = 1; i < 11; i++) {
        if (CPF[i] != CPF[0]) {
            todos_iguais = 0;
            break;
        }
    }
    if (todos_iguais) {
        return 0;
    }
    return 1;
}

int validar_data(const char *data) {
    if (strlen(data) != 8) {
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(data[i])) {
            return 0;
        }
    }
    int dia = (data[0] - '0') * 10 + (data[1] - '0');
    int mes = (data[2] - '0') * 10 + (data[3] - '0');
    int ano = (data[4] - '0') * 1000 + (data[5] - '0') * 100 + (data[6] - '0') * 10 + (data[7] - '0');

    if (ano < 1900 || ano > 2100) {
        return 0;
    }
    if (mes < 1 || mes > 12) {
        return 0;
    }
    int dias_no_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        dias_no_mes[2] = 29;
    }
    if (dia < 1 || dia > dias_no_mes[mes]) {
        return 0;
    }
    return 1;
}

void formatar_data(const char *data_bruta, char *data_formatada) {
    if (strlen(data_bruta) != 8) {
        strcpy(data_formatada, "??/??/????");
        return;
    }
    snprintf(data_formatada, 11, "%c%c/%c%c/%c%c%c%c",
             data_bruta[0], data_bruta[1], data_bruta[2], data_bruta[3], data_bruta[4], data_bruta[5], data_bruta[6], data_bruta[7]);
}

void formatar_hora(int hora_inteira, char *hora_formatada) {
    int horas = hora_inteira / 100;
    int minutos = hora_inteira % 100;
    if (horas < 0 || horas > 23 || minutos < 0 || minutos > 59) {
        strcpy(hora_formatada, "??:??");
        return;
    }
    snprintf(hora_formatada, 6, "%02d:%02d", horas, minutos);
}

int validar_horario(int hora_inteira) {
    int horas = hora_inteira / 100;
    int minutos = hora_inteira % 100;
    if (horas < 0 || horas > 23) return 0;
    if (minutos < 0 || minutos > 59) return 0;
    return 1;
}

int minutos_validos(int hora_inteira) {
    int minutos = hora_inteira % 100;
    return (minutos == 0 || minutos == 15 || minutos == 30 || minutos == 45);
}

int paciente_existe(Sistema *sistema, const char *cpf) {
    for (int i = 0; i < sistema->total_pacientes; i++) {
        if (strcmp(sistema->pacientes[i].CPF, cpf) == 0) {
            return 1;
        }
    }
    return 0;
}

int comparar_datas(const char *data1, const char *data2) {
    int ano1 = (data1[4] - '0') * 1000 + (data1[5] - '0') * 100 + (data1[6] - '0') * 10 + (data1[7] - '0');
    int mes1 = (data1[2] - '0') * 10 + (data1[3] - '0');
    int dia1 = (data1[0] - '0') * 10 + (data1[1] - '0');
    int data_int1 = ano1 * 10000 + mes1 * 100 + dia1;

    int ano2 = (data2[4] - '0') * 1000 + (data2[5] - '0') * 100 + (data2[6] - '0') * 10 + (data2[7] - '0');
    int mes2 = (data2[2] - '0') * 10 + (data2[3] - '0');
    int dia2 = (data2[0] - '0') * 10 + (data2[1] - '0');
    int data_int2 = ano2 * 10000 + mes2 * 100 + dia2;

    if (data_int1 < data_int2) return -1;
    if (data_int1 > data_int2) return 1;
    return 0;
}

int comparar_datas_horas(const char *data1, int hora1, const char *data2, int hora2) {
    int cmp_data = comparar_datas(data1, data2);
    if (cmp_data != 0) return cmp_data;
    if (hora1 < hora2) return -1;
    if (hora1 > hora2) return 1;
    return 0;
}

int paciente_tem_agendamento_futuro(Sistema *sistema, const char *cpf) {
    time_t t = time(NULL);
    struct tm *agora = localtime(&t);

    char data_atual[9];
    snprintf(data_atual, 9, "%02d%02d%04d", agora->tm_mday, agora->tm_mon + 1, agora->tm_year + 1900);
    int hora_atual = agora->tm_hour * 100 + agora->tm_min;

    for (int i = 0; i < sistema->total_agendamentos; i++) {
        Agendamento *ag = &sistema->Agendamento[i];
        if (strcmp(ag->CPF_paciente, cpf) == 0) {
            if (comparar_datas_horas(ag->data_consulta, ag->hora_consulta, data_atual, hora_atual) > 0) {
                return 1;
            }
        }
    }
    return 0;
}

int agendamento_sobreposto(Sistema *sistema, const char *data, int hora_inicio, int hora_fim) {
    for (int i = 0; i < sistema->total_agendamentos; i++) {
        Agendamento *ag = &sistema->Agendamento[i];
        if (strcmp(ag->data_consulta, data) == 0) {
            if (hora_inicio < ag->final_consulta && hora_fim > ag->hora_consulta) {
                return 1;
            }
        }
    }
    return 0;
}

int calcular_idade(const char *data_nasc) {
    time_t t = time(NULL);
    struct tm *agora = localtime(&t);

    int ano_atual = agora->tm_year + 1900;
    int mes_atual = agora->tm_mon + 1;
    int dia_atual = agora->tm_mday;

    int ano_nasc = (data_nasc[4] - '0') * 1000 + (data_nasc[5] - '0') * 100 + (data_nasc[6] - '0') * 10 + (data_nasc[7] - '0');
    int mes_nasc = (data_nasc[2] - '0') * 10 + (data_nasc[3] - '0');
    int dia_nasc = (data_nasc[0] - '0') * 10 + (data_nasc[1] - '0');

    int idade = ano_atual - ano_nasc;

    if (mes_atual < mes_nasc || (mes_atual == mes_nasc && dia_atual < dia_nasc)) {
        idade--;
    }
    return idade;
}

void cadastrar_paciente(Sistema *sistema) {
    if (sistema->total_pacientes >= Pacientes_maximos) {
        printf("\nERRO: Capacidade maxima de pacientes atingida. Nao sera possivel cadastrar mais\n");
        return;
    }
    char cpf[12];
    char nome[Nomes_maximos];
    char data_nasc[9];
    int idade;

    while (1) {
        printf("CPF: ");
        fgets(cpf, sizeof(cpf), stdin);
        cpf[strcspn(cpf, "\n")] = 0;

        if (!validar_CPF(cpf)) {
            printf("ERRO: CPF invalido. Deve conter 11 digitos numericos e nao ser uma sequencia de digitos iguais\n");
            continue;
        }
        if (paciente_existe(sistema, cpf)) {
            printf("Erro: CPF ja cadastrado\n");
            continue;
        }
        limpar_buffer();
        break;
    }

    while (1) {
        printf("Nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = 0;

        if (strlen(nome) < 4) {
            printf("ERRO: Nome muito curto. Digite pelo menos 4 caracteres\n");
            continue;
        }
        Maisculo(nome);
        break;
    }

    while (1) {
        printf("Data de nascimento: ");
        fgets(data_nasc, sizeof(data_nasc), stdin);
        data_nasc[strcspn(data_nasc, "\n")] = 0;

        if (!validar_data(data_nasc)) {
            printf("ERRO: Data de nascimento invalida. Use o formato DDMMAAAA (ex: 01012000)\n");
            continue;
        }
        idade = calcular_idade(data_nasc);
        if (idade < 13) {
            printf("Erro: paciente deve ter 13 anos ou mais\n");
            continue;
        }
        break;
    }

    strcpy(sistema->pacientes[sistema->total_pacientes].CPF, cpf);
    strcpy(sistema->pacientes[sistema->total_pacientes].nome, nome);
    strcpy(sistema->pacientes[sistema->total_pacientes].data_nascimento, data_nasc);
    sistema->pacientes[sistema->total_pacientes].idade = idade;

    sistema->total_pacientes++;
    printf("\nPaciente '%s' cadastrado com sucesso!\n", nome);
}

void excluir_paciente(Sistema *sistema) {
    if (sistema->total_pacientes == 0) {
        printf("\nNenhum paciente cadastrado para exclusao\n");
        return;
    }

    char cpf[12];
    int paciente_encontrado_idx = -1;

    printf("CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    for (int i = 0; i < sistema->total_pacientes; i++) {
        if (strcmp(sistema->pacientes[i].CPF, cpf) == 0) {
            paciente_encontrado_idx = i;
            break;
        }
    }

    if (paciente_encontrado_idx == -1) {
        printf("Erro: paciente não cadastrado\n");
        return;
    }

    if (paciente_tem_agendamento_futuro(sistema, cpf)) {
        printf("ERRO: Paciente possui agendamento(s) futuro(s) e nao pode ser excluido\n");
        return;
    }

    for (int i = 0; i < sistema->total_agendamentos; ) {
        if (strcmp(sistema->Agendamento[i].CPF_paciente, cpf) == 0) {
            sistema->Agendamento[i] = sistema->Agendamento[sistema->total_agendamentos - 1];
            sistema->total_agendamentos--;
        } else {
            i++; 
        }
    }

    sistema->pacientes[paciente_encontrado_idx] = sistema->pacientes[sistema->total_pacientes - 1];
    sistema->total_pacientes--;
    printf("\nPaciente com CPF '%s' e todos os seus agendamentos foram excluidos com sucesso\n", cpf);
}

void agendar_consulta(Sistema *sistema) {
    if (sistema->total_agendamentos >= Agendamentos_maximos) {
        printf("\nERRO: Capacidade maxima de agendamentos atingida. Nao sera possivel agendar mais consultas\n");
        return;
    }

    char cpf[12];
    char data_consulta[9];
    int hora_inicio, hora_fim;

    time_t t = time(NULL);
    struct tm *agora = localtime(&t);
    char data_atual[9];
    snprintf(data_atual, 9, "%02d%02d%04d", agora->tm_mday, agora->tm_mon + 1, agora->tm_year + 1900);
    int hora_atual = agora->tm_hour * 100 + agora->tm_min;

    while (1) {
        printf("CPF: ");
        fgets(cpf, sizeof(cpf), stdin);
        cpf[strcspn(cpf, "\n")] = 0;

        if (!paciente_existe(sistema, cpf)) {
            printf("Erro: paciente não cadastrado\n");
            continue;
        }
        if (paciente_tem_agendamento_futuro(sistema, cpf)) {
            printf("ERRO: Paciente com CPF '%s' ja possui um agendamento futuro\n", cpf);
            return;
        }
        limpar_buffer();
        break;
    }

    while (1) {
        printf("Data da consulta: ");
        fgets(data_consulta, sizeof(data_consulta), stdin);
        data_consulta[strcspn(data_consulta, "\n")] = 0;

        if (!validar_data(data_consulta)) {
            printf("ERRO: Data invalida. Use o formato DDMMAAAA (ex: 01012025)\n");
            continue;
        }
        if (comparar_datas(data_consulta, data_atual) < 0) {
            printf("ERRO: Data da consulta nao pode ser no passado\n");
            continue;
        }
        break;
    }

    while (1) {
        printf("Hora inicial: ");
        if (scanf("%d", &hora_inicio) != 1) {
            printf("ERRO: Entrada invalida. Digite apenas numeros\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        if (!validar_horario(hora_inicio)) {
            printf("ERRO: Horario inicial invalido. Formato esperado HHMM (0000 a 2359)\n");
            continue;
        }
        if (!minutos_validos(hora_inicio)) {
            printf("ERRO: Minutos devem ser 00, 15, 30 ou 45\n");
            continue;
        }
        if (comparar_datas(data_consulta, data_atual) == 0 && hora_inicio <= hora_atual) {
            printf("ERRO: Para consultas hoje, a hora inicial deve ser futura em relacao a hora atual\n");
            continue;
        }
        if (hora_inicio < 800 || hora_inicio >= 1900) {
            printf("ERRO: Hora inicial fora do horario de funcionamento do consultorio (08:00 as 19:00)\n");
            continue;
        }
        break;
    }

    while (1) {
    printf("Hora final: ");
        if (scanf("%d", &hora_fim) != 1) {
            printf("ERRO: Entrada invalida. Digite apenas numeros\n");
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        if (!validar_horario(hora_fim)) {
            printf("ERRO: Horario final invalido. Formato esperado HHMM (0000 a 2359)\n");
            continue;
        }
        if (!minutos_validos(hora_fim)) {
            printf("ERRO: Minutos devem ser 00, 15, 30 ou 45\n");
            continue;
        }
        if (hora_fim <= hora_inicio) {
            printf("ERRO: Hora final deve ser maior que hora inicial\n");
            continue;
        }
        if (hora_fim > 1900) {
            printf("ERRO: Hora final fora do horario de funcionamento do consultorio (maximo 19:00)\n");
            continue;
        }
        break;
    }

    if (agendamento_sobreposto(sistema, data_consulta, hora_inicio, hora_fim)) {
        printf("Erro: ja existe uma consulta agendada nessa data/hora\n");
        return;
    }

    strcpy(sistema->Agendamento[sistema->total_agendamentos].CPF_paciente, cpf);
    strcpy(sistema->Agendamento[sistema->total_agendamentos].data_consulta, data_consulta);
    sistema->Agendamento[sistema->total_agendamentos].hora_consulta = hora_inicio;
    sistema->Agendamento[sistema->total_agendamentos].final_consulta = hora_fim;

    sistema->total_agendamentos++;
    printf("\nAgendamento realizado com sucesso!\n");
}

void cancelar_agendamento(Sistema *sistema) {
    if (sistema->total_agendamentos == 0) {
        printf("\nErro: agendamento nao encontrado\n");
        return;
    }

    char cpf[12];
    int agendamento_encontrado_idx = -1;

    printf("CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    time_t t = time(NULL);
    struct tm *agora = localtime(&t);
    char data_atual[9];
    snprintf(data_atual, 9, "%02d%02d%04d", agora->tm_mday, agora->tm_mon + 1, agora->tm_year + 1900);
    int hora_atual = agora->tm_hour * 100 + agora->tm_min;

    for (int i = 0; i < sistema->total_agendamentos; i++) {
        Agendamento *ag = &sistema->Agendamento[i];
        if (strcmp(ag->CPF_paciente, cpf) == 0) {
            if (comparar_datas_horas(ag->data_consulta, ag->hora_consulta, data_atual, hora_atual) > 0) {
                agendamento_encontrado_idx = i;
                break;
            }
        }
    }

    if (agendamento_encontrado_idx == -1) {
        printf("Erro: agendamento nao encontrado\n");
        return;
    }

    sistema->Agendamento[agendamento_encontrado_idx] = sistema->Agendamento[sistema->total_agendamentos - 1];
    sistema->total_agendamentos--;

    printf("\nAgendamento cancelado com sucesso!\n");
}

int comparar_pacientes_cpf(const void *a, const void *b) {
    Paciente *pacienteA = (Paciente *)a;
    Paciente *pacienteB = (Paciente *)b;
    return strcmp(pacienteA->CPF, pacienteB->CPF);
}

int comparar_pacientes_nome(const void *a, const void *b) {
    Paciente *pacienteA = (Paciente *)a;
    Paciente *pacienteB = (Paciente *)b;
    return strcmp(pacienteA->nome, pacienteB->nome);
}

void listar_pacientes(Sistema *sistema, int escolha_ordenacao) { 
    if (sistema->total_pacientes == 0) {
        printf("\nNenhum paciente cadastrado\n");
        return;
    }

    if (escolha_ordenacao == 1) { 
        qsort(sistema->pacientes, sistema->total_pacientes, sizeof(Paciente), comparar_pacientes_cpf);
        printf("\nLista de Pacientes (Ordenada por CPF):\n");
    } else if (escolha_ordenacao == 2) { 
        qsort(sistema->pacientes, sistema->total_pacientes, sizeof(Paciente), comparar_pacientes_nome);
        printf("\nLista de Pacientes (Ordenada por Nome):\n");
    } else {
        printf("Opcao de ordenacao invalida. Exibindo na ordem padrao\n");
    }

    printf("-----------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-11s | %-40s | %-12s | %-5s | %-12s | %-10s |\n",
           "CPF", "Nome", "Nascimento", "Idade", "Data Consulta", "Hora Cons");
    printf("-----------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < sistema->total_pacientes; i++) {
        char data_nasc_formatada[11];
        formatar_data(sistema->pacientes[i].data_nascimento, data_nasc_formatada);

        int future_agendamento_idx = -1;
        time_t t = time(NULL);
        struct tm *agora = localtime(&t);
        char data_atual[9];
        snprintf(data_atual, 9, "%02d%02d%04d", agora->tm_mday, agora->tm_mon + 1, agora->tm_year + 1900);
        int hora_atual = agora->tm_hour * 100 + agora->tm_min;

        for (int j = 0; j < sistema->total_agendamentos; j++) {
            if (strcmp(sistema->Agendamento[j].CPF_paciente, sistema->pacientes[i].CPF) == 0) {
                if (comparar_datas_horas(sistema->Agendamento[j].data_consulta, sistema->Agendamento[j].hora_consulta, data_atual, hora_atual) > 0) {
                    future_agendamento_idx = j;
                    break;
                }
            }
        }

        char data_consulta_formatada[11] = "---";
        char hora_consulta_formatada[6] = "---";

        if (future_agendamento_idx != -1) {
            formatar_data(sistema->Agendamento[future_agendamento_idx].data_consulta, data_consulta_formatada);
            formatar_hora(sistema->Agendamento[future_agendamento_idx].hora_consulta, hora_consulta_formatada);
        }

        printf("| %-11s | %-40s | %-12s | %-5d | %-12s | %-10s |\n",
               sistema->pacientes[i].CPF,
               sistema->pacientes[i].nome,
               data_nasc_formatada,
               sistema->pacientes[i].idade,
               data_consulta_formatada,
               hora_consulta_formatada);
    }
    printf("-----------------------------------------------------------------------------------------------------------------------\n");
}

int comparar_agendamentos_data_hora(const void *a, const void *b) {
    Agendamento *agendamentoA = (Agendamento *)a;
    Agendamento *agendamentoB = (Agendamento *)b;

    int cmp_data = comparar_datas(agendamentoA->data_consulta, agendamentoB->data_consulta);
    if (cmp_data != 0) {
        return cmp_data;
    }
    if (agendamentoA->hora_consulta < agendamentoB->hora_consulta) return -1;
    if (agendamentoA->hora_consulta > agendamentoB->hora_consulta) return 1;
    return 0;
}

void listar_agendamentos(Sistema *sistema) {
    if (sistema->total_agendamentos == 0) {
        printf("\nNenhum agendamento cadastrado\n");
        return;
    }

    Agendamento agendamentos_filtrados[Agendamentos_maximos];
    int total_filtrados = 0;

    int escolha_filtragem;
    printf("\nListagem da Agenda\n");
    printf("Opcoes de Listagem:\n");
    printf("1 - Listar toda a agenda\n");
    printf("2 - Listar agenda de um periodo\n");
    printf("Escolha uma opcao: ");
    if (scanf("%d", &escolha_filtragem) != 1) {
        printf("ERRO: Entrada invalida. Digite 1 ou 2\n");
        limpar_buffer();
        return;
    }
    limpar_buffer();

    char data_inicio_filtro[9] = "";
    char data_fim_filtro[9] = "";

    if (escolha_filtragem == 2) {
        while (1) {
            printf("Data Inicial: ");
            fgets(data_inicio_filtro, sizeof(data_inicio_filtro), stdin);
            data_inicio_filtro[strcspn(data_inicio_filtro, "\n")] = 0;
            if (!validar_data(data_inicio_filtro)) {
                printf("ERRO: Data inicial invalida. Use o formato DDMMAAAA\n");
                continue;
            }
            break;
        }
        while (1) {
            printf("Data Final: ");
            fgets(data_fim_filtro, sizeof(data_fim_filtro), stdin);
            data_fim_filtro[strcspn(data_fim_filtro, "\n")] = 0;
            if (!validar_data(data_fim_filtro)) {
                printf("ERRO: Data final invalida. Use o formato DDMMAAAA\n");
                continue;
            }
            if (comparar_datas(data_inicio_filtro, data_fim_filtro) > 0) {
                printf("ERRO: Data final nao pode ser anterior a data inicial\n");
                continue;
            }
            break;
        }

        for (int i = 0; i < sistema->total_agendamentos; i++) {
            if (comparar_datas(sistema->Agendamento[i].data_consulta, data_inicio_filtro) >= 0 &&
                comparar_datas(sistema->Agendamento[i].data_consulta, data_fim_filtro) <= 0) {
                agendamentos_filtrados[total_filtrados++] = sistema->Agendamento[i];
            }
        }
    } else if (escolha_filtragem == 1) {
        for (int i = 0; i < sistema->total_agendamentos; i++) {
            agendamentos_filtrados[total_filtrados++] = sistema->Agendamento[i];
        }
    } else {
        printf("Opcao de listagem invalida\n");
        return;
    }

    if (total_filtrados == 0) {
        printf("\nNenhum agendamento encontrado para os criterios selecionados\n");
        return;
    }

    qsort(agendamentos_filtrados, total_filtrados, sizeof(Agendamento), comparar_agendamentos_data_hora);

    printf("---------------------------------------------------------------------\n");
    printf("| %-11s | %-12s | %-10s | %-10s | %-20s |\n",
           "CPF Paciente", "Data Consulta", "Hora Inicio", "Hora Fim", "Nome Paciente");
    printf("---------------------------------------------------------------------\n");

    for (int i = 0; i < total_filtrados; i++) {
        char data_formatada[11], hora_inicio_formatada[6], hora_fim_formatada[6];
        formatar_data(agendamentos_filtrados[i].data_consulta, data_formatada);
        formatar_hora(agendamentos_filtrados[i].hora_consulta, hora_inicio_formatada);
        formatar_hora(agendamentos_filtrados[i].final_consulta, hora_fim_formatada);

        char nome_paciente[Nomes_maximos] = "DESCONHECIDO";
        for (int j = 0; j < sistema->total_pacientes; j++) {
            if (strcmp(sistema->pacientes[j].CPF, agendamentos_filtrados[i].CPF_paciente) == 0) {
                strncpy(nome_paciente, sistema->pacientes[j].nome, Nomes_maximos - 1);
                nome_paciente[Nomes_maximos - 1] = '\0';
                break;
            }
        }

        printf("| %-11s | %-12s | %-10s | %-10s | %-20s |\n",
               agendamentos_filtrados[i].CPF_paciente,
               data_formatada,
               hora_inicio_formatada,
               hora_fim_formatada,
               nome_paciente);
    }
    printf("---------------------------------------------------------------------\n");
}

void menu_cadastro_pacientes(Sistema *sistema) {
    int opcao;
    do {
        printf("\nMenu do Cadastro de Pacientes\n");
        printf("1-Cadastrar novo paciente\n");
        printf("2-Excluir paciente\n");
        printf("3-Listar pacientes (ordenado por CPF)\n");
        printf("4-Listar pacientes (ordenado por nome)\n");
        printf("5-Voltar p/ menu principal\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("ERRO: Entrada invalida. Por favor, digite um numero\n");
            limpar_buffer();
            opcao = -1; 
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1:
                cadastrar_paciente(sistema);
                break;
            case 2:
                excluir_paciente(sistema);
                break;
            case 3:
                listar_pacientes(sistema, 1); 
                break;
            case 4:
                listar_pacientes(sistema, 2); 
                break;
            case 5:
                break;
            default:
                printf("\nOpcao invalida! Por favor, escolha uma opcao entre 1 e 5\n");
        }
    } while (opcao != 5);
}

void menu_agenda(Sistema *sistema) {
    int opcao;
    do {
        printf("\nAgenda\n");
        printf("1-Agendar consulta\n");
        printf("2-Cancelar agendamento\n");
        printf("3-Listar agenda\n");
        printf("4-Voltar p/ menu principal\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("ERRO: Entrada invalida. Por favor, digite um numero\n");
            limpar_buffer();
            opcao = -1;
            continue;
        }
        limpar_buffer();

        switch (opcao) {
            case 1:
                agendar_consulta(sistema);
                break;
            case 2:
                cancelar_agendamento(sistema);
                break;
            case 3:
                listar_agendamentos(sistema);
                break;
            case 4:
                break;
            default:
                printf("\nOpcao invalida! Por favor, escolha uma opcao entre 1 e 4\n");
        }
    } while (opcao != 4);
}


int main() {
    Sistema sistema = { .total_pacientes = 0, .total_agendamentos = 0 };
    int opcao_principal;

    do {
        printf("\nMenu Principal\n");
        printf("1-Cadastro de pacientes\n");
        printf("2-Agenda\n");
        printf("3-Fim\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao_principal) != 1) {
            printf("ERRO: Entrada invalida. Por favor, digite um numero\n");
            limpar_buffer();
            opcao_principal = -1;
            continue;
        }
        limpar_buffer();

        switch (opcao_principal) {
            case 1:
                menu_cadastro_pacientes(&sistema);
                break;
            case 2:
                menu_agenda(&sistema);
                break;
            case 3:
                break;
            default:
                printf("\nOpcao invalida! Por favor, escolha uma opcao entre 1 e 3\n");
        }
        if (opcao_principal != 3) { 
        }
    } while (opcao_principal != 3);

    return 0;
}