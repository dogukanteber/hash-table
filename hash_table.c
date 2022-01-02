#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

static ht_item* init_ht_item(const char* k, const char* v) {
    ht_item* new_item = malloc(sizeof(ht_item));

    new_item->key = strdup(k);
    new_item->value = strdup(v);

    return new_item;
}

ht_hash_table* create_hash_table() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->count = 0;
    ht->size = 20;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

    return ht;
}

static void destroy_ht_item(ht_item* hti) {
    free(hti->key);
    free(hti->value);
    free(hti);
}

void destroy_hash_table(ht_hash_table* ht) {
    for ( int i=0; i<ht->size; ++i ) {
        if ( ht->items[i] ) {
            destroy_ht_item(ht->items[i]);
        }
    }
    free(ht->items);
    free(ht);
}

static long pow(long a, long b) {
    // TODO:: in O(logn) time complexity
}

static int hash_function(const char* str, const int a, const int m) {
    long hash = 0;
    const int length = strlen(str);
    for ( int i=0; i<length; ++i ) {
        hash += (long) pow(a, length - (i+1) * str[i]);
        hash = hash % m;
    }
    return (int)hash;
}

// what is HT_PRIME_1??
static int get_hash(const char* str, const int size, const int attempt) {
    const int hash_a = hash_function(str, HT_PRIME_1, size);
    const int hash_b = hash_function(str, HT_PRIME_2, size);

    return (hash_a + (attempt * (hash_b + 1))) % size;
}