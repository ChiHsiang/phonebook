#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#ifdef OPT
#define OUT_FILE "opt.txt"
#elif HASH
#define OUT_FILE "hash.txt"
#else
#define OUT_FILE "orig.txt"
#endif

#define DICT_FILE "./dictionary/words.txt"
#define SLOT_FILE "slots.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

#ifdef HASH
    static int slot[TABLE_SIZE];
#endif

    /* build the entry */
#if defined HASH
    entry *pHead, *e[TABLE_SIZE];
    pHead = (entry *) malloc(sizeof(entry) * TABLE_SIZE);
    printf("size of entry :%lu bytes\n", sizeof(entry));
    for (i = 0; i < TABLE_SIZE; i++) {
        e[i] = &pHead[i];
        e[i]->pNext = NULL;
    }
    i = 0;
#else
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry :%lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif

    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;

#if defined HASH
        unsigned int hash_value = BKDRHash(line);
        slot[hash_value]++;
        e[hash_value] = append(line, e[hash_value]);
#else
        e = append(line, e);
#endif
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

#if defined HASH
    for (int i = 0; i < TABLE_SIZE; i++) {
        e[i] = &pHead[i];
    }
#else
    e = pHead;
#endif

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    //e = pHead;

    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

#ifdef HASH
    for (i = 0; i < TABLE_SIZE; i++) {
        if(e[i])
            free_list(e[i]->pNext);
        free(pHead);
    }
#else
    entry *tmp;
    while ((tmp = pHead) != NULL) {
        pHead = pHead->pNext;
        free(tmp);
    }
#endif

#ifdef HASH
    FILE *slot_file = fopen(SLOT_FILE, "a");
    for (i = 0; i < TABLE_SIZE; i++) {
        fprintf(slot_file, "%d %d\n", i, slot[i]);
    }
    fclose(slot_file);
#endif

    return 0;
}
