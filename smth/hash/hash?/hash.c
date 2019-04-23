#include "hashset.h"

enum {HASH_ERR_FAIL = -1, HASH_ERR_OK = 0, HASH_ERR_OTHER = 1};

const unsigned int prime_1 = 89;
const unsigned int prime_2 = 4591;

hashset_t hashset_create()
{
    hashset_t set = calloc(1, sizeof(struct hashset_st));

    if (set == NULL)
        return HASH_ERR_FAIL;

    set->nbits = 3;
    set->capacity = (size_t)(1 << set->nbits);
    set->mask = set->capacity - 1;
    set->items = calloc(set->capacity, sizeof(size_t));
    if (set->items == NULL) {
        hashset_destroy(set);
        return HASH_ERR_FAIL;
    }
    set->nitems = 0;
    set->n_deleted_items = 0;
    return set;
}

size_t hashset_num_items(hashset_t set)
{
    return set->nitems;
}

void hashset_destroy(hashset_t set)
{
    if (set)
        free(set->items);
    free(set);
}

static int hashset_add_member(hashset_t set, void *item)
{
    size_t value = (size_t)item;
    size_t hash;

    if (value == 0 || value == 1)
        return HASH_ERR_FAIL;

    hash = set->mask & (prime_1 * value);

    while (set->items[hash] != 0 && set->items[hash] != 1) {
        if (set->items[hash] == value) {
            return HASH_ERR_OK;
        } else {
            //! поиск свободного слота
            hash = set->mask & (hash + prime_2);
        }
    }
    set->nitems++;
    if (set->items[hash] == 1)
        set->n_deleted_items--;
    set->items[hash] = value;
    return HASH_ERR_OTHER;
}

static void maybe_rehash(hashset_t set)
{
    size_t *old_items;
    size_t old_capacity, ii;


    if (set->nitems + set->n_deleted_items >= (double)set->capacity * 0.85) {
        old_items = set->items;
        old_capacity = set->capacity;
        set->nbits++;
        set->capacity = (size_t)(1 << set->nbits);
        set->mask = set->capacity - 1;
        set->items = calloc(set->capacity, sizeof(size_t));
        set->nitems = 0;
        set->n_deleted_items = 0;
        assert(set->items);
        for (ii = 0; ii < old_capacity; ii++) {
            hashset_add_member(set, (void *)old_items[ii]);
        }
        free(old_items);
    }
}

int hashset_add(hashset_t set, void *item)
{
    int rv = hashset_add_member(set, item);
    maybe_rehash(set);
    return rv;
}

int hashset_remove(hashset_t set, void *item)
{
    size_t value = (size_t)item;
    size_t ii = set->mask & (prime_1 * value);

    while (set->items[ii] != 0) {
        if (set->items[ii] == value) {
            set->items[ii] = 1;
            set->nitems--;
            set->n_deleted_items++;
            return HASH_ERR_OTHER;
        } else {
            ii = set->mask & (ii + prime_2);
        }
    }
    return HASH_ERR_OK;
}

int hashset_is_member(hashset_t set, void *item)
{
    size_t value = (size_t)item;
    size_t ii = set->mask & (prime_1 * value);

    while (set->items[ii] != 0) {
        if (set->items[ii] == value) {
            return HASH_ERR_OTHER;
        } else {
            ii = set->mask & (ii + prime_2);
        }
    }
    return HASH_ERR_OK;
}
