#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "products.h"

void create_index(const char *data_filename, const char *index_filename) {
    FILE *data_file = fopen(data_filename, "rb");
    FILE *index_file = fopen(index_filename, "wb");
    if (!data_file || !index_file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Product p;
    IndexEntry entry;
    long offset = 0;

    while (fread(&p, sizeof(Product), 1, data_file)) {
        entry.key = p.product_id;
        entry.offset = offset;
        if (fwrite(&entry, sizeof(IndexEntry), 1, index_file) != 1) {
            perror("Erro ao escrever no arquivo de índice");
            fclose(data_file);
            fclose(index_file);
            return;
        }
        offset += sizeof(Product);
    }

    fclose(data_file);
    fclose(index_file);
}

IndexEntry* binary_search_index(const char *index_filename, unsigned long long target_key) {
    FILE *index_file = fopen(index_filename, "rb");
    if (!index_file) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    IndexEntry result;
    int left = 0;
    fseek(index_file, 0, SEEK_END);
    int right = ftell(index_file) / sizeof(IndexEntry) - 1;

    while (left <= right) {
        int middle = (left + right) / 2;
        fseek(index_file, middle * sizeof(IndexEntry), SEEK_SET);
        fread(&result, sizeof(IndexEntry), 1, index_file);

        if (result.key == target_key) {
            fclose(index_file);
            IndexEntry *found_entry = (IndexEntry*) malloc(sizeof(IndexEntry));
            *found_entry = result;  // Copiar resultado encontrado
            return found_entry;
        } else if (result.key < target_key) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }

    fclose(index_file);
    return NULL;
}

void search_with_index(const char *data_filename, const char *index_filename, unsigned long long target_key) {
    IndexEntry *index_result = binary_search_index(index_filename, target_key);
    if (!index_result) {
        printf("Registro não encontrado.\n");
        return;
    }

    FILE *data_file = fopen(data_filename, "rb");
    if (!data_file) {
        perror("Erro ao abrir arquivo");
        free(index_result);
        return;
    }

    fseek(data_file, index_result->offset, SEEK_SET);
    Product p;
    fread(&p, sizeof(Product), 1, data_file);

    printf("Registro encontrado: ID = %llu, Marca = %s, Preço = %.2f\n", p.product_id, p.brand, p.price);

    fclose(data_file);
    free(index_result);
}

void remove_product(const char *data_filename, const char *index_filename, unsigned long long target_id) {
    FILE *data_file = fopen(data_filename, "rb");
    FILE *temp_file = fopen("temp.bin", "wb");
    if (!data_file || !temp_file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Product p;
    int found = 0;

    while (fread(&p, sizeof(Product), 1, data_file)) {
        if (p.product_id != target_id) {
            fwrite(&p, sizeof(Product), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(data_file);
    fclose(temp_file);

    if (found) {
        remove(data_filename);
        rename("temp.bin", data_filename);
        create_index(data_filename, index_filename);
    } else {
        printf("Produto com ID %llu não encontrado.\n", target_id);
        remove("temp.bin");
    }
}
