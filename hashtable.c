#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashtable.h"

char *find_value(const hashtable *t, const char *key)
{
    unsigned i = find_slot(t, key);

    if (strcmp(t->arr[i].key, "") != 0)
        return(t->arr[i].val);
    else
        return("not found");
}

float load_factor(const hashtable *t)
{
    return(t->k / (float) t->n);
}

unsigned find_slot(const hashtable *t, const char *key)
{
    unsigned i = hash(key) % t->n;

    /* Search until we either find the key, or find an empty slot. */
    while (strcmp(t->arr[i].key, "") != 0 && strcmp(t->arr[i].key, key) != 0)
        i = (i + 1) % t->n;
    return(i);
}

/* FNV-1 32 bit hash. */
unsigned hash(const char *key)
{
    unsigned h = 2166136261UL, i;

    for (i = 0; i < strlen(key); ++i) {
        h *= 16777619UL;
        h ^= key[i];
    }
    return(h);
}

unsigned is_prime(unsigned n)
{
    unsigned i;

    if (n < 2)
        return(0);
    else if (n < 4)
        return(1);
    else if (n % 2 == 0 || n % 3 == 0)
        return(0);
    else {
        i = 5;
        while (i <= sqrt(n)) {
            if (n % i == 0 || n % (i + 2) == 0)
                return(0);
            i += 6;
        }
    }
    return(1);
}

unsigned next_prime(unsigned n)
{
    if (n != 2 && n % 2 == 0)
        ++n;
    while (!is_prime(n))
        n += 2;
    return(n);
}

unsigned process_keys(hashtable *t, const char *filename)
{
    unsigned count = 0;
    char key[256];
    char *value;
    FILE *fp = fopen(filename, "r");

    fgets(key, sizeof(key), fp);
    while (!feof(fp)) {
        key[strlen(key) - 1] = '\0';
        value = find_value(t, key);
        if (strcmp(value, "not found") != 0)
            ++count;
        fgets(key, sizeof(key), fp);
    }
    fclose(fp);
    return(count);
}

void add_record(hashtable *t, const char *key, const char *val)
{
    unsigned i = find_slot(t, key);

    if (strcmp(t->arr[i].key, "") != 0) { /* we found our key */
        strcpy(t->arr[i].val, val);
        return;
    }
    if (load_factor(t) >= 0.7F) {
        rebuild_table(t);
        i = find_slot(t, key);
    }
    ++(t->k);
    assign_slot(&t->arr[i], key, val);
}

void assign_slot(slot *s, const char *key, const char *val)
{
    delete_slot(s);
    s->key = (char *) malloc((strlen(key) + 1) * sizeof(char));
    s->val = (char *) malloc((strlen(val) + 1) * sizeof(char));
    strcpy(s->key, key);
    strcpy(s->val, val);
}

void delete_slot(slot *s)
{
    free(s->key);
    free(s->val);
}

void delete_table(hashtable *t)
{
    int i;

    for (i = t->n - 1; i >= 0; --i)
        delete_slot(&t->arr[i]);
    free(t->arr);
    t->n = 0;
    t->k = 0;
}

void init_slot(slot *s)
{
    s->key = (char *) malloc(sizeof(char));
    s->val = (char *) malloc(sizeof(char));
    s->key[0] = '\0';
    s->val[0] = '\0';
}

void init_table(hashtable *t)
{
    unsigned i;

    t->k = 0;
    t->n = 5;
    t->arr = (slot *) malloc(t->n * sizeof(slot));
    for (i = 0; i < t->n; ++i)
        init_slot(&t->arr[i]);
}

void print_table(const hashtable *t)
{
    unsigned i, n = (t->n <= 50) ? t->n : 50;

    for (i = 0; i < n; ++i) {
        printf("%d ", i);
        if (strcmp(t->arr[i].key, "") != 0)
            printf("%s: %s", t->arr[i].key, t->arr[i].val);
        putchar('\n');
    }
    printf("k = %d, n = %d, load = %f\n", t->k, t->n, load_factor(t));
}

void read_records(hashtable *t, const char *filename)
{
    char record[256];
    char *key, *value;
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL)
        return;
    fgets(record, sizeof(record), fp);
    while (!feof(fp)) {
        key = strtok(record, "\t");
        value = strtok(NULL, "\n");
        add_record(t, key, value);
        fgets(record, sizeof(record), fp);
    }
    fclose(fp);
}

void rebuild_table(hashtable *t)
{
    unsigned i;
    hashtable *newtab = (hashtable *) malloc(sizeof(hashtable));

    newtab->k = 0;
    newtab->n = next_prime(2 * t->n);

    newtab->arr = (slot *) malloc(newtab->n * sizeof(slot));
    for (i = 0; i < newtab->n; ++i)
        init_slot(&newtab->arr[i]);

    for (i = 0; i < t->n; ++i)
        if (strcmp(t->arr[i].key, "") != 0)
            add_record(newtab, t->arr[i].key, t->arr[i].val);

    delete_table(t);

    t->k = newtab->k;
    t->n = newtab->n;
    t->arr = newtab->arr;
    free(newtab);
}

void write_table(hashtable *t, char *filename)
{
    unsigned i;
    FILE *fp;

    fp = fopen(filename, "w");
    for (i = 0; i < t->n; ++i)
        if (strcmp(t->arr[i].key, "") != 0)
            fprintf(fp, "%s\t%s\n", t->arr[i].key, t->arr[i].val);
    fclose(fp);
    printf("data written to file %s\n", filename);
}
