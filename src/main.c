#include "set.h"
#include "association.h"
#include <stdio.h>

#define NB_ASSOS 5

struct associations *assos[5] = {};

void data_setup()
{
    assos[0] = asso__create(0, ASSO, "Eirbware", "Aide, formation et services informatique", 6);
    assos[1] = asso__create(4, ASSO, "BDE", "Bureau des Élèves de l'ENSEIRB-Matméca", 30);
    assos[2] = asso__create(1, ASSO, "AEI", "Junior-Entreprise", 31);
    assos[3] = asso__create(2, ASSO, "Gala Most'Fete", "28 janvier 2023", 14);
    assos[4] = asso__create(3, ASSO, "Cycl'Eirb", "DOS D'ANE !!!", 7);
}

void data_clean()
{
    for (size_t i = 0; i < NB_ASSOS; i++)
    {
        asso__del(assos[i]);
    }
}

int main(int argc, char *argv[])
{
    data_setup();
    struct set *set_assos_enseirb = set__empty(asso__cmp, asso__cpy, asso__del);
    for (size_t i = 0; i < NB_ASSOS; i++)
    {
        set__add(set_assos_enseirb, assos[i]);
    }

    set__debug_data(asso__print, set_assos_enseirb);
    set__free(set_assos_enseirb);
    data_clean();
    return 0;
}
