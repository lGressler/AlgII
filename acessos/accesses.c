#include "accesses.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void load_accesses_from_csv(const char *csv_filename, const char *binary_filename) {
    FILE *csv_file = fopen(csv_filename, "r");
    if (csv_file == NULL) {
        printf("Erro ao abrir arquivo CSV de acessos: %s\n", csv_filename);
        return;
    }

    FILE *bin_file = fopen(binary_filename, "wb");
    if (bin_file == NULL) {
        printf("Erro ao criar arquivo binário de acessos: %s\n", binary_filename);
        fclose(csv_file);
        return;
    }

    Access *accesses = (Access *)malloc(sizeof(Access) * 1000); // Supondo até 1000 acessos
    int count = 0;
    char line[512];
    fgets(line, sizeof(line), csv_file); // Ignorar cabeçalho

    while (fgets(line, sizeof(line), csv_file)) {
        Access access;
        char *token = strtok(line, ",");
        strncpy(access.event_time, token, 19);
        access.event_time[19] = '\0'; // Garantir null-terminator

        token = strtok(NULL, ",");
        strncpy(access.event_type, token, 19);
        access.event_type[19] = '\0'; // Garantir null-terminator

        token = strtok(NULL, ",");
        access.product_id = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");
        access.category_id = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");
        strncpy(access.category_code, token, 49);
        access.category_code[49] = '\0'; // Garantir null-terminator

        token = strtok(NULL, ",");
        strncpy(access.brand, token, 49);
        access.brand[49] = '\0'; // Garantir null-terminator

        token = strtok(NULL, ",");
        access.price = atof(token);

        token = strtok(NULL, ",");
        access.user_id = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");
        strncpy(access.user_session, token, 49);
        access.user_session[49] = '\0'; // Garantir null-terminator

        accesses[count++] = access; // Adiciona o acesso ao array
    }

    fwrite(accesses, sizeof(Access), count, bin_file);
    fclose(csv_file);
    fclose(bin_file);
    free(accesses);
}

void show_accesses(const char *bin_filename) {
    FILE *bin_file = fopen(bin_filename, "rb");
    if (bin_file == NULL) {
        printf("Erro ao abrir arquivo binário de acessos: %s\n", bin_filename);
        return;
    }

    Access access;
    while (fread(&access, sizeof(Access), 1, bin_file)) {
        printf("Tempo: %s, Tipo: %s, ID Produto: %llu, ID Categoria: %llu, Categoria: %s, Marca: %s, Preço: %.2f, ID Usuário: %llu, Sessão: %s\n",
               access.event_time, access.event_type, access.product_id, access.category_id,
               access.category_code, access.brand, access.price, access.user_id, access.user_session);
    }

    fclose(bin_file);
}

Access* search_access(const char *bin_filename, unsigned long long product_id) {
    FILE *bin_file = fopen(bin_filename, "rb");
    if (bin_file == NULL) {
        printf("Erro ao abrir arquivo binário de acessos: %s\n", bin_filename);
        return NULL;
    }

    Access *access = (Access *)malloc(sizeof(Access));
    while (fread(access, sizeof(Access), 1, bin_file)) {
        if (access->product_id == product_id) {
            fclose(bin_file);
            return access;
        }
    }

    free(access);
    fclose(bin_file);
    return NULL;
}

void remove_access(const char *filename, unsigned long long product_id) {
    FILE *file = fopen(filename, "rb");
    FILE *temp_file = fopen("temp_access.bin", "wb");
    if (!file || !temp_file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Access access;
    int found = 0;

    while (fread(&access, sizeof(Access), 1, file)) {
        if (access.product_id != product_id) {
            fwrite(&access, sizeof(Access), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove(filename);
        rename("temp_access.bin", filename);
    } else {
        printf("Acesso com ID de produto %llu não encontrado.\n", product_id);
        remove("temp_access.bin");
    }
}
