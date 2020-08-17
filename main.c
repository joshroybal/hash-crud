#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"

char printmenu();
void stripnl(char *);

int main(int argc, char *argv[])
{
   char ans;
   char key[80], value[80];
   struct hash_table table = initialize();
   FILE *fp;

   if (argc > 1) {
      if ((fp = fopen(argv[1], "r"))) {
         while (fscanf(fp, " %[^\t] %[^\n]", key, value) != EOF)
            insert(&table, key, value);
         fclose(fp);
      }
      else {
         fprintf(stderr, "error opening file %s\n", argv[1]);
         dealloc(&table);
         return(1);
      }
   }
   else {
      if ((fp = fopen("/mnt/data/table.dat", "rb")) != (FILE *) NULL)
         read_table(&table, fp);
         fclose(fp);
   }

   while ((ans = printmenu()) != 'E') {
		memset(key, 0, sizeof(key));
   	memset(value, 0, sizeof(value));
      printf(" key: ");
      fgets(key, sizeof(key), stdin);
      stripnl(key);
      if (ans == 'C') {
         while (strlen(key)) {
            printf(" value: ");
            fgets(value, sizeof(value), stdin);
            stripnl(value);
            insert(&table, key, value);
            printf(" key: ");
            fgets(key, sizeof(key), stdin);
            stripnl(key);
         }
      }
      else if (ans == 'R') {
         while (strlen(key)) {
            printf(" value: %s\n", search(&table, key));
            printf(" key: ");
            fgets(key, sizeof(key), stdin);
            stripnl(key);
         }
      }
      else if (ans == 'U') {
         while (strlen(key)) {
            printf(" value: ");
            fgets(value, sizeof(value), stdin);
            stripnl(value);
            update(&table, key, value);
            printf(" key: ");
            fgets(key, sizeof(key), stdin);
            stripnl(key);
         }    
      } 
      else if (ans == 'D') {
         while (strlen(key)) {
            printf(" value: %s\n", search(&table, key));
            delkey(&table, key);
            printf(" key: ");
            fgets(key, sizeof(key), stdin);
            stripnl(key);
         }       
      }      
   }
   
   fp = fopen("/mnt/data/table.dat", "wb");
   write_table(&table, fp);
   fclose(fp);
	dealloc(&table);
	return(0);
}

char printmenu()
{
   char ch;
   char ans[25];

   do {
      printf(" (c)reate records\n");
      printf(" (r)etrieve records\n");
      printf(" (u)pdate records\n");
      printf(" (d)elete records\n");
      printf(" (e)xit\n");
      printf(" -> ");
      memset(ans, 0, sizeof(ans));
      fgets(ans, sizeof(ans), stdin);
      ch = toupper(ans[0]);
   } while (ch != 'C' && ch != 'R' && ch != 'U' && ch != 'D' && ch != 'E');
   return(ch);
}

void stripnl(char *str)
{
   int n = strlen(str);

   if (str[n - 2] == '\r' && str[n - 1] == '\n') {
      str[n - 2] = '\0';
      return;
   }
   if (str[n - 1] == '\n')
      str[n - 1] = '\0';
   return;
}
