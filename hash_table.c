#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"
#include "prime.h"

#define HT_PRIME_1 137
#define HT_PRIME_2 389
#define HT_DEFAULT_BASE_SIZE 50

static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item* init_ht_item(const char* k, const char* v) {
    ht_item* new_item = malloc(sizeof(ht_item));

    new_item->key = strdup(k);
    new_item->value = strdup(v);

    return new_item;
}

static ht_hash_table* create_hash_table_new(const int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->base_size = base_size

    ht->count = next_prime(base_size);
    ht->size = 20;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

    return ht;
}

ht_hash_table* create_hash_table() {
    return create_hash_table_new(HT_DEFAULT_BASE_SIZE);
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
    if ( base_size < HT_DEFAULT_BASE_SIZE )
        return;

    ht_hash_table* new_ht = create_hash_table_new(base_size);
    for ( int i=0; i<ht->size; ++i ) {
        ht_item* item = ht->items[i];
        if ( item != NULL && item != &HT_DELETED_ITEM ) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    destroy_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
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

static long m_pow(long base, long power) {
    int result = 1;
    while ( power ) {
        if ( power % 2 == 0 ) {
            power = power / 2;
            base = base * base;
        }
        else {
            result = result * base;
            power--;
        }
    }
    return result;
}

static int hash_function(const char* str, const int a, const int m) {
    long hash = 0;
    const int length = strlen(str);
    for ( int i=0; i<length; ++i ) {
        hash += (long) m_pow(a, length - (i+1) * str[i]);
        hash = hash % m;
    }
    return (int)hash;
}


static int get_hash(const char* str, const int size, const int attempt) {
    const int hash_a = hash_function(str, HT_PRIME_1, size);
    const int hash_b = hash_function(str, HT_PRIME_2, size);

    return (hash_a + (attempt * (hash_b + 1))) % size;
}

void ht_insert(ht_hash_table* ht, const char* key, const char* val) {
    cosnt int load = ht->count * 100 / ht->size;
    if ( load > 70 ) {
        ht_resize_up(ht);
    }
    ht_item* ht_item = init_ht_item(key, val);
    int index = get_hash(ht_item->key, ht->size, 0);
    ht_item* curr_item = ht->items[index];

    int i = 1;
    while ( curr_item != NULL ) {
        if ( curr_item != &HT_DELETED_ITEM ) {
            if ( strcmp(curr_item->key, key) == 0 ) {
                destroy_ht_item(curr_item);
                ht->items[index] = item;
                return;
            }
        }
        index = get_hash(ht_item->key, ht->size, i);
        curr_item = ht->items[index];
        i++;
    }
    ht->items[index] = ht_item;
    ht->count++;
}

char* ht_get(ht_hash_table* ht, const char* key) {
    int index = get_hash(key, ht->size, 0);
    ht_item* curr_item = ht->items[index];
    int i = 1;
    while ( curr_item != NULL ) {
        if ( curr_item != &HT_DELETED_ITEM ) {
            if ( strcmp(curr_item->key, key) == 0 ) {
                return curr_item->value;
            }
        }
        index = get_hash(key, ht->size, i);
        curr_item = ht->items[index];
        i++;
    }
    return NULL;
}

void ht_remove(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if ( load < 70 ) {
        ht_resize_down(ht);
    }
    int index = get_hash(key, ht->size, 0);
    ht_item* curr_item = ht->items[index];
    
    int i = 1;
    while ( curr_item != NULL ) {
        if ( curr_item != &HT_DELETED_ITEM ) {
            if ( strcmp(curr_item->key, key) == 0 ) {
                destroy_ht_item(curr_item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = get_hash(key, ht->size, i);
        curr_item = ht->items[index];
        i++;
    }
    ht->count--;
}