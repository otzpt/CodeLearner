/*
 * TITLE: qsort(3), bsearch(3): sorting and searching an array
 * GROUP: Common tasks
 * USES: #include <stdlib.h>
 * SUMMARY: qsort sorts an array of anything, given a function that compares
 *   two elements. bsearch finds an element in an array that is already sorted.
 * SIGNATURE:
 *   void qsort(void *base, size_t count, size_t size, compare)
 *   void *bsearch(const void *key, const void *base, size_t count, size_t size, compare)
 *   int compare(const void *a, const void *b)
 * PARAMETERS:
 *   - base, count, size: the array, how many elements, and the size of one.
 *   - compare: gets pointers to two elements, and returns below zero, zero or
 *     above zero.
 *   - key: a pointer to the value to find.
 * RETURNS: qsort returns nothing, and the array is now in order. bsearch
 *   returns a pointer to a matching element, or NULL.
 * ERRORS:
 *   - Neither reports an error. A compare function that is not consistent
 *     gives an unspecified order.
 * NOTES:
 *   - compare receives pointers TO the elements. For an array of strings that
 *     is a pointer to a pointer, the usual mistake.
 *   - Write (x > y) - (x < y), not x - y: a subtraction can overflow.
 *   - qsort is not stable: elements that compare equal may change order. Break
 *     ties by comparing a second field.
 *   - bsearch needs the array sorted by the same compare function.
 * USEDFOR:
 *   - Sorting file names, or records by one field.
 * SEE: man 3 qsort, man 3 bsearch
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    const char *name;
    int age;
};

static int by_number(const void *a, const void *b)
{
    int x = *(const int *) a;
    int y = *(const int *) b;

    return (x > y) - (x < y);
}

static int by_text(const void *a, const void *b)
{
    return strcmp(*(const char *const *) a, *(const char *const *) b);
}

static int by_age(const void *a, const void *b)
{
    const struct Person *x = a;
    const struct Person *y = b;

    return (x->age > y->age) - (x->age < y->age);
}

int main(void)
{
    int numbers[] = { 42, -7, 19, 3 };
    const char *names[] = { "pear", "apple", "fig" };
    struct Person people[] = { { "Ana", 30 }, { "Rui", 25 }, { "Eva", 41 } };
    size_t number_count = sizeof numbers / sizeof numbers[0];

    qsort(numbers, number_count, sizeof numbers[0], by_number);
    printf("numbers:");
    for (size_t i = 0; i < number_count; i++) {
        printf(" %d", numbers[i]);
    }
    printf("\n");

    qsort(names, 3, sizeof names[0], by_text);
    printf("names: %s %s %s\n", names[0], names[1], names[2]);

    qsort(people, 3, sizeof people[0], by_age);
    printf("youngest: %s, oldest: %s\n", people[0].name, people[2].name);

    int wanted = 19;
    int *found = bsearch(&wanted, numbers, number_count, sizeof numbers[0], by_number);
    if (found != NULL) {
        printf("19 is at index %td\n", found - numbers);
    }
    return 0;
}
