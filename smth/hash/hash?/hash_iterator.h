#ifndef HASHSET_ITERATOR_H
#define HASHSET_ITERATOR_H

#include "hashset.h"

/// Параметры итератора
struct hashset_itr_st {
    hashset_t set; ///< Таблица
    size_t index; ///< Индекс итератора
};

typedef struct hashset_itr_st* hashset_itr_t;

/// \brief Создать итератор и обозначить первым доступным элементом.
hashset_itr_t hashset_iterator(hashset_t set);

/// \brief Вернуть значение данного индекса итератора.
size_t hashset_iterator_value(hashset_itr_t iterator);

/// \brief Вернуть 0, если есть следующий элемент и другое в противном случае.
int hashset_iterator_has_next(hashset_itr_t iterator);

/*!
 *      \brief Получить следующий итератор.
 *
 *      Проверить на наличие следующего итератора и
 *      \returns Вернуть индекс следующего итератора, и -1 в противном случае.
 */
int hashset_iterator_next(hashset_itr_t iterator);

#endif // HASHSET_ITERATOR_H
