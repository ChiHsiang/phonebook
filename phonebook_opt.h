#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define OPT 1

#define MAX_LAST_NAME_SIZE 16

/* TODO: After modifying the original version, uncomment the following
 * line to set OPT properly */
// #define OPT 1
typedef struct __PHONE_BOOK_ENTRY {
    struct __PHONE_BOOK_ENTRY *pNext;
    char lastName[MAX_LAST_NAME_SIZE];
} entry;

entry *findName(char lastName[], entry *pHead);
entry *append(char lastName[], entry *e);

#endif
