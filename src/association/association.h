#ifndef __ASSOCIATION_H__
#define __ASSOCIATION_H__

#include <stdlib.h>

#define MAX_STRING 64
#define MAX_TEL 12

enum statut
{
    ASSO,
    CLUB_BDE,
    CLUB_BDS
};

struct association;

void *asso__create(const size_t classement, const enum statut statut,
                   const char *nom,
                   const char *type,
                   const size_t nb_membres);

int asso__cmp(const void *asso1, const void *asso2);

void *asso__cpy(const void *asso);

void asso__del(void *asso);

void asso__print(const void *asso);

#endif //__ASSOCIATION_H__