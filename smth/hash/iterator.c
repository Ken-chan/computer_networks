#include "iterator.h"


struct iterator* IterCreate(hashsetP set)
{
    struct iterator* it = malloc(sizeof(struct iterator));
    it->set = set;
    int i = 0;
    while(set->table[i] == 0 && i < it->set->size)
        i++;
    if(i < it->set->size)
        it->cur=it->set->table[i];
    return it;

}

char* getValue(struct iterator* it)
{
    return it->cur->value;
}

int hasNext(struct iterator* it)
{
    if(it->cur->next)
        return 1;
    int hash = getHash(it->set, it->cur->value);
    printf("11");
    printf("%s", it->set->table[hash]->value);
    while(it->set->table[hash]->value == 0 && hash < it->set->size)
        hash++;
    if(hash < it->set->size)
        return 1;
    return 0;
}


int Next(struct iterator* it)
{
    if(it->cur->next)
    {
        it->cur = it->cur->next;
        return 1;
    }
    int hash = getHash(it->set, it->cur->value);
    while(it->set->table[hash]->value == 0 && hash < it->set->size)
        hash++;
    if(hash < it->set->size)
    {
        it->cur = it->set->table[hash];
        return 1;
    }
    return 0;
}
