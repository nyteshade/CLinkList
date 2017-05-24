#include <stdio.h>
#include "LinkList.h"

int main(int argc, char **argv) 
{
  LinkList *list = LLCreate();

  list->pushKString(list, "name", "Brielle");
  list->pushKInt(list, "age", 32);

  printf("Name: %s\n", list->popKString(list, "name"));
  printf("Age : %d\n", list->popKInt(list, "age"));
  
  LLDelete(list);
  
  return 0;
}
