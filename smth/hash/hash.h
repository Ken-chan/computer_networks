#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct hashElem
{
    char* value;
    struct hashElem* next;
};

struct hashset
{
    struct hashElem** table;
    int size;
};

typedef struct hashset* hashsetP;
typedef struct hashElem* hashElemP;

hashsetP hashset_create();

/**
 * @brief getHash - Генерация хэша. Возвращает хэш.
 * @param set - таблица
 * @param val - строка данных
 * @return
 */
int getHash(hashsetP set, char* val);

/// \brief Удалить таблицу.
void hashset_destroy(hashsetP set);

/// \brief Вернуть количество элементов в таблице.
int hashset_num_items(hashsetP set);

  /**
 * @brief hashset_add - Добавить элемент в таблицу.
 * @param set - таблица
 * @param val - строка данных
 * @return
 */
int hashset_add(hashsetP set, char* val);

/**
 * @brief hashset_remove - Удалить элемент из таблицы.
 * @param set - таблица
 * @param val - строка данных
 * @return
 */
int hashset_remove(hashsetP set, char* val);

/**
 * @brief hashset_is_member - Проверить наличие элемента в таблице.
 * @param set - таблица
 * @param val - строка данных
 * @return
 */
int hashset_is_member(hashsetP set, char* val);


#endif // HASH_H
