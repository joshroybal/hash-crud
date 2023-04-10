#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

#define FLDSIZ 256

char printmenu();
void stripnl(char *);

int main()
{
    char ans;
    char key[FLDSIZ], value[FLDSIZ];
    char *target;
    unsigned alt = 0;
    hashtable table;

    init_table(&table);
    puts("reading records");
    read_records(&table, "table.tab");
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
                add_record(&table, key, value);
                alt = 1;
                printf(" key: ");
                fgets(key, sizeof(key), stdin);
                stripnl(key);
            }
        }
        else if (ans == 'R') {
            while (strlen(key)) {
                target = find_value(&table, key);
                printf(" value: %s\n", target);
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
                add_record(&table, key, value);
                alt = 1;
                printf(" key: ");
                fgets(key, sizeof(key), stdin);
                stripnl(key);
            }
        }
        else if (ans == 'D') {
            while (strlen(key)) {
                puts("not implemented");
                break;
                /* delete_key(&table, key); */
                alt = 1;
                printf(" key: ");
                fgets(key, sizeof(key), stdin);
                stripnl(key);
            }
        }
    }

    /* print_table(&table); */
    if (alt)
        write_table(&table, "table.tab");
    delete_table(&table);
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
