#include "accesses.h"
#include <stdlib.h>
#include <string.h>

void load_accesses_from_csv(const char *csv_filename, const char *bin_filename) {
    FILE *csv_file = fopen(csv_filename, "r");
    if (csv_file == NULL) {
        printf("Erro ao abrir arquivo CSV de acessos: %s\n", csv_filename);
        return;
    }

    FILE *bin_file = fopen(bin_filename, "wb");
    if (bin_file == NULL) {
        printf("Erro ao criar arquivo binário de acessos: %s\n", bin_filename);
        fclose(csv_file);
        return;
    }

    Access access;
    char line[512];
    fgets(line, sizeof(line), csv_file); // Ignorar o cabeçalho

    while (fgets(line, sizeof(line), csv_file)) {
        // Quebra a linha e carrega os campos correspondentes
        char *token = strtok(line, ",");
        strcpy(access.event_time, token);

        token = strtok(NULL, ",");
        strcpy(access.event_type, token);

        token = strtok(NULL, ",");
        access.product_id = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");
        access.category_id = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");
        strcpy(access.category_code, token);

        token = strtok(NULL, ",");
        strcpy(access.brand, token);

        token = strtok(NULL, ",");
        access.price = atof(token);

        token = strtok(NULL, ",");
        access.user_id = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");
        strcpy(access.user_session, token);

        // Grava o registro no arquivo binário
        fwrite(&access, sizeof(Access), 1, bin_file);
    }

    fclose(csv_file);
    fclose(bin_file);
}

void show_accesses(const char *bin_filename) {
    FILE *bin_file = fopen(bin_filename, "rb");
    if (bin_file == NULL) {
        printf("Erro ao abrir arquivo binário de acessos: %s\n", bin_filename);
        return;
    }

    Access access;
    while (fread(&access, sizeof(Access), 1, bin_file)) {
        printf("Event Time: %s, Event Type: %s, Product ID: %llu, Category ID: %llu, Brand: %s, Price: %.2f, User ID: %llu, Session: %s\n",
               access.event_time, access.event_type, access.product_id, access.category_id,
               access.brand, access.price, access.user_id, access.user_session);
    }

    fclose(bin_file);
}
