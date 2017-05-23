# C LinkList

## Overview
I work on various retro computing platforms. I wanted a quick linked list that I could use on them, written in C, that allowed me to, without fuss, work with both Strings and Numbers.

This is an early WIP (_work in progress_), but for now it compiles and runs and does so with an easy ability to clear the memory allocated for its use.

## TODO
 - [ ] Add a suite of handy functions that allow for anything from iterating over items to hashing them.
 - [ ] Add support for KeyedVoids allowing pointers to whatever the user may want, keyed by a string.
 - [ ] Add proper code comments and usage to the header
 - [ ] Add a proper test.c file that generates a unit test binary

## Confirmed Compiler Support
 - [ ] SAS/C 6.58 (Amiga/68k)
 - [ ] VBCC (Amiga/68k)
 - [x] Metrowerks Codewarrior 4.x (Mac 68k/PPC)
 - [ ] Metrowerks Codewarrior 2.x (Mac 68k/PPC)
...more to come

## Defines of Note
Given that a lot of retro platforms neither support wchar or 64-bit types such as long long or long double, these are escaped behind some pound sign defines.

Ensure that ```BIG_TYPES``` is defined for the 64-bit types and ```WCHAR_SUPPORT``` for wide UTF-8 style characters.

### Example
```c
  LinkList *list = LLCreate();

  LLPushKeyedString(list, "name", "Brielle", 0L);
  LLPushKeyedString(list, "age", "MYOB", 0L);

  printf("Name: %s\n", LLPopKeyedString(list, "name")->string.s);
  printf("Age : %s\n", LLPopKeyedString(list, "age")->string.s);

  LLDelete(list);
```

### Proposed Fun
The idea below is to add a suite of function pointers to the main `LinkList` struct that will be assigned when `LLCreate()` is invoked.  This allows an almost OO syntax to be used with the exception being that the list itself must be passed in as the first parameter for each of the said function calls. 

In addition to potentially looking cleaner, this also provides an opportunity for your own code to have its own LLCreate() function that reassigns these methods to your own versions of the functions without chaning the standard usage pattern.

```
  LinkList *list = LLCreate();
  list->addKeyedString(list, “name”, “Brielle”);
  list->addKeyedString(list, “age”, “MYOB”);

  /* use list */

  LLDelete(list);
```