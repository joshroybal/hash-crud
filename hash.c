#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

struct hash_table initialize()
{
   int i;
   struct hash_table t;

   t.n = 2;
   t.k = 0;
   t.buckets = (struct bucket **) malloc(t.n * sizeof(struct bucket *));
   for (i = 0; i < t.n; ++i)
      t.buckets[i] = (struct bucket *) NULL;
   return(t);
}

struct hash_table rebuild(struct hash_table *t)
{
   struct hash_table copy;
   struct bucket *curr;
   int i;

   copy.n = 2 * t->n;
   copy.k = 0;
   copy.buckets = (struct bucket **) malloc(copy.n * sizeof(struct bucket *));
   for (i = 0; i < copy.n; ++i)
      copy.buckets[i] = (struct bucket *) NULL;
   
   for (i = 0; i < t->n; ++i) {
      if (t->buckets[i]) {
         insert(&copy, t->buckets[i]->key, t->buckets[i]->value);
         curr = t->buckets[i];
         while (curr->next) {
            insert(&copy, curr->next->key, curr->next->value);
            curr = curr->next;
         }
      }
   }
   dealloc(t);
   return(copy);
}

struct hash_table shrink(struct hash_table *t)
{
   int i;
   struct hash_table copy;
   struct bucket *curr;

   copy.n = t->n/2;
   copy.k = 0;
   copy.buckets = (struct bucket **) malloc(copy.n * sizeof(struct bucket *));
   for (i = 0; i < copy.n; ++i)
      copy.buckets[i] = (struct bucket *) NULL;
   
   for (i = 0; i < t->n; ++i) {
      if (t->buckets[i]) {
         insert(&copy, t->buckets[i]->key, t->buckets[i]->value);
         curr = t->buckets[i];
         while (curr->next) {
            insert(&copy, curr->next->key, curr->next->value);
            curr = curr->next;
         }
      }
   }
   dealloc(t);
   return(copy);   
}

void dealloc(struct hash_table *t)
{
   int i;
   struct bucket *curr, *tag;

   for (i = 0; i < t->n; ++i) {
      if (t->buckets[i]) {
         curr = t->buckets[i];
         while (curr) {
            tag = curr;
            curr = curr->next;
            free(tag->key);
            free(tag->value);
            free(tag);
         }
      }
   }
   free(t->buckets);
}

unsigned int hash(char *key)
{
   int i;
   unsigned int h = 0;

   for (i = 0; i < strlen(key); ++i)
      h = h*31+key[i];
   return(h);
}

void insert(struct hash_table *t, char *key, char *value)
{
   unsigned int idx;
   struct bucket *curr;

   if (strlen(search(t, key)))   /* no duplicate keys */
      return;

   if (t->k == t->n) /* about to insert into full table, so rebuild  */
      *t = rebuild(t);

   idx = hash(key) % t->n;

   if (t->buckets[idx] == (struct bucket *) NULL) {
      t->buckets[idx] = (struct bucket *) malloc(sizeof(struct bucket));
      t->buckets[idx]->key = (char *) malloc(strlen(key)+1);
      t->buckets[idx]->value = (char *) malloc(strlen(value)+1);
      strcpy(t->buckets[idx]->key, key);
      strcpy(t->buckets[idx]->value, value);
      t->buckets[idx]->next = (struct bucket *) NULL;
   }
   else {
      curr = t->buckets[idx];
      while (curr->next)
         curr = curr->next;
      curr->next = (struct bucket *) malloc(sizeof(struct bucket));
      curr->next->key = (char *) malloc(strlen(key)+1);
      curr->next->value = (char *) malloc(strlen(value)+1);
      strcpy(curr->next->key, key);
      strcpy(curr->next->value, value);
      curr->next->next = (struct bucket *) NULL;
   }

   ++(t->k);
}

char *search(struct hash_table *t, char *key)
{
   struct bucket *curr = t->buckets[hash(key) % t->n];

   while (curr) {
      if (strcmp(curr->key, key) == 0)
         return(curr->value);
      curr = curr->next;
   }
   return("");
}

void update(struct hash_table *t, char *key, char *value)
{
   unsigned int idx = hash(key)%t->n;
   struct bucket *curr;

   if (t->buckets[idx] == (struct bucket *) NULL) {
      t->buckets[idx] = (struct bucket *) malloc(sizeof(struct bucket));
      t->buckets[idx]->key = (char *) malloc(strlen(key)+1);
      t->buckets[idx]->value = (char *) malloc(strlen(value)+1);      
      strcpy(t->buckets[idx]->key, key);
      strcpy(t->buckets[idx]->value, value);
      t->buckets[idx]->next = (struct bucket *) NULL;
   }
   else {   /* no duplicate keys */
      curr = t->buckets[idx];
      while (strcmp(curr->key, key) != 0 && curr->next)
         curr = curr->next;
      if (strcmp(curr->key, key) == 0)
         strcpy(curr->value, value);
   }
}

void delkey(struct hash_table *t, char *k)
{
   unsigned int idx;
   struct bucket *curr, *tag;

   if (t->k == t->n/4)
      *t = shrink(t);
   
   idx = hash(k) % t->n;
   
   if (t->buckets[idx] == (struct bucket *) NULL)
      return;

   if (strcmp(t->buckets[idx]->key, k) == 0) {
      tag = t->buckets[idx];
      t->buckets[idx] = t->buckets[idx]->next;
      free(tag->key);
      free(tag->value);
      free(tag);
      --(t->k);
      return;
   }

   curr = t->buckets[idx];
   while (curr->next) {
      if (strcmp(curr->next->key, k) == 0) {
         tag = curr->next;
         if (t->buckets[idx] == tag)
            t->buckets[idx] = (struct bucket *) NULL;
         curr->next = curr->next->next;
         free(tag->key);
         free(tag->value);
         free(tag);
         --(t->k);
         return;
      }
      curr = curr->next;
   }
}

void write_table(const struct hash_table *t, FILE *fp)
{
   int i;
   char record[80];
   struct bucket *curr;

   for (i = 0; i < t->n; ++i) {
      curr = t->buckets[i];
      if (curr) {
         while (curr) {
            memset(record, 0, sizeof(record));
            memcpy(record, curr->key, strlen(curr->key)+1);
            memcpy(record+strlen(record)+1, curr->value, strlen(curr->value)+1);
            fwrite(record, sizeof(record), 1, fp);
            curr = curr->next;
         }
      }
   }
}

void read_table(struct hash_table *t, FILE *fp)
{
   char record[80];

   while (!feof(fp)) {
      fread(record, sizeof(record), 1, fp);
      insert(t, record, record+strlen(record)+1);
   }
}
