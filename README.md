# C LinkList

## Overview
I work on various retro computing platforms. I wanted a quick linked list that I could use on them, written in C, that allowed me to, without fuss, work with both Strings and Numbers. 

This is an early WIP (_work in progress_), but for now it compiles and runs and does so with an easy ability to clear the memory allocated for its use. 

## TODO
I would like to add a suite of handy functions that allow for anything from iterating over items to hashing them. I also need to add support for KeyedVoids allowing pointers to whatever the user may want, keyed by a string. 

###Example
```c
  LinkList *list = LLCreate();

  LLPushKeyedString(list, "name", "Brielle", 0L);
  LLPushKeyedString(list, "age", "MYOB", 0L);
  
  printf("Name: %s\n", LLPopKeyedString(list, "name")->string.s);
  printf("Age : %s\n", LLPopKeyedString(list, "age")->string.s);
  
  LLDelete(list);
```