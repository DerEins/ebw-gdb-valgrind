#include "../src/set/set.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b)
{
    return *((int *)a) - *((int *)b);
}

void *copy(const void *a)
{
    int *b = malloc(sizeof(*b));
    *b = *(int *)a;
    return b;
}

void del(void *a)
{
    free(a);
}

static void test__create_set(void)
{
    printf("%s", __func__);

    struct set *st = set__empty(cmp, copy, del);
    assert(set__is_empty(st));
    set__free(st);

    printf("\tOK\n");
}

static void test__set__find(void)
{
    printf("%s", __func__);
    struct set *new_set = set__empty(cmp, copy, del);

    // set__find(set__empty, i) = false
    int not_to_find = 1;
    assert(set__find(new_set, &not_to_find) == 0);

    // set__find(set__add(s, i), i) = true
    int j = 1; // i'
    set__add(new_set, &j);
    assert(set__find(new_set, &j));

    // set__find(set__add(s, i), i’) = set__find(s, i’)
    int i = 3; // i
    for (j = 1; j <= 2; ++j)
    {
        int is_found_without_adding_i = set__find(new_set, &j);
        set__add(new_set, &i);
        assert(is_found_without_adding_i == set__find(new_set, &j));
        i = 4;
    }
    set__free(new_set);
    printf("\tOK\n");
}

static void test__set__add(void)
{
    printf("%s", __func__);
    struct set *st = set__empty(cmp, copy, del);

    int tab[4] = {3, -3, 1, 2};
    // simple adding test
    assert(set__add(st, &tab[0]) == 0);
    // set__print(st);
    assert(set__find(st, &tab[0]));
    assert(set__size(st) == 1);

    // not add negative number
    // assert(set__add(st, &tab[1]) < 0);
    // assert(!set__find(st, &tab[1]));
    // assert(set__size(st) == 1);

    // not add 3 if already added
    assert(set__add(st, &tab[0]) < 0);
    assert(set__find(st, &tab[0]));
    assert(set__size(st) == 1);

    // add 1
    assert(set__add(st, &tab[2]) == 0);
    // set__print(st);
    assert(set__find(st, &tab[2]));
    assert(set__find(st, &tab[0]));
    assert(set__size(st) == 2);

    // add 2
    assert(set__add(st, &tab[3]) == 0);
    // set__print(st);
    assert(set__find(st, &tab[2]));
    assert(set__find(st, &tab[3]));
    assert(set__find(st, &tab[0]));
    assert(set__size(st) == 3);

    set__free(st);
    printf("\tOK\n");
}

static void test__set__remove(void)
{
    printf("%s", __func__);
    // set__remove(set__empty, i) = set__empty
    struct set *st = set__empty(cmp, copy, del);

    int one = 1;
    assert(set__remove(st, &one));
    assert(set__size(st) == 0);

    // removing negative item from an empty set
    int minus_one = -1;
    assert(set__remove(st, &minus_one));
    assert(set__size(st) == 0);

    // set__remove(set__add(s, i), i) = s
    int tab[5] = {0, 2, 4, 6, 5};
    for (int i = 0; i < 4; ++i)
    {
        set__add(st, &tab[i]);
    }
    assert(set__add(st, &tab[4]) == 0 && (set__remove(st, &tab[4]) == 0));
    for (int k = 0; k < 4; ++k)
    {
        assert(set__find(st, &tab[k]));
    }
    assert(set__size(st) == 4);

    // set__remove(set__add(s, i), i’) = set__add(set__remove(s, i’), i)
    struct set *st1 = set__empty(cmp, copy, del);
    struct set *st2 = set__empty(cmp, copy, del);

    int tab2[5] = {1, 3, 4, 5, 2};
    set__add(st1, &tab2[0]);
    set__add(st2, &tab2[0]);
    for (int i = 1; i <= 3; ++i)
    {
        set__add(st1, &tab2[i]);
        set__add(st2, &tab2[i]);
    }
    // int i = 2; // i
    // int j = 5; // i'

    set__add(st1, &tab2[4]);
    int remove_after_add = set__remove(st1, &tab2[3]);

    set__remove(st2, &tab2[3]);
    int add_after_remove = set__add(st2, &tab2[4]);

    for (int k = 0; k < 3; ++k)
    {
        assert(set__find(st1, &tab2[k]));
        assert(set__find(st2, &tab2[k]));
    }
    assert(set__find(st1, &tab2[4]));
    assert(set__find(st2, &tab2[4]));

    assert(set__find(st1, &tab2[3]) == 0);
    assert(set__find(st2, &tab2[3]) == 0);
    assert(set__size(st1) == set__size(st2));
    assert(set__size(st1) == 4);
    assert(remove_after_add == add_after_remove);

    set__free(st);
    set__free(st1);
    set__free(st2);
    printf("\tOK\n");
}

