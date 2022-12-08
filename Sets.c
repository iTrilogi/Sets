#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "S_LinkedList.h"

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;
    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }
    return hash;
}

struct set_node
{
    const char *key;
    struct set_node *next;
    size_t key_len;
};
struct set_table
{
    struct set_node **nodes;
    size_t hashmap_size;
};

struct set_table *set_table_new(const size_t hashmap_size)
{
    struct set_table *table = malloc(sizeof(struct set_table));
    if (!table)
    {
        return NULL;
    }
    table->hashmap_size = hashmap_size;
    table->nodes = calloc(table->hashmap_size, sizeof(struct set_node *));
    if (!table->nodes)
    {
        free(table);
        return NULL;
    }
    return table;
}
struct set_node *search_at_index(struct set_table *table, const char *key, const size_t key_len)
{
    size_t index = djb33x_hash(key, key_len) % table->hashmap_size;

    struct set_node *current_node = table->nodes[index];

    while (current_node)
    {
        if (current_node->key == key)
        {
            printf("Got you: %s \n ", current_node->key);
            return current_node;
        }
       
        current_node = current_node->next;
    }
    printf("Search not found\n");

    return current_node;
}

int *set_insert(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *current = table->nodes[index];
    struct set_node *c_table = table->nodes[index];

    while (c_table)
    {
        if (current->key == key)
        {
            return NULL;
        }
        c_table = c_table->next;
    }

    if (!table->nodes[index])
    {
        table->nodes[index] = (struct set_node *)addToEmpty((node_t *)table->nodes[index], (void *)key);
        table->nodes[index]->key_len = key_len;
    }
    else
    {
        current = (struct set_node *)add_at_end((node_t *)table->nodes[index], (void *)key);
        current->key_len = key_len;
        printf(" sono keyleng %zu \n", current->key_len);
    }

    return 0;
}

int remove_at_index(struct set_table *table, const char *key, const size_t key_len)
{
    size_t index = djb33x_hash(key, key_len) % table->hashmap_size;

    struct set_node *current_node = table->nodes[index];
    struct set_node *previous_node;
    int counter = 0;
    while (current_node)
    {
        if (current_node->key == key)
        {
            if (!current_node->next)
            {
                printf("Removed the last Key \"%s\" at index [%zu]\n", current_node->key, index);
                table->nodes[index] = (struct set_node *)del_last((node_t *)table->nodes[index]);
                return 0;
            }
            else if (!previous_node)
            {
                printf("Removed the first Key \"%s\" at index [%zu]\n", current_node->key, index);
                table->nodes[index] = (struct set_node *)del_first((node_t *)table->nodes[index]);
                return 0;
            }
            else
            {
                printf("Removed a mid Key \"%s\" at index [%zu]\n", current_node->key, index);
                table->nodes[index] = (struct set_node *)del_at_pos((node_t *)table->nodes[index], counter);
                return 0;
            }
        }
        counter++;
        previous_node = current_node;
        current_node = current_node->next;
    }
    printf("Remove not found\n");
    return -1;
}
void print(struct set_table *table)
{
    for (size_t i = 0; i < table->hashmap_size; i++)
    {
        struct set_node *current = table->nodes[i];
        while (current)
        {
            printf("index: %zu  %s\n", i, current->key);
            current = current->next;
        }
    }
}
int main(int argc, char const *argv[])
{
    int hashmap_size = 2;
    struct set_table *table = set_table_new(hashmap_size);
    set_insert(table, "testa", 5);
    set_insert(table, "testina", 7);
    set_insert(table, "testone", 7);
    set_insert(table, "gatto", 5);
    set_insert(table, "franco", 6);
    set_insert(table, "caldaia", 7);

    search_at_index(table, "testa", 5);
    search_at_index(table, "gatto", 5);
    search_at_index(table, "franco", 6);
    search_at_index(table, "asdasd", 7);

    // remove_at_index(table, "gatto", 5);

    print(table);

    return 0;
}
