#ifndef ACCESSES_H
#define ACCESSES_H

typedef struct {
    char event_time[20];
    char event_type[20];
    unsigned long long product_id;
    unsigned long long category_id;
    char category_code[50];
    char brand[50];
    float price;
    unsigned long long user_id;
    char user_session[50];
} Access;

// Funções
void load_accesses_from_csv(const char *csv_filename, const char *binary_filename);
void show_accesses(const char *bin_filename);
Access* search_access(const char *bin_filename, unsigned long long product_id);
void remove_access(const char *filename, unsigned long long product_id);

#endif
