#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

struct ht_item
{
  int hash; /* значение хэш-функции  0 - 63*/

  bool *value; /* ключ */

  struct ht_item *next; /* следующий элемент в цепочке */

};

struct htable
{
  int size; /* текущий размер хэш-таблицы */

  struct ht_item **items_table; /* элементы */

};

struct htable* htable_new(void);
void htable_free(struct htable *tbl);
bool htable_get(struct htable *tbl,
  const char *key, const size_t key_len, int *value);
bool htable_set(struct htable *tbl,
  const char *key, const size_t key_len, const int value);
bool htable_del(struct htable *tbl,
  const char *key, const size_t key_len);

#endif // HASH_H
