#ifndef ITERATOR_H
#define ITERATOR_H
#include "hash.h"

struct iterator
{
    hashsetP set;
    hashElemP cur;
};

struct iterator* IterCreate(hashsetP set);

int hasNext(struct iterator* it);

int Next(struct iterator* it);

char* getValue(struct iterator* it);

#endif // ITERATOR_H
