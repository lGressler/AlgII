// products.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "products.h"

void insert_product(const char *filename, Product product) {
    FILE *file = fopen(filename, "ab");
    if (file) {
        fwrite(&product, sizeof(Product), 1, file);
        fclose(file);
    } else {
        perror("Erro ao abrir arquivo de produtos");
    }
}

void show_products(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        Product product;
        while (fread(&product, sizeof(Product), 1, file) == 1) {
            printf("ID: %d, Category ID: %d, Category Code: %s, Brand: %s, Price: %.2f\n",
                   product.product_id, product.category_id, product.category_code, product.brand, product.price);
        }
        fclose(file);
    } else {
        perror("Erro ao abrir arquivo de produtos");
    }
}

void load_products_from_csv(const char *csv_filename, const char *binary_filename) {
    FILE *csv_file = fopen(csv_filename, "r");
    if (!csv_file) {
        perror("Erro ao abrir arquivo CSV");
        return;
    }

    FILE *bin_file = fopen(binary_filename, "wb");
    if (!bin_file) {
        perror("Erro ao abrir arquivo binário");
        fclose(csv_file);
        return;
    }

    char line[256];
    Product p;

    // Ignorar a primeira linha do cabeçalho
    fgets(line, sizeof(line), csv_file);

    while (fgets(line, sizeof(line), csv_file)) {
        sscanf(line, "%*[^,],%*[^,],%d,%d,%20[^,],%20[^,],%f",
               &p.product_id, &p.category_id, p.category_code, p.brand, &p.price);
        fwrite(&p, sizeof(Product), 1, bin_file);
    }

    fclose(csv_file);
    fclose(bin_file);
}
