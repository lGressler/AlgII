# Projeto: Organização de Arquivos Sequencial-Indexado em C

## Explicação do menu do código: 

Opção 1: Processa todos os arquivos CSV na pasta datasets e carrega os dados para arquivos binários de produtos e acessos.
Opção 2: Exibe todos os produtos presentes no arquivo binário products.bin.
Opção 3: Exibe todos os acessos presentes no arquivo binário accesses.bin.
Opção 4: Permite inserir manualmente um novo produto no arquivo binário products.bin.
Opção 5: Permite inserir manualmente um novo acesso no arquivo binário accesses.bin.
Opção 6: Busca um produto específico pelo product_id e exibe suas informações.
Opção 7: Busca um acesso pelo user_id e exibe os dados relacionados.
Opção 8: Remove um produto específico baseado no product_id.
Opção 9: Remove um acesso específico baseado no user_id.
Opção 0: Sai do programa.

## Definição do contexto a ser explorado:

O contexto explorado neste projeto refere-se ao comportamento do usuário em um e-commerce de múltiplas categorias, com dados obtidos de um dataset do Kaggle: [eCommerce Behavior Data from Multi-Category Store](https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-data-from-multi-category-store). Este dataset contém informações de eventos como compras e visualizações de produtos, bem como detalhes sobre os usuários e produtos.

**Implementação:**
- O dataset foi baixado manualmente e colocado na pasta `datasets` do projeto. Os arquivos CSV representam os dados de comportamento no e-commerce para os meses de outubro a abril.

---

## Montagem dos arquivos de dados:

O objetivo aqui é montar dois arquivos principais a partir dos CSVs:

- **Arquivo de produtos**: com campos como `product_id`, `brand`, e `price`.
- **Arquivo de acessos**: com campos como `user_id`, `user_session`, e `event_type`.

### Tarefas Implementadas:

1. **Escolha dos arquivos**: Os CSVs baixados (7 meses) foram nomeados como `2019-Oct.csv`, `2019-Nov.csv`, `2019-Dec.csv.gz`, `2020-Jan.csv.gz`, `2020-Feb.csv.gz`, `2020-Mar.csv.gz`, e `2020-Apr.csv.gz`.
2. **Campos no arquivo**: Cada arquivo de produtos contém os seguintes campos:
   - **Campo chave**: `product_id` (identificador único).
   - **Campo repetido**: `brand` (marcas podem ser repetidas).
   - Outro campo relevante: `price`.

   Para acessos:
   - **Campo chave**: `user_id`.
   - **Campo repetido**: `event_type` (tipo de ação como compra ou visualização).
   - Outro campo relevante: `user_session`.

3. **Consultas definidas**:
   - Qual é a marca mais comprada?
   - Qual é o produto mais caro?
   - Quais usuários mais realizaram compras?

4. **Ordenação e modo binário**: Os dados são organizados por chave (ex.: `product_id` ou `user_id`) e salvos em arquivos binários para leitura mais eficiente.

---

## Organização e registros dos arquivos de dados:

Os registros precisam ser de tamanho fixo, e as operações de leitura devem ser feitas utilizando o comando `seek` para navegar nos arquivos.

### Funções Implementadas:

1. **Função para inserir os dados**: As funções `load_products_from_csv` e `load_accesses_from_csv` são responsáveis por ler os arquivos CSV, ordenar os registros por chave e inseri-los nos arquivos binários. A ordenação é realizada internamente pela ordem natural das chaves.
2. **Função para mostrar os dados**: As funções `show_products` e `show_accesses` exibem os dados presentes nos arquivos binários, carregando-os de volta para a memória.
3. **Função de pesquisa binária**: Implementada em `search_product_by_id` e `search_access_by_user_id` para buscar registros nos arquivos de forma eficiente.
4. **Consulta com pesquisa binária**: A função `search_product_by_id` usa pesquisa binária para localizar um produto específico pelo `product_id`.

---

## Índices em arquivo:

O índice parcial foi criado para os campos chave (`product_id` e `user_id`). Esses índices são usados para acelerar a busca de registros nos arquivos de dados, permitindo uma pesquisa mais eficiente através de binários, em vez de uma varredura completa.

### Funções Implementadas:

1. **Arquivo de índice parcial**: A função `create_index` cria um índice parcial, armazenando chaves e as posições dos registros no arquivo de dados.
2. **Consulta usando o índice**: A função `search_product_in_index` pesquisa o índice para encontrar a posição do registro desejado, e em seguida utiliza o comando `seek` para buscar diretamente no arquivo de dados.

---

## Inserção/remoção de dados e reconstrução do índice:

Para suportar a inserção e remoção de dados nos arquivos, o índice precisa ser reconstruído após cada alteração significativa. Isso garante que o índice reflita corretamente as mudanças nos dados subjacentes.

### Funções Implementadas:

1. **Inserção de um novo registro**: A função `insert_product` é responsável por adicionar um novo registro ao arquivo de produtos, inserindo-o de forma ordenada e, em seguida, atualizando o índice com a nova chave.
2. **Remoção de um registro**: A função `remove_product` remove um registro baseado no `product_id`. Após a remoção, o índice é reconstruído para refletir a ausência do registro.
3. **Reconstrução do índice**: O índice é reconstruído a cada inserção ou remoção de registro para garantir sua precisão, utilizando a função `create_index` novamente.

---

## Função `process_all_files`:

Esta função é responsável por buscar todos os arquivos `.csv` na pasta `datasets`, carregar os produtos e acessos a partir desses arquivos, e exibir os dados processados.

```c
void process_all_files() {
    WIN32_FIND_DATA find_file_data;
    HANDLE h_find = INVALID_HANDLE_VALUE;
    char search_path[256];

    snprintf(search_path, sizeof(search_path), "%s*%s", DATASET_FOLDER, CSV_EXTENSION);

    h_find = FindFirstFile(search_path, &find_file_data);
    if (h_find == INVALID_HANDLE_VALUE) {
        printf("Erro ao abrir a pasta datasets.\n");
        return;
    }

    do {
        if (!(find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char filepath[256];
            snprintf(filepath, sizeof(filepath), "%s%s", DATASET_FOLDER, find_file_data.cFileName);

            char products_bin[256], accesses_bin[256];
            snprintf(products_bin, sizeof(products_bin), "%s_products.bin", find_file_data.cFileName);
            snprintf(accesses_bin, sizeof(accesses_bin), "%s_accesses.bin", find_file_data.cFileName);

            printf("Processando arquivo: %s\n", filepath);

            load_products_from_csv(filepath, products_bin);
            load_accesses_from_csv(filepath, accesses_bin);

            show_products(products_bin);
            show_accesses(accesses_bin);
        }
    } while (FindNextFile(h_find, &find_file_data) != 0);

    FindClose(h_find);
}
```

## Conclusão:

O projeto foi organizado para processar os arquivos de dados do e-commerce, criar arquivos binários para armazenamento eficiente, e utilizar índices para acelerar as buscas. Todas as funcionalidades descritas no enunciado foram implementadas, incluindo a inserção e remoção de registros e a reconstrução de índices, garantindo a eficiência na consulta de dados.