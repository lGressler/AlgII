#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "products.h"
#include "accesses.h"

#define DATASET_FOLDER "datasets/"
#define CSV_EXTENSION ".csv"

//teste

// Função para verificar se o arquivo tem extensão .csv
int is_csv_file(const char *filename) {
    size_t len = strlen(filename);
    return len > 4 && strcmp(filename + len - 4, CSV_EXTENSION) == 0;
}

// Função que processa todos os arquivos .csv da pasta datasets
void process_all_files() {
    WIN32_FIND_DATA find_file_data;
    HANDLE h_find = INVALID_HANDLE_VALUE;
    char search_path[256];

    // Monta o caminho de busca para arquivos na pasta datasets
    snprintf(search_path, sizeof(search_path), "%s*%s", DATASET_FOLDER, CSV_EXTENSION);

    h_find = FindFirstFile(search_path, &find_file_data);
    if (h_find == INVALID_HANDLE_VALUE) {
        printf("Erro ao abrir a pasta datasets.\n");
        return;
    }

    do {
        if (!(find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            // Cria o caminho completo para o arquivo
            char filepath[256];
            snprintf(filepath, sizeof(filepath), "%s%s", DATASET_FOLDER, find_file_data.cFileName);

            // Cria os nomes dos arquivos binários para produtos e acessos
            char products_bin[256], accesses_bin[256];
            snprintf(products_bin, sizeof(products_bin), "%s_products.bin", find_file_data.cFileName);
            snprintf(accesses_bin, sizeof(accesses_bin), "%s_accesses.bin", find_file_data.cFileName);

            printf("Processando arquivo: %s\n", filepath);

            // Carrega os produtos e acessos do CSV
            load_products_from_csv(filepath, products_bin);
            load_accesses_from_csv(filepath, accesses_bin);

            // Exibe os produtos e acessos
            show_products(products_bin);
            show_accesses(accesses_bin);
        }
    } while (FindNextFile(h_find, &find_file_data) != 0);

    FindClose(h_find);
}

// Função para exibir o menu e chamar as funções conforme a escolha do usuário
void menu() {
    int option;
    char product_file[256], access_file[256], key[256];

    do {
        printf("\nMenu:\n");
        printf("1. Processar todos os arquivos CSV\n");
        printf("2. Mostrar produtos\n");
        printf("3. Mostrar acessos\n");
        printf("4. Consultar produto (Pesquisa binária)\n");
        printf("5. Consultar acesso (Pesquisa binária)\n");
        printf("6. Inserir novo produto\n");
        printf("7. Inserir novo acesso\n");
        printf("8. Remover produto\n");
        printf("9. Remover acesso\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                process_all_files();
                break;
            case 2:
                printf("Digite o nome do arquivo binário de produtos: ");
                scanf("%s", product_file);
                show_products(product_file);
                break;
            case 3:
                printf("Digite o nome do arquivo binário de acessos: ");
                scanf("%s", access_file);
                show_accesses(access_file);
                break;
            case 4:
                printf("Digite o nome do arquivo binário de produtos: ");
                scanf("%s", product_file);
                printf("Digite a chave do produto (ID) a ser buscado: ");
                scanf("%s", key);
                search_product(product_file, key);
                break;
            case 5:
                printf("Digite o nome do arquivo binário de acessos: ");
                scanf("%s", access_file);
                printf("Digite a chave do acesso (user_id) a ser buscado: ");
                scanf("%s", key);
                search_access(access_file, key);
                break;
            case 6:
                printf("Digite o nome do arquivo binário de produtos: ");
                scanf("%s", product_file);
                insert_product(product_file);
                break;
            case 7:
                printf("Digite o nome do arquivo binário de acessos: ");
                scanf("%s", access_file);
                insert_access(access_file);
                break;
            case 8:
                printf("Digite o nome do arquivo binário de produtos: ");
                scanf("%s", product_file);
                printf("Digite a chave do produto (ID) a ser removido: ");
                scanf("%s", key);
                remove_product(product_file, key);
                break;
            case 9:
                printf("Digite o nome do arquivo binário de acessos: ");
                scanf("%s", access_file);
                printf("Digite a chave do acesso (user_id) a ser removido: ");
                scanf("%s", key);
                remove_access(access_file, key);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (option != 0);
}

int main() {
    // Chama o menu
    menu();

    return 0;
}
