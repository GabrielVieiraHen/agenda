#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTATOS 100
#define MAX_NOME 50
#define MAX_TELEFONE 15
#define MAX_EMAIL 50

// Definição da struct Contato
struct Contato {
    char nome[MAX_NOME];
    char telefone[MAX_TELEFONE];
    char email[MAX_EMAIL];
    int ativo; // 1 para ativo, 0 para inativo
};

// Declaração das funções
void adicionarContato(struct Contato agenda[], int *totalContatos);
void listarContatos(struct Contato agenda[], int totalContatos);
void buscarContato(struct Contato agenda[], int totalContatos);
void excluirContato(struct Contato agenda[], int *totalContatos);
void salvarAgenda(struct Contato agenda[], int totalContatos);
void carregarAgenda(struct Contato agenda[], int *totalContatos);
void limparTela();
void pausar();

int main() {
    struct Contato agenda[MAX_CONTATOS];
    int totalContatos = 0;
    int opcao;

    // Carregar contatos do arquivo no início
    carregarAgenda(agenda, &totalContatos);

    do {
        limparTela();
        printf("\n=== AGENDA DE CONTATOS ===\n");
        printf("1. Adicionar Contato\n");
        printf("2. Listar Contatos\n");
        printf("3. Buscar Contato\n");
        printf("4. Excluir Contato\n");
        printf("5. Salvar e Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer

        switch(opcao) {
            case 1:
                adicionarContato(agenda, &totalContatos);
                break;
            case 2:
                listarContatos(agenda, totalContatos);
                break;
            case 3:
                buscarContato(agenda, totalContatos);
                break;
            case 4:
                excluirContato(agenda, &totalContatos);
                break;
            case 5:
                salvarAgenda(agenda, totalContatos);
                printf("\nAgenda salva com sucesso! Encerrando...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
                pausar();
        }
    } while(opcao != 5);

    return 0;
}

void adicionarContato(struct Contato agenda[], int *totalContatos) {
    if(*totalContatos >= MAX_CONTATOS) {
        printf("\nAgenda cheia!\n");
        pausar();
        return;
    }

    limparTela();
    printf("\n=== ADICIONAR CONTATO ===\n");
    
    printf("Nome: ");
    fgets(agenda[*totalContatos].nome, MAX_NOME, stdin);
    agenda[*totalContatos].nome[strcspn(agenda[*totalContatos].nome, "\n")] = 0;

    printf("Telefone: ");
    fgets(agenda[*totalContatos].telefone, MAX_TELEFONE, stdin);
    agenda[*totalContatos].telefone[strcspn(agenda[*totalContatos].telefone, "\n")] = 0;

    printf("Email: ");
    fgets(agenda[*totalContatos].email, MAX_EMAIL, stdin);
    agenda[*totalContatos].email[strcspn(agenda[*totalContatos].email, "\n")] = 0;

    agenda[*totalContatos].ativo = 1;
    (*totalContatos)++;

    printf("\nContato adicionado com sucesso!\n");
    pausar();
}

void listarContatos(struct Contato agenda[], int totalContatos) {
    limparTela();
    printf("\n=== LISTA DE CONTATOS ===\n");
    
    if(totalContatos == 0) {
        printf("\nNenhum contato cadastrado!\n");
    } else {
        for(int i = 0; i < totalContatos; i++) {
            if(agenda[i].ativo) {
                printf("\nContato %d:", i+1);
                printf("\nNome: %s", agenda[i].nome);
                printf("\nTelefone: %s", agenda[i].telefone);
                printf("\nEmail: %s\n", agenda[i].email);
                printf("------------------------\n");
            }
        }
    }
    pausar();
}

void buscarContato(struct Contato agenda[], int totalContatos) {
    char busca[MAX_NOME];
    int encontrou = 0;

    limparTela();
    printf("\n=== BUSCAR CONTATO ===\n");
    printf("Digite o nome para buscar: ");
    fgets(busca, MAX_NOME, stdin);
    busca[strcspn(busca, "\n")] = 0;

    for(int i = 0; i < totalContatos; i++) {
        if(agenda[i].ativo && strstr(agenda[i].nome, busca) != NULL) {
            printf("\nContato encontrado:");
            printf("\nNome: %s", agenda[i].nome);
            printf("\nTelefone: %s", agenda[i].telefone);
            printf("\nEmail: %s\n", agenda[i].email);
            encontrou = 1;
        }
    }

    if(!encontrou) {
        printf("\nNenhum contato encontrado!\n");
    }
    pausar();
}

void excluirContato(struct Contato agenda[], int *totalContatos) {
    int indice;

    limparTela();
    printf("\n=== EXCLUIR CONTATO ===\n");
    listarContatos(agenda, *totalContatos);
    
    printf("\nDigite o numero do contato que deseja excluir: ");
    scanf("%d", &indice);
    getchar();
    
    if(indice > 0 && indice <= *totalContatos) {
        agenda[indice-1].ativo = 0;
        printf("\nContato excluido com sucesso!\n");
    } else {
        printf("\nIndice invalido!\n");
    }
    pausar();
}

void salvarAgenda(struct Contato agenda[], int totalContatos) {
    FILE *arquivo = fopen("agenda.txt", "w");
    
    if(arquivo == NULL) {
        printf("\nErro ao abrir o arquivo!\n");
        return;
    }

    for(int i = 0; i < totalContatos; i++) {
        if(agenda[i].ativo) {
            fprintf(arquivo, "%s;%s;%s\n", 
                agenda[i].nome, 
                agenda[i].telefone, 
                agenda[i].email);
        }
    }

    fclose(arquivo);
}

void carregarAgenda(struct Contato agenda[], int *totalContatos) {
    FILE *arquivo = fopen("agenda.txt", "r");
    
    if(arquivo == NULL) {
        return;
    }

    char linha[MAX_NOME + MAX_TELEFONE + MAX_EMAIL + 3];
    *totalContatos = 0;

    while(fgets(linha, sizeof(linha), arquivo) && *totalContatos < MAX_CONTATOS) {
        linha[strcspn(linha, "\n")] = 0;
        
        char *token = strtok(linha, ";");
        if(token != NULL) {
            strcpy(agenda[*totalContatos].nome, token);
            
            token = strtok(NULL, ";");
            if(token != NULL) {
                strcpy(agenda[*totalContatos].telefone, token);
                
                token = strtok(NULL, ";");
                if(token != NULL) {
                    strcpy(agenda[*totalContatos].email, token);
                    agenda[*totalContatos].ativo = 1;
                    (*totalContatos)++;
                }
            }
        }
    }

    fclose(arquivo);
}

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}
