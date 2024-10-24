#ifndef PRODUCTS_H
#define PRODUCTS_H

typedef struct {
    unsigned long long product_id;
    char category_code[21];
    char brand[51];
    float price;
} Product;

// Funções
void load_products_from_csv(const char *csv_filename, const char *binary_filename);
void insert_product(const char *filename, Product product);
void show_products(const char *bin_filename);
Product* search_product(const char *bin_filename, unsigned long long product_id);
void remove_product(const char *filename, unsigned long long product_id);

#endif
