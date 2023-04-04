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

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
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
