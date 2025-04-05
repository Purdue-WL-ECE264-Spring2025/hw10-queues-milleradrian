#include "linked_list.h"
#include <stdlib.h>

// creates new node with given value
struct list_node *new_node(size_t value) 
{
  struct list_node *node = malloc(sizeof(struct list_node));
  if (node == NULL) 
  {
    fprintf(stderr, "couldn't allocate memory for new node\n");
    return NULL;
  }
  node->value = value;
  node->next = NULL;
  return node;
}

// adds node to beginning of list
void insert_at_head(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);
  if (!node)
  {
    return;
  } 
  node->next = list->head;
  list->head = node;
}

// adds node to end of list
void insert_at_tail(struct linked_list *list, size_t value) 
{
  struct list_node *node = new_node(value);
  if (!node)
  { 
    return;
  }

  if (list->head == NULL) 
  {
    list->head = node;
    return;
  }
  struct list_node *cur = list->head;
  while (cur->next != NULL) 
  {
    cur = cur->next;
  }
  cur->next = node;
}

// removes head node and returns value
size_t remove_from_head(struct linked_list *list) 
{
  if (list == NULL || list->head == NULL) 
  {
  // if nothing to remove, just return 0
  return 0;
  }
  struct list_node *temp = list->head;
  size_t value = temp->value;

  list->head = temp->next;
  free(temp);
  return value;
}

// removes last node and returns value
size_t remove_from_tail(struct linked_list *list) 
{
  if (!list || !list->head)
  {
    return 0;
  }

  if (list->head->next == NULL) 
  {
    size_t val = list->head->value;
    free(list->head);
    list->head = NULL;
    return val;
  }
  struct list_node *cur = list->head;

  // go until second to last node
  while (cur->next && cur->next->next) 
  {
  cur = cur->next;
  }
  size_t val = cur->next->value;
  free(cur->next);
  cur->next = NULL;

  return val;
}

// clears entire list and frees memory
void free_list(struct linked_list list) 
{
  struct list_node *cur = list.head;
  while (cur) 
  {
    struct list_node *next = cur->next;
    free(cur);
    cur = next;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) 
{
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) 
  {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}


