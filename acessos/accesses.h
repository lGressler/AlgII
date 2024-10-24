#ifndef ACCESSES_H
#define ACCESSES_H

#include <stdio.h>

// Estrutura para armazenar os acessos do e-commerce com base nos eventos
typedef struct {
    char event_time[20];     // Tempo do evento
    char event_type[20];     // Tipo de evento (view, cart, remove_from_cart, purchase)
    unsigned long long product_id; // ID do produto
    unsigned long long category_id; // ID da categoria do produto
    char category_code[50];  // Código da categoria
    char brand[50];          // Marca do produto
    float price;             // Preço do produto
    unsigned long long user_id; // ID do usuário
    char user_session[50];   // Sessão do usuário
} Access;

// Funções atualizadas
void load_accesses_from_csv(const char *csv_filename, const char *bin_filename);
void show_accesses(const char *bin_filename);

#endif