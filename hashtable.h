/* Open addressing and linear probing hash table implementation. */
#ifndef HASHTABLE_H
#define HASHTABLE_H

struct slot {
    char *key, *val;
};
typedef struct slot slot;

struct hashtable {
    unsigned k, n;
    slot *arr;
};
typedef struct hashtable hashtable;

char *find_value(const hashtable *, const char *);
float load_factor(const hashtable *);
unsigned find_slot(const hashtable *, const char *);
unsigned hash(const char *);
unsigned is_prime(unsigned);
unsigned next_prime(unsigned);
unsigned process_keys(hashtable *, const char *);

void add_record(hashtable *, const char *, const char *);
void delete_record(hashtable *, const char *, const char *);
void assign_slot(slot *, const char *, const char *);
void delete_slot(slot *);
void delete_table(hashtable *);
void init_slot(slot *);
void init_table(hashtable *);
void print_table(const hashtable *);
void read_records(hashtable *, const char *);
void rebuild_table(hashtable *);
void write_table(hashtable *, char *);

#endif
