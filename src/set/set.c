#include "struct_set.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int find(const struct set *s, void *c, int begin, int end)
{
    if (s->cmp(c, s->s[begin]) < 0)
    {
        return begin;
    }
    if (s->cmp(c, s->s[end]) > 0)
    {
        return end + 1;
    }
    int mid = 0;
    while ((end - begin) > 1)
    {
        mid = (begin + end) / 2;
        if (s->cmp(s->s[mid], c) < 0)
        {
            begin = mid;
        }
        else if (s->cmp(s->s[(begin + end) / 2], c) > 0)
        {
            end = mid;
        }
        else
        {
            return mid;
        }
    }
    return (s->cmp(c, s->s[begin]) == 0) ? begin : end;
}

void shift_right(void *s[], size_t begin, size_t end)
{
    unsigned int cpt = begin;
    void *tmp = s[cpt];
    void *swp = 0;
    for (cpt = begin; cpt < end; ++cpt)
    {
        swp = tmp;
        tmp = s[cpt + 1];
        s[cpt + 1] = swp;
    }
    s[cpt + 1] = tmp;
}

void shift_left(void *s[], size_t begin, size_t end)
{
    unsigned int cpt = begin;
    for (cpt = begin; cpt < end; ++cpt)
    {
        s[cpt] = s[cpt + 1];
    }
}

struct set *set__empty(int (*cmp)(const void *, const void *), void *(*copy)(const void *), void (*del)(void *))
{
    struct set *new_set = malloc(sizeof(*new_set));
    new_set->s = NULL;
    new_set->size = 0;
    new_set->capacity = 0;
    new_set->cmp = cmp;
    new_set->copy = copy;
    new_set->del = del;
    return new_set;
}

int set__is_empty(const struct set *se)
{
    return se->size == 0 && se->capacity == 0;
}

int set__add(struct set *se, void *c)
{
    if (se->s == NULL)
    {
        se->capacity = 1;
        se->size = 1;
        se->s = malloc(sizeof(*se->s));
        se->s[0] = se->copy(c);
        return 0;
    }

    unsigned int found_idx = find(se, c, 0, se->size - 1);

    if (found_idx >= se->size || se->cmp(se->s[found_idx], c) != 0)
    {
        if (found_idx >= se->capacity || se->capacity <= se->size)
        {
            se->capacity *= 2;
            se->s = realloc(se->s, se->capacity * sizeof(void *));
        }
        if (found_idx < se->size)
        {
            shift_right(se->s, found_idx, se->size - 1);
        }
        se->s[found_idx] = se->copy(c);
        se->size += 1;
        return 0;
    }
    return -1;
}

int set__find(const struct set *se, void *c)
{
    if (se->s == NULL)
    {
        return 0;
    }
    unsigned int found_idx = find(se, c, 0, se->size - 1);
    return (se->size > 0 && found_idx < se->size && se->cmp(se->s[found_idx], c) == 0);
}

int set__remove(struct set *se, void *c)
{

    if (se->s == NULL)
    {
        return -3;
    }
    int found_idx = find(se, c, 0, se->size - 1);
    if (se->cmp(se->s[found_idx], c) == 0)
    {
        if (se->size == 1)
        {
            free(se->s[0]);
            free(se->s);
            se->s = NULL;
            se->capacity = 0;
            se->size = 0;
            return 0;
        }
        se->del(se->s[found_idx]);
        shift_left(se->s, found_idx, se->size - 1);
        if (se->size - 1 <= se->capacity / 2)
        {
            se->capacity /= 2;
            se->s = realloc(se->s, se->capacity * sizeof(void *));
        }
        se->size -= 1;
        return 0;
    }
    return -1;
}

size_t set__size(const struct set *se)
{
    return se->size;
}

struct set *set__filter(int (*filter)(void *), const struct set *se)
{
    struct set *s_filtered = set__empty(se->cmp, se->copy, se->del);
    size_t cpt = 0;
    while (cpt < se->size)
    {
        if (filter(se->s[cpt]))
        {
            set__add(s_filtered, se->s[cpt]);
        }
        ++cpt;
    }
    return s_filtered;
}

void set__debug_data(void (*print_el)(const void *), const struct set *se)
{
    size_t cpt = 0;

    printf("======\t Début de l'ensemble \t======\n");
    while (cpt < se->size)
    {
        printf("------\t Élement %lu \t\t------\n", cpt);
        print_el(se->s[cpt]);
        ++cpt;
    }
    printf("======\t Fin de l'ensemble \t======\n");
}

void set__free(struct set *se)
{
    for (size_t i = 0; i < se->size; ++i)
    {
        se->del(se->s[i]);
    }
    free(se->s);
    free(se);
}