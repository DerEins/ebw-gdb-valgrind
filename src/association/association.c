#include "association.h"
#include <stdio.h>
#include <string.h>

struct association
{
    size_t classement;
    enum statut statut;
    char *nom;
    char *type;
    size_t nb_membres;
};

void *asso__create(
    const size_t classement,
    const enum statut statut,
    const char *nom,
    const char *type,
    const size_t nb_membres)
{
    struct association *new_asso = malloc(sizeof(struct association));
    new_asso->classement = classement;
    new_asso->statut = statut;
    new_asso->nom = malloc(sizeof(char) * (strlen(nom) + 1));
    strcpy(new_asso->nom, nom);
    new_asso->type = malloc(sizeof(char) * (strlen(type) + 1));
    strcpy(new_asso->type, type);
    new_asso->nb_membres = nb_membres;
    return new_asso;
}

void *asso__cpy(const void *asso)
{
    struct association *new_asso = malloc(sizeof(struct association));
    new_asso->classement = ((struct association *)asso)->classement;
    new_asso->statut = ((struct association *)asso)->statut;
    new_asso->nom = malloc(sizeof(char) * (strlen(((struct association *)asso)->nom) + 1));
    strcpy(new_asso->nom, ((struct association *)asso)->nom);
    new_asso->type = malloc(sizeof(char) * (strlen(((struct association *)asso)->type) + 1));
    strcpy(new_asso->type, ((struct association *)asso)->type);
    new_asso->nb_membres = ((struct association *)asso)->nb_membres;
    return new_asso;
}

int asso__cmp(const void *asso1, const void *asso2)
{
    return ((struct association *)asso1)->classement - ((struct association *)asso2)->classement;
}

void asso__print(const void *asso)
{
    printf("Nom\t\t\t: %s\n", ((struct association *)asso)->nom);
    printf("Statut\t\t\t: ");

    switch (((struct association *)asso)->statut)
    {
    case ASSO:
        printf("Association loi 1901\n");
        break;
    case CLUB_BDE:
        printf("Club rattaché au BDE\n");
        break;
    case CLUB_BDS:
        printf("Club rattaché au BDS\n");
        break;
    default:
        printf("Non renseigné\n");
        break;
    }

    printf("Type\t\t\t: %s\n", ((struct association *)asso)->type);
    printf("Nombre de membres\t: %lu\n", ((struct association *)asso)->nb_membres);
    printf("Classement\t\t: %lu\n", ((struct association *)asso)->classement);
}

void asso__del(void *asso)
{
    free(((struct association *)asso)->type);
    free(((struct association *)asso)->nom);
    free(asso);
}