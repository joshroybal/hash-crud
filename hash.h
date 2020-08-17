#ifndef HASH_H
#define HASH_H

struct bucket {
   char *key;
   char *value;
   struct bucket *next;
};

struct hash_table {
   struct bucket **buckets;
   int n;   /* no. of buckets */
   int k;   /* no. of keys    */
};

struct hash_table initialize();
struct hash_table rebuild(struct hash_table *);
struct hash_table shrink(struct hash_table *);
void dealloc(struct hash_table *);
unsigned int hash(char *);
void insert(struct hash_table *, char *, char *);
void update(struct hash_table *, char *, char *);
void delkey(struct hash_table *, char *);
char *search(struct hash_table *, char *);
void write_table(const struct hash_table *, FILE *);
void read_table(struct hash_table *, FILE *);

#endif
