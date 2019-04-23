#include "hash.h"
#define PRIME 213
enum {HASH_ERR_FAIL = 0, HASH_ERR_OK = 1, HASH_ERR_OTHER = -1};

hashsetP hashset_create()
{
    hashsetP set = calloc(1, sizeof(struct hashset));

    if (set == NULL)
        return HASH_ERR_FAIL;
    set->size = 211;
    set->table = calloc(set->size, sizeof(hashElemP));
    for(int i = 0; i < set->size; i++)
    {
        set->table[i] = malloc(sizeof(hashElemP));
        set->table[i]->next = NULL;
        set->table[i]->value = 0;
    }
    if (set->table == NULL) {
        return HASH_ERR_FAIL;
    }
    return set;
}

int hashset_num_items(hashsetP set)
{
    return set->size;
}

void hashset_destroy(hashsetP set)
{
    if (set)
    {
        int i;
        for(i = 0; i < set->size; i++)
        {
            hashElemP this = set->table[i];
            hashElemP next = set->table[i]->next;
            while(next != NULL)
            {
                free(this);
                this = next;
                next = this->next;
            }
            free(this);
        }
        free(set->table);
        free(set);
    }
}

int getHash(hashsetP set, char* val)
{
    int i = 0;
    int tmp = 0;
    int hash;
    while(val[i] != 0)
    {
        tmp += (int)val[i];
        i++;
    }
    hash = tmp*PRIME % set->size;
    return hash;
}

int hashset_add(hashsetP set, char* val)
{    
    if (val == 0)
        return HASH_ERR_FAIL;
    hashElemP elem = calloc(1, sizeof(struct hashElem));

    elem->value = val;
    elem->next = NULL;
    int hash = getHash(set, val);
    hashElemP this = set->table[hash];

    if (this->value == 0)
        set->table[hash] = elem;
    while (this != NULL)
    {
        if (this->value == elem->value)
            return HASH_ERR_OK;
        else {
            //! поиск свободного слота
            if(this->next)
            this = this->next;
            else
            {
                this->next = elem;
                return HASH_ERR_OK;
            }
        }
    }
    return HASH_ERR_FAIL;
}

int hashset_remove(hashsetP set, char* val)
{
    if(set == NULL)
        return HASH_ERR_FAIL;
    int hash = getHash(set, val);
    if(set->table[hash]->value == 0)
        return HASH_ERR_FAIL;
    if(!strcmp(set->table[hash]->value, val))
    {
        if(set->table[hash]->next)
        {
            hashElemP tmp = set->table[hash]->next;
            set->table[hash]->next = tmp->next;
            set->table[hash]->value = tmp->value;
            free(tmp);
        }
        else
            set->table[hash]->value = 0;
        return HASH_ERR_OK;
    }

    hashElemP this = set->table[hash];
    while (this != NULL)
    {
        if (strcmp(this->value, val))
            if(this->next)
            {
                hashElemP tmp = this->next;
                this->next = tmp->next;
                this->value = tmp->value;
                free(tmp);
            }
            else
                free(this);
            return HASH_ERR_OK;
    }
    return HASH_ERR_FAIL;
}

int hashset_is_member(hashsetP set, char* val)
{
    int hash = getHash(set, val);
    hashElemP this = set->table[hash];
    if (this->value == 0)
        return HASH_ERR_FAIL;
    while (this)
    {
        if (this->value == val)
        {
            return HASH_ERR_OK;
        }
        else
            this = this->next;
    }
    return HASH_ERR_FAIL;
}
