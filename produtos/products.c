#include "products.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void load_products_from_csv(const char *csv_filename, const char *binary_filename) {
    FILE *csv_file = fopen(csv_filename, "r");
    if (csv_file == NULL) {
        printf("Erro ao abrir arquivo CSV de produtos: %s\n", csv_filename);
        return;
    }

    FILE *bin_file = fopen(binary_filename, "wb");
    if (bin_file == NULL) {
        printf("Erro ao criar arquivo binário de produtos: %s\n", binary_filename);
        fclose(csv_file);
        return;
    }

    Product *products = (Product *)malloc(sizeof(Product) * 1000); // Supondo que temos até 1000 produtos
    int count = 0;
    char line[512];
    fgets(line, sizeof(line), csv_file); // Ignorar cabeçalho

    while (fgets(line, sizeof(line), csv_file)) {
        Product product;
        char *token = strtok(line, ",");
        product.product_id = strtoull(token, NULL, 10);

        token = strtok(NULL, ",");
        strncpy(product.category_code, token, 20);
        product.category_code[20] = '\0'; // Garantir null-terminator

        token = strtok(NULL, ",");
        strncpy(product.brand, token, 50);
        product.brand[50] = '\0'; // Garantir null-terminator

        token = strtok(NULL, ",");
        product.price = atof(token);

        products[count++] = product; // Adiciona o produto ao array
    }

    // Ordenar os produtos por product_id (Bubble Sort, por simplicidade)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (products[j].product_id > products[j + 1].product_id) {
                Product temp = products[j];
                products[j] = products[j + 1];
                products[j + 1] = temp;
            }
        }
    }

    fwrite(products, sizeof(Product), count, bin_file);
    fclose(csv_file);
    fclose(bin_file);
    free(products);
}

void insert_product(const char *filename, Product product) {
    FILE *file = fopen(filename, "ab");
    if (file) {
        fwrite(&product, sizeof(Product), 1, file);
        fclose(file);
    } else {
        perror("Erro ao abrir arquivo de produtos");
    }
}

void show_products(const char *bin_filename) {
    FILE *bin_file = fopen(bin_filename, "rb");
    if (bin_file == NULL) {
        printf("Erro ao abrir arquivo binário de produtos: %s\n", bin_filename);
        return;
    }

    Product product;
    while (fread(&product, sizeof(Product), 1, bin_file)) {
        printf("ID: %llu, Categoria: %s, Marca: %s, Preço: %.2f\n",
               product.product_id, product.category_code, product.brand, product.price);
    }

    fclose(bin_file);
}

Product* search_product(const char *bin_filename, unsigned long long product_id) {
    FILE *bin_file = fopen(bin_filename, "rb");
    if (bin_file == NULL) {
        printf("Erro ao abrir arquivo binário de produtos: %s\n", bin_filename);
        return NULL;
    }

    Product *product = (Product *)malloc(sizeof(Product));
    while (fread(product, sizeof(Product), 1, bin_file)) {
        if (product->product_id == product_id) {
            fclose(bin_file);
            return product;
        }
    }

    free(product);
    fclose(bin_file);
    return NULL;
}

void remove_product(const char *filename, unsigned long long product_id) {
    FILE *file = fopen(filename, "rb");
    FILE *temp_file = fopen("temp.bin", "wb");
    if (!file || !temp_file) {
        perror("Erro ao abrir arquivo");
        return;
    }

    Product product;
    int found = 0;

    while (fread(&product, sizeof(Product), 1, file)) {
        if (product.product_id != product_id) {
            fwrite(&product, sizeof(Product), 1, temp_file);
        } else {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove(filename);
        rename("temp.bin", filename);
    } else {
        printf("Produto com ID %llu não encontrado.\n", product_id);
        remove("temp.bin");
    }
}
