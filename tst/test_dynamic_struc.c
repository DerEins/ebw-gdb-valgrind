#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/set/struct_set.h"

int cmp(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}

void* copy(const void* a)
{
    int* b = malloc(sizeof(*b));
    *b = *(int*)a;
    return b;
}

void del(void* a)
{
    free(a);
}

/*
static void test__find(void)
{
    printf("%s", __func__);
    int tab_init[4] = { 0x0, 0x0, 0x0, 0x0 };
    int tab[7] = { 2, 4, 6, 8, 0, 5, 9 };
    struct set s = { (void*)tab_init,
        4,
        4,
        cmp,
        copy,
        del };
    for (size_t i = 0; i < s.size; ++i) {
        s.s[i] = copy(&tab[i]);
    }
    assert(find(&s, &tab[4], 0, 3) == 0);
    assert(find(&s, &tab[0], 0, 3) == 0);
    assert(find(&s, &tab[1], 0, 3) == 1);
    assert(find(&s, &tab[5], 0, 3) == 2);
    assert(find(&s, &tab[3], 0, 3) == 3);
    assert(find(&s, &tab[6], 0, 3) == 4);

    for (size_t i = 0; i < s.size; ++i) {
        del(s.s[i]);
    }
    printf("\tOK\n");
}*/

static void test__set__empty()
{
    printf("%s", __func__);
    struct set* empty_set = set__empty(cmp, copy, del);
    assert(empty_set->s == NULL);
    assert(empty_set->size == 0);
    assert(empty_set->capacity == 0);
    set__free(empty_set);
    printf("\tOK\n");
}

static void test__set__add(void)
{
    printf("%s", __func__);
    struct set* st = set__empty(cmp, copy, del);
    int tab[1000] = { 1, 2, 3 };

    // simple adding test
    int is_inserted = set__add(st, &tab[2]);
    assert(cmp(st->s[0], &tab[2]) == 0);
    assert(st->capacity == 1);
    assert(st->size == 1);
    assert(is_inserted == 0);

    // not add 3 if already added
    is_inserted = set__add(st, &tab[2]);
    assert(cmp(st->s[0], &tab[2]) == 0);
    assert(st->capacity == 1);
    assert(st->size == 1);
    assert(is_inserted < 0);

    // add 1
    is_inserted = set__add(st, &tab[0]);
    assert(cmp(st->s[0], &tab[0]) == 0);
    assert(cmp(st->s[1], &tab[2]) == 0);
    assert(is_inserted == 0);
    assert(st->size == 2);
    assert(st->capacity == 2);

    // add 2
    is_inserted = set__add(st, &tab[1]);
    assert(is_inserted == 0);
    assert(cmp(st->s[0], &tab[0]) == 0);
    assert(cmp(st->s[1], &tab[1]) == 0);
    assert(cmp(st->s[2], &tab[2]) == 0);
    assert(st->capacity == 4);
    assert(st->size == 3);

    // insert lots of even items
    for (int i = 3; i < 1000; ++i) {
        tab[i] = 2 * i;
        is_inserted = set__add(st, &tab[i]);
        assert(cmp(st->s[i], &tab[i]) == 0);
        assert(is_inserted == 0);
    }
    assert(st->capacity == 1024);
    assert(st->size == 1000);

    // insert random odd item
    int my_int = 501;
    is_inserted = set__add(st, &my_int);
    assert(cmp(st->s[0], &tab[0]) == 0);
    assert(cmp(st->s[1], &tab[1]) == 0);
    assert(cmp(st->s[2], &tab[2]) == 0);
    for (int i = 3; i < 251; ++i) {
        assert(cmp(st->s[i], &tab[i]) == 0);
    }
    assert(cmp(st->s[251], &my_int) == 0);
    for (int i = 252; i < 1000; ++i) {
        assert(cmp(st->s[i], &tab[i - 1]) == 0);
    }
    assert(st->capacity == 1024);
    assert(st->size == 1001);
    assert(is_inserted == 0);

    // insert item at the beginning
    int zero = 0;
    is_inserted = set__add(st, &zero);
    assert(cmp(st->s[0], &zero) == 0);
    assert(cmp(st->s[1], &tab[0]) == 0);
    assert(cmp(st->s[2], &tab[1]) == 0);
    assert(cmp(st->s[3], &tab[2]) == 0);
    for (int i = 4; i < 252; ++i) {
        assert(cmp(st->s[i], &tab[i - 1]) == 0);
    }
    assert(cmp(st->s[252], &my_int) == 0);
    for (int i = 253; i < 1000; ++i) {
        assert(cmp(st->s[i], &tab[i - 2]) == 0);
    }
    assert(st->capacity == 1024);
    assert(st->size == 1002);
    assert(is_inserted == 0);

    // insert item at the end
    int ten_thousand = 10000;
    is_inserted = set__add(st, &ten_thousand);
    assert(cmp(st->s[0], &zero) == 0);
    assert(cmp(st->s[1], &tab[0]) == 0);
    assert(cmp(st->s[2], &tab[1]) == 0);
    assert(cmp(st->s[3], &tab[2]) == 0);
    for (int i = 4; i < 252; ++i) {
        assert(cmp(st->s[i], &tab[i - 1]) == 0);
    }
    assert(cmp(st->s[252], &my_int) == 0);
    for (int i = 253; i < 1000; ++i) {
        assert(cmp(st->s[i], &tab[i - 2]) == 0);
    }
    assert(cmp(st->s[1002], &ten_thousand) == 0);
    assert(st->capacity == 1024);
    assert(st->size == 1003);
    assert(is_inserted == 0);

    // not insert item
    is_inserted = set__add(st, &my_int);
    assert(cmp(st->s[0], &zero) == 0);
    assert(cmp(st->s[1], &tab[0]) == 0);
    assert(cmp(st->s[2], &tab[1]) == 0);
    assert(cmp(st->s[3], &tab[2]) == 0);
    for (int i = 4; i < 252; ++i) {
        assert(cmp(st->s[i], &tab[i - 1]) == 0);
    }
    assert(cmp(st->s[252], &my_int) == 0);
    for (int i = 253; i < 1000; ++i) {
        assert(cmp(st->s[i], &tab[i - 2]) == 0);
    }
    assert(st->capacity == 1024);
    assert(st->size == 1003);
    assert(is_inserted < 0);

    set__free(st);
    printf("\tOK\n");
}

