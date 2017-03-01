#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define HASH 1
#define TABLE_SIZE 3571

/* original version */
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

entry *findName(char lastName[], entry *pHead[]);
entry *append(char lastName[], entry *e);

unsigned int BKDRhash_v2(char *str, unsigned int len);
unsigned int BKDRhash_v1(char *str);
unsigned int BKDRHash(char lastName[]);

void free_list(entry *e);

#endif
