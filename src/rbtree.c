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

node_t *get_successor(rbtree *t, node_t *p)
{
  if (p == rbtree_max(t))
    return t->nil;
  if (p == t->nil)
    return p;
  node_t *current_node;
  if (p->right == t->nil)
  {
    current_node = p;
    while (1)
    {
      if (current_node->parent->right == current_node)
        current_node = current_node->parent;
      else
        return current_node->parent;
    }
  }
  current_node = p->right;

  while (current_node->left != t->nil)
    current_node = current_node->left;

  return current_node;
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
  node_t *current_node = t->root;
  while (current_node != t->nil && current_node->right != t->nil)
    current_node = current_node->right;

  return current_node;
}

void rbtree_erase_fixup(rbtree *t, node_t *parent_node, int is_left)
{
  node_t *extra_node = is_left ? parent_node->left : parent_node->right;

  if (extra_node->color == RBTREE_RED)
  {
    extra_node->color = RBTREE_BLACK;
    return;
  }

  node_t *sibling_node = is_left ? parent_node->right : parent_node->left;
  node_t *sibling_left = sibling_node->left;
  node_t *sibling_right = sibling_node->right;

  if (sibling_node->color == RBTREE_BLACK)
  {
    if (is_left)
    {
      if (sibling_right->color == RBTREE_RED)
      {
        left_rotate(t, sibling_node);
        int tmp_color = sibling_node->color;
        sibling_node->color = parent_node->color;
        parent_node->color = (tmp_color == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
        sibling_right->color = RBTREE_BLACK;
      }
      else if (sibling_left->color == RBTREE_RED && sibling_right->color == RBTREE_BLACK)
      {
        sibling_node->color = RBTREE_RED;
        if (parent_node == t->root)
          return;
        int is_parent_left;
        is_parent_left = (parent_node->parent->left == parent_node) ? 1 : 0;
        rbtree_erase_fixup(t, parent_node->parent, is_parent_left);
      }
    }
    else
    {
      if (sibling_left->color == RBTREE_RED)
      {
        right_rotate(t, sibling_node);
        int tmp_color = sibling_node->color;
        sibling_node->color = parent_node->color;
        parent_node->color = (tmp_color == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
        sibling_left->color = RBTREE_BLACK;
      }
      else if (sibling_right->color == RBTREE_RED && sibling_left->color == RBTREE_BLACK)
      {
        left_rotate(t, sibling_node->right);
        int tmp_color = sibling_node->color;
        sibling_node->color = sibling_right->color;
        sibling_right->color = (tmp_color == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
        rbtree_erase_fixup(t, parent_node, is_left);
      }
      else if (sibling_left->color == RBTREE_BLACK && sibling_right->color == RBTREE_BLACK)
      {
        sibling_node->color = RBTREE_RED;
        if (parent_node == t->root)
          return;
        int is_parent_left;
        is_parent_left = (parent_node->parent->left == parent_node) ? 1 : 0;
        rbtree_erase_fixup(t, parent_node->parent, is_parent_left);
      }
    }
  }
  else
  {
    if (is_left)
      left_rotate(t, sibling_node);
    else
      right_rotate(t, sibling_node);
    int tmp_color = sibling_node->color;
    sibling_node->color = parent_node->color;
    parent_node->color = (tmp_color == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
    rbtree_erase_fixup(t, parent_node, is_left);
  }
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *right_node = p->right;
  node_t *left_node = p->left;
  node_t *removed_node_parent;

  int is_removed_black;
  int is_removed_node_left;

  if (right_node != t->nil && left_node != t->nil)
  {
    node_t *successor_node = get_next_node(t, p);
    p->key = successor_node->key;
    removed_node_parent = successor_node->parent;
    if (removed_node_parent->left == successor_node)
    {
      if (successor_node->right != t->nil)
      {
        removed_node_parent->left = successor_node->right;
        successor_node->right->parent = removed_node_parent;
      }
      else
      {
        removed_node_parent->left = t->nil;
      }
      is_removed_node_left = 1;
    }
    else
    {
      if (successor_node->right != t->nil)
      {
        removed_node_parent->right = successor_node->right;
        successor_node->right->parent = removed_node_parent;
      }
      else
      {
        removed_node_parent->right = t->nil;
      }
      is_removed_node_left = 0;
    }
    is_removed_black = successor_node->color ? 1 : 0;

    free(successor_node);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
