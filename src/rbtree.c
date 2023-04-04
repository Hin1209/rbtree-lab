#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)malloc(sizeof(node_t));
  nil->color = RBTREE_BLACK;
  tree->nil = nil;
  tree->root = nil;

  return tree;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void right_rotate(rbtree *tree, node_t *node)
{
  node_t *parent_node = node->parent;
  node_t *right_child = node->right;

  parent_node->left = right_child;
  right_child->parent = parent_node;

  if (parent_node == tree->root)
  {
    tree->root = node;
    node->parent = tree->nil;
  }
  else
  {
    node_t *grand_parent_node = parent_node->parent;

    int is_parent_left;
    if (grand_parent_node->left == parent_node)
      is_parent_left = 1;
    else
      is_parent_left = 0;

    if (is_parent_left)
      grand_parent_node->left = node;
    else
      grand_parent_node->right = node;

    node->parent = grand_parent_node;
  }

  parent_node->parent = node;
  node->right = parent_node;
}

void left_rotate(rbtree *tree, node_t *node)
{
  node_t *parent_node = node->parent;
  node_t *left_child = node->left;

  parent_node->right = left_child;
  left_child->parent = parent_node;

  node->left = parent_node;

  if (parent_node == tree->root)
  {
    tree->root = node;
    node->parent = tree->nil;
  }
  else
  {
    node_t *grand_parent_node = parent_node->parent;

    int is_parent_left;
    if (grand_parent_node->left == parent_node)
      is_parent_left = 1;
    else
      is_parent_left = 0;

    if (is_parent_left)
      grand_parent_node->left = node;
    else
      grand_parent_node->right = node;
  }
  parent_node->parent = node;
}

void rbtree_insert_fixup(rbtree *tree, node_t *node)
{
  if (node == tree->root)
  {
    node->color = RBTREE_BLACK;
    return;
  }

  node_t *parent_node = node->parent;
  if (parent_node->color == RBTREE_BLACK)
    return;

  node_t *grand_parent_node = parent_node->parent;
  node_t *uncle_node;
  int is_parent_left;
  int is_left_child;

  if (grand_parent_node->left == parent_node)
  {
    uncle_node = grand_parent_node->right;
    is_parent_left = 1;
  }
  else
  {
    uncle_node = grand_parent_node->left;
    is_parent_left = 0;
  }

  if (node == parent_node->left)
    is_left_child = 1;
  else
    is_left_child = 0;

  if (uncle_node->color == RBTREE_RED)
  {
    grand_parent_node->color = RBTREE_RED;
    parent_node->color = RBTREE_BLACK;
    uncle_node->color = RBTREE_BLACK;
    rbtree_insert_fixup(tree, grand_parent_node);
  }
  else
  {
    if (is_parent_left)
    {
      if (is_left_child)
      {
        right_rotate(tree, node->parent);
        node->parent->color = RBTREE_BLACK;
        node->parent->right->color = RBTREE_RED;
      }
      else
      {
        left_rotate(tree, node);
        right_rotate(tree, node);
        node->color = RBTREE_BLACK;
        node->right->color = RBTREE_RED;
      }
    }
    else
    {
      if (is_left_child)
      {
        right_rotate(tree, node);
        left_rotate(tree, node);
        node->color = RBTREE_BLACK;
        node->left->color = RBTREE_RED;
      }
      else
      {
        left_rotate(tree, node->parent);
        node->parent->color = RBTREE_BLACK;
        node->parent->left->color = RBTREE_RED;
      }
    }
  }
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node->key = key;
  node->color = RBTREE_RED;
  node->left = node->right = t->nil;

  if (t->root == t->nil)
  {
    t->root = node;
    node->parent = t->nil;
    return node;
  }

  node_t *current_node = t->root;
  while (1)
  {
    if (current_node->key <= key)
    {
      if (current_node->right == t->nil)
      {
        current_node->right = node;
        node->parent = current_node;
        break;
      }
      current_node = current_node->right;
    }
    else
    {
      if (current_node->left == t->nil)
      {
        current_node->left = node;
        node->parent = current_node;
        break;
      }
      current_node = current_node->left;
    }
  }

  rbtree_insert_fixup(t, node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *current_node = t->root;
  while (key != current_node->key)
  {
    if (current_node == t->nil)
      return NULL;

    if (key < current_node->key)
      current_node = current_node->left;
    else
      current_node = current_node->right;
  }

  return (current_node != t->nil) ? current_node : NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *current_node = t->root;
  while (current_node != t->nil && current_node->left != t->nil)
    current_node = current_node->left;

  return current_node;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
