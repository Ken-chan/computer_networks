#include "hash.h"
#include "iterator.h"


static void test_add(void)
{
    hashsetP set = hashset_create();

    assert(hashset_add(set, "one") == 1);
    assert(hashset_add(set, "two") == 1);
    assert(hashset_add(set, "three") == 1);
    assert(hashset_add(set, "four") == 1);
    assert(hashset_add(set, "five") == 1);
    assert(hashset_add(set, "sixe") == 1);
    assert(hashset_add(set, "seven") == 1);
    assert(hashset_add(set, "eight") == 1);
    assert(hashset_add(set, "nine") == 1);
    assert(hashset_add(set, "ten") == 1);
    assert(hashset_add(set, "eleven") == 1);
    assert(hashset_add(set, "twelw") == 1);

    assert(hashset_add(set, "Buddy") == 1);
    assert(hashset_add(set, "you're") == 1);
    assert(hashset_add(set, "a_boy") == 1);
    assert(hashset_add(set, "make") == 1);
    assert(hashset_add(set, "a_big_noise") == 1);
    assert(hashset_add(set, "Playin'_in") == 1);
    assert(hashset_add(set, "the_street") == 1);
    assert(hashset_add(set, "gonna_be") == 1);
    assert(hashset_add(set, "a_big_man") == 1);
    assert(hashset_add(set, "someday") == 1);
    assert(hashset_add(set, "You_got") == 1);
    assert(hashset_add(set, "mud") == 1);
    assert(hashset_add(set, "on_yo'_face") == 1);
    assert(hashset_add(set, "You") == 1);
    assert(hashset_add(set, "big") == 1);
    assert(hashset_add(set, "disgrace") == 1);
    assert(hashset_add(set, "Kickin'_your_can_all_over_the_place") == 1);
    assert(hashset_add(set, "Singin'") == 1);
    assert(hashset_add(set, "we") == 1);
    assert(hashset_add(set, "will") == 1);
    assert(hashset_add(set, "we") == 1);
    assert(hashset_add(set, "will") == 1);
    assert(hashset_add(set, "rock") == 1);
    assert(hashset_add(set, "you") == 1);
    assert(hashset_add(set, "we") == 1);
    assert(hashset_add(set, "will") == 1);
    assert(hashset_add(set, "we") == 1);
    assert(hashset_add(set, "will") == 1);
    assert(hashset_add(set, "rock") == 1);
    assert(hashset_add(set, "you") == 1);

    printf("test_add - OK\n");

    assert(hashset_is_member(set, "twelw") == 1);
    assert(hashset_is_member(set, "one") == 1);
    assert(hashset_is_member(set, "eleven") == 1);
    assert(hashset_is_member(set, "one") == 1);
    assert(hashset_is_member(set, "two") == 1);
    assert(hashset_is_member(set, "three") == 1);
    assert(hashset_is_member(set, "four") == 1);
    assert(hashset_is_member(set, "five") == 1);
    assert(hashset_is_member(set, "sixe") == 1);
    assert(hashset_is_member(set, "seven") == 1);
    assert(hashset_is_member(set, "It") == 0);
    assert(hashset_is_member(set, "was") == 0);
    assert(hashset_is_member(set, "interesting") == 0);
    assert(hashset_is_member(set, "to") == 0);
    assert(hashset_is_member(set, "create") == 0);
    assert(hashset_is_member(set, "it") == 0);
    assert(hashset_is_member(set, "twelw") == 1);

    printf("test_is_member - OK\n");

    assert(hashset_remove(set, "one") == 1);
    assert(hashset_remove(set, "one") == 0);
    assert(hashset_remove(set, "we") == 1);
    assert(hashset_remove(set, "will") == 1);
    assert(hashset_remove(set, "we") == 1);
    assert(hashset_remove(set, "will") == 0);
    assert(hashset_remove(set, "rock") == 1);
    assert(hashset_remove(set, "you") == 1);

    printf("test_remove - OK\n");

    assert(hashset_is_member(set, "twelw") == 1);
    assert(hashset_is_member(set, "one") == 0);
    assert(hashset_is_member(set, "we") == 0);
    assert(hashset_is_member(set, "will") == 0);
    assert(hashset_is_member(set, "we") == 0);
    assert(hashset_is_member(set, "will") == 0);
    assert(hashset_is_member(set, "rock") == 0);
    assert(hashset_is_member(set, "you") == 0);
    assert(hashset_is_member(set, "we") == 0);
    assert(hashset_is_member(set, "will") == 0);
    assert(hashset_is_member(set, "we") == 0);
    assert(hashset_is_member(set, "will") == 0);
    assert(hashset_is_member(set, "rock") == 0);
    assert(hashset_is_member(set, "you") == 0);

    printf("test_is_member - OK\n");

    hashset_destroy(set);
}

int main()
{
    test_add();

    printf("everithing ok!");
    return 0;
}
