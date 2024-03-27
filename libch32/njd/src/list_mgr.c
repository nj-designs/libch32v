/**
 * @file list_mgr.c
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2020-03-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "list_mgr.h"

/**
 * @brief Insert node before target
 *
 * @param List
 * @param node
 * @param node_to_insert
 */
void list_insert_node_before(list_head_t *List, list_node_t *node,
                             list_node_t *node_to_insert) {
  if (List->head == NULL) {
    /* Empty list - ignore Elem and create list */
    List->head = node_to_insert;
    node_to_insert->prev = node_to_insert;
    node_to_insert->next = node_to_insert;
  } else {
    if (node == NULL || node == List->head) {
      /* Insert before head of list - so assign node pointer
         and update head of list to point to element being inserted */
      node = List->head;
      List->head = node_to_insert;
    }

    /* Insert before */
    node_to_insert->next = node;
    node_to_insert->prev = node->prev;
    node->prev = node_to_insert;
    node_to_insert->prev->next = node_to_insert;
  }
}
/**
 * @brief Insert node after target
 *
 * @param List
 * @param node
 * @param node_to_insert
 */
void list_insert_node_after(list_head_t *List, list_node_t *node,
                            list_node_t *node_to_insert) {
  if (List->head == NULL) {
    /* Empty list - ignore node and create list */
    List->head = node_to_insert;
    node_to_insert->prev = node_to_insert;
    node_to_insert->next = node_to_insert;
  } else {
    if (node == NULL) {
      /* Inserting after tail of list, so assign node to tail of list */
      node = List->head->prev;
    }
    /* Insert after */
    node_to_insert->prev = node;
    node_to_insert->next = node->next;
    node->next = node_to_insert;
    node_to_insert->next->prev = node_to_insert;
  }
}

/**
 * @brief Remove node from list
 *
 * @param List
 * @param node
 */
void list_remove_node(list_head_t *List, list_node_t *node) {
  if (node == node->next) {
    /* node is the only node in the list,
       so clear Head pointer */
    List->head = NULL;
  } else {
    if (node == List->head) {
      /* 1st node, so update the head pointer */
      List->head = node->next;
    }

    /* Remove node from the list */
    node->next->prev = node->prev;
    node->prev->next = node->next;
  }
  list_node_init(node);
}

/**
 * @brief Build list from array of nodes
 *
 * @param list
 * @param array
 * @param node_size
 * @param num_nodes
 */
void list_init_from_array(list_head_t *list, void *array, uint32_t node_size,
                          uint32_t num_nodes) {
  list_init(list);
  for (uint32_t idx = 0; idx < num_nodes; idx++) {
    list_node_init((list_node_t *)array);
    list_insert_node_after(list, NULL, (list_node_t *)array);
    array = (uint8_t *)array + node_size;
  }
}
