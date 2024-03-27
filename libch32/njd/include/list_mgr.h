/**
 * @file list_mgr.h
 * @author Neil Johnson (nj.designs@protonmail.com)
 * @brief
 * @version 0.1
 * @date 2020-03-08
 *
 * @copyright Copyright (c) 2020
 *
 */
#pragma once

#include <stdlib.h>
#include <stdint.h>

/**
 * @brief Linked List Node type.
 *
 * This must the 1st element in any structure you embed it it
 *
 */
typedef struct list_node {
  struct list_node *prev; /**< Previous Node */
  struct list_node *next; /**< Next Node */
} list_node_t;

/**
 * @brief The actual linked list
 *
 */
typedef struct list_head {
  list_node_t *head;
} list_head_t;

/**
 * @brief Init list
 *
 */
#define list_init(list) \
  { (list)->head = NULL; }

/**
 * @brief Init List Element
 *
 */
#define list_node_init(node) \
  {                          \
    (node)->prev = NULL;     \
    (node)->next = NULL;     \
  }

#define list_head(list) (void *)((list)->head)
#define list_tail(list) \
  (void *)((list)->head == NULL ? NULL : (list)->head->prev)

#define list_next_node(list, node) \
  (void *)((node)->next == (list)->head ? NULL : (node)->next)
#define list_prev_node(list, node) \
  (void *)((node) == (list)->head ? NULL : (node)->prev)

void list_init_from_array(list_head_t *list, void *array, uint32_t node_size,
                          uint32_t num_nodes);

void list_remove_node(list_head_t *List, list_node_t *node);

void list_insert_node_before(list_head_t *List, list_node_t *node,
                             list_node_t *node_to_insert);
void list_insert_node_after(list_head_t *List, list_node_t *node,
                            list_node_t *node_to_insert);
