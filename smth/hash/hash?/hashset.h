#ifndef HASHSET_H
#define HASHSET_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/// Параметры хэш-таблицы
struct hashset_st {
    size_t nbits; ///< Биты для хэша
    size_t mask; ///< Хэш

    size_t capacity; ///< Размер под таблицу
    size_t *items; ///< Массив
    size_t nitems; ///< Размер массива
    size_t n_deleted_items; ///< Количество удаленных элементов
    };

    typedef struct hashset_st* hashset_t;

    /// \brief Создать таблицу.
    hashset_t hashset_create(void);

    /// \brief Удалить таблицу.
    void hashset_destroy(hashset_t set);

    /// \brief Вернуть количество элементов в таблице.
    size_t hashset_num_items(hashset_t set);

    /*!
     *      \brief Добавить элемент в таблицу.
     *
     *      \note 0 и 1 - особые значения, означающие nil и удаленные элементы
     *      функция вернет -1, указав ошибку.
     *
     *      \returns Вернуть 0, если элемент уже установлен и ненулевое значение в противном случае.
     */
    int hashset_add(hashset_t set, void *item);

    /*!
     *      \brief Удалить элемент из таблицы.
     *
     *      \returns Вернуть ненулевое значение, если элемент был удален, и ноль
     *      в пртивном случае.
     */
    int hashset_remove(hashset_t set, void *item);

    /*!
     *      \brief Проверить наличие элемента в таблице.
     *
     *      \returns Вернуть ненулевое значение, если элемент найден, и ноль
     *      в противном случае.
     */
    int hashset_is_member(hashset_t set, void *item);

#endif // HASHSET_H
