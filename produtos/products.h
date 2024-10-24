// products.h
#ifndef PRODUCTS_H
#define PRODUCTS_H

typedef struct {
    int product_id;       // ID do produto
    int category_id;      // ID da categoria
    char category_code[21]; // Código da categoria
    char brand[21];       // Nome da marca
    float price;          // Preço do produto
} Product;

void insert_product(const char *filename, Product product);
void show_products(const char *filename);
void load_products_from_csv(const char *csv_filename, const char *binary_filename);

#endif