static void test__set__size()
{
    printf("%s", __func__);

    // set__size(set__empty) = 0
    struct set *st = set__empty(cmp, copy, del);
    assert(set__size(st) == 0);

    // set__size(set__add(s, i)) = set__size(s)+1 [iset__find(s,i)=false]
    int tab[5] = {2, 4, 6, 8, 7};
    for (int i = 0; i < 4; ++i)
    {
        set__add(st, &tab[i]);
    }
    size_t original_size = set__size(st);
    set__add(st, &tab[4]);
    assert(set__size(st) == original_size + 1);

    // set__size(set__add(s, i))= set__size(s) [if set__find(s,i)=true]
    ++original_size;
    set__add(st, &tab[0]);
    assert(set__size(st) == original_size);

    set__free(st);
    printf("\tOK\n");
}

int is_even(void *n)
{
    return *(int *)n % 2 == 0;
}

static void test__set__filter()
{
    printf("%s", __func__);
    struct set *st = set__empty(cmp, copy, del);
    int tab[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 10; ++i)
    {
        set__add(st, &tab[i]);
    }

    struct set *st_filtered = set__filter(is_even, st);
    for (int i = 0; i < 10; ++i)
    {
        if (is_even(&tab[i]))
            assert(set__find(st_filtered, &tab[i]));
        else
            assert(!set__find(st_filtered, &tab[i]));
    }
    set__free(st);
    set__free(st_filtered);
    printf("\tOK\n");
}

void int__print(const void *p)
{
    int *_p = (int *)p;
    printf(" %d \n", *_p);
}

static void test__set__debug_data()
{
    printf("%s", __func__);
    struct set *st = set__empty(cmp, copy, del);
    int tab[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 10; ++i)
    {
        set__add(st, &tab[i]);
    }

    set__debug_data(int__print, st);
    set__free(st);
    printf("\tOK\n");
}

static void test__problem_1(void)
{
    printf("%s\n", __func__);
    struct set *s = set__empty(cmp, copy, del);

    int vs[4] = {1, 5, 8, 12};
    for (int i = 0; i < 4; i++)
        set__add(s, &vs[i]);

    int v5 = 12;
    printf("Taille de s: %lu (attendu: 4)\n", set__size(s));
    assert(set__size(s) == 4);
    printf("%d appartient à s? %s (attendu: OUI)\n",
           v5, (set__find(s, &v5) ? "OUI" : "NON"));
    assert(set__find(s, &v5));
    set__free(s);
    printf("\tOK\n");
}

static void test__problem_2(void)
{
    printf("%s\n", __func__);
    struct set *s1 = set__empty(cmp, copy, del);
    int vs[4] = {7, 3, 13, 1};
    for (int i = 0; i < 4; i++)
        set__add(s1, &vs[i]);

    struct set *s2 = set__empty(cmp, copy, del);
    int ws[4] = {7, 3, 13, 1};
    for (int i = 0; i < 4; i++)
        set__add(s2, &ws[i]);

    printf("Taille de s1: %lu (attendu: 4)\n", set__size(s1));
    assert(set__size(s1) == 4);
    printf("Taille de s2: %lu (attendu: 4)\n", set__size(s2));
    assert(set__size(s2) == 4);
    printf("vs[0]=%d appartient à s1? %s (attendu: OUI)\n",
           vs[0], (set__find(s1, &vs[0]) ? "OUI" : "NON"));
    assert(set__find(s1, &vs[0]));
    printf("vs[0]=%d appartient à s2? %s (attendu: OUI)\n",
           vs[0], (set__find(s2, &vs[0]) ? "OUI" : "NON"));
    assert(set__find(s2, &vs[0]));
    set__free(s1);
    set__free(s2);
    printf("\tOK\n");
}

static void test__problem_3(void)
{
    printf("%s\n", __func__);
    struct set *s = set__empty(cmp, copy, del);

    int vs[4] = {8, 5, 15, 3};
    for (int i = 0; i < 4; i++)
        set__add(s, &vs[i]);

    printf("Ordre des valeurs dans s: \n");
    set__debug_data(int__print, s);
    printf("\n");

    set__free(s);
    printf("\tOK\n");
}

static void test__problem_4(void)
{
    printf("%s\n", __func__);
    struct set *s = set__empty(cmp, copy, del);

    int vs[4] = {8, 5, 15, 3};
    for (int i = 0; i < 4; i++)
        set__add(s, &vs[i]);

    set__debug_data(int__print, s);
    printf("\n");

    vs[1] = 17;
    set__debug_data(int__print, s);
    printf("\n");

    set__free(s);
    printf("\tOK\n");
}

int main()
{
    printf("%s\n", __FILE__);

    test__create_set();
    test__set__find();
    test__set__add();
    test__set__remove();
    test__set__size();
    test__set__filter();
    test__set__debug_data();
    test__problem_1();
    test__problem_2();
    test__problem_3();
    test__problem_4();
    return EXIT_SUCCESS;
}
