#include "hash_iterator.h"

enum {HASH_ERR_FAIL = -1, HASH_ERR_OK = 0};

hashset_itr_t hashset_iterator(hashset_t set)
{
    hashset_itr_t iterator = calloc(1, sizeof(struct hashset_itr_st));
    if (iterator == NULL)
      return HASH_ERR_FAIL;

    iterator->set = set;
    iterator->index = 0;

    //! Получить первый индек итератора, если он есть
    if (set->nitems > 0)
      hashset_iterator_next(iterator);

    return iterator;
}

int hashset_iterator_has_next(hashset_itr_t iterator)
{
    size_t index;

    //! Законичился или полный set
    if (iterator->set->nitems == 0 || iterator->index == iterator->set->capacity - 1)
      return HASH_ERR_FAIL;

    //! Поиск следующего значения
    index = iterator->index;
    while (index <= iterator->set->capacity -1) {
      size_t value = iterator->set->items[index];
      if (value != 0)
        return HASH_ERR_OK;
    }

    //! Другой вариант
    return HASH_ERR_FAIL;

}

int hashset_iterator_next(hashset_itr_t iterator)
{

    if(hashset_iterator_has_next(iterator) == 0)
        return HASH_ERR_FAIL;

    iterator->index++;

    while (iterator->set->items[(iterator->index)] == 0 && iterator->index < iterator->set->capacity) {
      iterator->index++;
    }

    return iterator->index;
}

size_t hashset_iterator_value(hashset_itr_t iterator) {

    /*!
     * Проверка, что у нас нет нулевого значения, это может произойти, если создан итератор
     * перед добавлением элементов в набор.
     */
    if(iterator->set->items[iterator->index] == 0)
      hashset_iterator_next(iterator);

    return iterator->set->items[iterator->index];
}
