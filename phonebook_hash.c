#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_hash.h"

entry *findName(char lastName[], entry *pHead[])
{
    unsigned int n = BKDRhash_v1(lastName);
    n %= TABLE_SIZE;
    entry *tmp;
    if (pHead[n]->pNext) {
        tmp = pHead[n]->pNext;
    } else {
        return NULL;
    }

    while (tmp) {
        if (strcasecmp(lastName, tmp->lastName) == 0) {
            return tmp;
        }
        tmp = tmp->pNext;
    }

    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

unsigned int BKDRhash_v2(char *str, unsigned int len)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    unsigned int i    = 0;

    for (i = 0; i < len; str++, i++) {
        hash = (hash * seed) + (*str);
    }

    return hash;
}

unsigned int BKDRhash_v1(char *str)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

unsigned int BKDRHash(char lastName[])
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    int i = 0;
    while (i < strlen(lastName)) {
        hash = hash * seed + lastName[i];
        i++;
    }
    return (hash & 0x7FFFFFFF) % TABLE_SIZE;
}

void free_list(entry *e)
{
    entry *tmp;
    while ((tmp = e) != NULL) {
        e = e->pNext;
        free(tmp);
    }
}