static void test__set__is_empty()
{
    printf("%s", __func__);
    struct set* st = set__empty(cmp, copy, del);

    // set__is_empty(set__empty) = true
    assert(set__is_empty(st));

    // set__is_empty(set__add(s, i)) = false
    int my_int = 3;
    set__add(st, &my_int);
    assert(!set__is_empty(st));

    set__free(st);
    printf("\tOK\n");
}

static void test__set__find(void)
{
    printf("%s", __func__);
    struct set* new_set = set__empty(cmp, copy, del);

    int j = 1; // i'
    // set__find(set__empty, i) = false
    assert(set__find(new_set, &j) == 0);

    // set__find(set__add(s, i), i) = true

    set__add(new_set, &j);
    assert(set__find(new_set, &j));

    // set__find(set__add(s, i), i’) = set__find(s, i’)
    int i = 3; // i
    for (j = 1; j <= 2; ++j) {
        int is_found_without_adding_i = set__find(new_set, &j);
        set__add(new_set, &i);
        assert(is_found_without_adding_i == set__find(new_set, &j));
        i = 4;
    }
    set__free(new_set);
    printf("\tOK\n");
}

static void test__set__remove(void)
{
    printf("%s", __func__);
    // set__remove(set__empty, i) = set__empty
    struct set* st = set__empty(cmp, copy, del);

    int tab[5] = { 1, 2, 4, 6, 8 };
    int is_removed = set__remove(st, &tab[0]);
    assert(st->size == 0);
    assert(st->capacity == 0);
    assert(is_removed < 0);

    // set__remove(set__add(s, i), i) = s
    for (int i = 0; i < 4; ++i) {
        set__add(st, &tab[i + 1]);
    }
    int five = 5;
    int is_added = set__add(st, &five);
    is_removed = set__remove(st, &five);
    assert(is_added == 0 && is_removed == 0);
    for (int i = 0; i < 4; ++i) {
        assert(cmp(st->s[i], &tab[i + 1]) == 0);
    }
    assert(st->size == 4);
    assert(st->capacity == 4);

    // set__remove(set__add(s, i), i’) = set__add(set__remove(s, i’), i)
    struct set* st1 = set__empty(cmp, copy, del);
    struct set* st2 = set__empty(cmp, copy, del);
    int tab2[5] = { 1, 3, 4, 5 };
    for (int i = 0; i < 4; ++i) {
        set__add(st1, &tab2[i]);
        set__add(st2, &tab2[i]);
    }
    int i = 2; // i
    int j = 5; // i'

    set__add(st1, &i);
    int remove_after_add = set__remove(st1, &j);

    is_removed = set__remove(st2, &j);
    int add_after_remove = set__add(st2, &i);

    for (int k = 0; k < 4; ++k) {
        assert(cmp(st1->s[k], st2->s[k]) == 0);
    }
    assert(st1->size == st2->size);
    assert(st1->capacity == st2->capacity);
    assert(st1->capacity == 4);
    assert(st1->size == 4);
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
    struct set* st = set__empty(cmp, copy, del);
    assert(set__size(st) == 0);

    // set__size(set__add(s, i)) = set__size(s)+1 [iset__find(s,i)=false]
    int tab[5] = { 2, 4, 6, 8, 7 };
    for (int i = 0; i < 4; ++i) {
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

int main()
{
    printf("%s\n", __FILE__);
    // test__find();
    test__set__empty();
    test__set__add();
    test__set__is_empty();
    test__set__find();
    test__set__remove();
    test__set__size();
    return 0;
}