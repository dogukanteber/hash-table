typedef struct {
    char* key;
    char* value;   
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

void ht_insert(ht_hash_table* ht, const char* key, const char* val);
char* ht_get(ht_hash_table* ht, const char* key);
void ht_remove(ht_hash_table* ht, const char* key);
