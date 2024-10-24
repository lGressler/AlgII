#ifndef INDEX_H
#define INDEX_H

typedef struct {
    unsigned long long key;      // Campo chave (ID do produto)
    long offset;                 // Deslocamento no arquivo de dados
} IndexEntry;

void create_index(const char *data_filename, const char *index_filename);
IndexEntry* binary_search_index(const char *index_filename, unsigned long long target_key);
void search_with_index(const char *data_filename, const char *index_filename, unsigned long long target_key);
void remove_product(const char *data_filename, const char *index_filename, unsigned long long target_id);

#endif
