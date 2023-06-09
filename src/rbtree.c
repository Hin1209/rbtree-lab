#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  tree->nil = nil;
  tree->root = nil;

  return tree;
}

// 현재 노드가 부모 노드의 왼쪽 자식인지 판별하는 함수
int is_node_left(node_t *p)
{
  return (p->parent->left == p);
}

// 두 노드의 color를 바꿔주는 함수
void exchange_color(node_t *p1, node_t *p2)
{
  int tmp_color = p1->color;
  p1->color = p2->color;
  p2->color = (tmp_color == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
}

// node의 메모리를 해제하는 함수
void node_is_free(rbtree *tree, node_t *p)
{
  if (p->left != tree->nil)
    node_is_free(tree, p->left);
  if (p->right != tree->nil)
    node_is_free(tree, p->right);
  free(p);
}

void delete_rbtree(rbtree *tree)
{
  node_t *start = tree->root;
  if (start != tree->nil)
    node_is_free(tree, start);
  free(tree->nil);
  free(tree);
}

// inorder 순서로 현재 노드의 다음 노드를 찾아주는 함수
node_t *get_successor(const rbtree *tree, node_t *p)
{
  if (p == rbtree_max(tree))
    return tree->nil;
  node_t *current_node;
  if (p->right == tree->nil)
  {
    current_node = p;
    while (1)
    {
      if (is_node_left(current_node))
        return current_node->parent;
      else
        current_node = current_node->parent;
    }
  }
  current_node = p->right;

  while (current_node->left != tree->nil)
    current_node = current_node->left;

  return current_node;
}

void right_rotate(rbtree *tree, node_t *node)
{
  node_t *parent_node = node->parent;
  node_t *right_child = node->right;
  node_t *grand_parent_node = parent_node->parent;

  parent_node->left = right_child;
  right_child->parent = parent_node;

  if (parent_node == tree->root)
    tree->root = node;
  else if (parent_node != tree->root)
  {
    if (is_node_left(parent_node))
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
  node_t *grand_parent_node = parent_node->parent;

  parent_node->right = left_child;
  left_child->parent = parent_node;

  if (parent_node == tree->root)
    tree->root = node;
  else if (parent_node != tree->root)
  {
    if (is_node_left(parent_node))
      grand_parent_node->left = node;
    else
      grand_parent_node->right = node;

    node->parent = grand_parent_node;
  }

  node->left = parent_node;
  parent_node->parent = node;
}

// insert 리밸런싱 함수
void rbtree_insert_fixup(rbtree *tree, node_t *node)
{
  if (node == tree->root)
  {
    node->color = RBTREE_BLACK;
    return;
  }

  node_t *parent_node = node->parent;
  node_t *grand_parent_node = parent_node->parent;
  node_t *uncle_node;

  if (parent_node->color == RBTREE_BLACK)
    return;

  // uncle node 설정
  if (is_node_left(parent_node))
    uncle_node = grand_parent_node->right;
  else
    uncle_node = grand_parent_node->left;

  if (uncle_node->color == RBTREE_RED)
  {
    grand_parent_node->color = RBTREE_RED;
    parent_node->color = RBTREE_BLACK;
    uncle_node->color = RBTREE_BLACK;
    rbtree_insert_fixup(tree, grand_parent_node);
  }
  else if (uncle_node->color == RBTREE_BLACK)
  {
    if (is_node_left(parent_node) && is_node_left(node))
    {
      right_rotate(tree, node->parent);
      node->parent->color = RBTREE_BLACK;
      node->parent->right->color = RBTREE_RED;
    }
    else if (is_node_left(parent_node) && !is_node_left(node))
    {
      left_rotate(tree, node);
      right_rotate(tree, node);
      node->color = RBTREE_BLACK;
      node->right->color = RBTREE_RED;
    }
    else if (!is_node_left(parent_node) && is_node_left(node))
    {
      right_rotate(tree, node);
      left_rotate(tree, node);
      node->color = RBTREE_BLACK;
      node->left->color = RBTREE_RED;
    }
    else if (!is_node_left(parent_node) && !is_node_left(node))
    {
      left_rotate(tree, node->parent);
      node->parent->color = RBTREE_BLACK;
      node->parent->left->color = RBTREE_RED;
    }
  }
}

node_t *rbtree_insert(rbtree *tree, const key_t key)
{
  node_t *node = (node_t *)malloc(sizeof(node_t));
  node_t *current_node = tree->root;

  node->key = key;
  node->color = RBTREE_RED;
  node->left = node->right = tree->nil;

  // 삽입할 위치 찾기
  while (current_node != tree->nil)
  {
    if (current_node->key <= key)
    {
      if (current_node->right == tree->nil)
      {
        current_node->right = node;
        break;
      }
      current_node = current_node->right;
    }
    else
    {
      if (current_node->left == tree->nil)
      {
        current_node->left = node;
        break;
      }
      current_node = current_node->left;
    }
  }
  node->parent = current_node;

  if (current_node == tree->nil)
    tree->root = node;

  // 삽입 이후 리밸런싱
  rbtree_insert_fixup(tree, node);
  return node;
}

node_t *rbtree_find(const rbtree *tree, const key_t key)
{
  node_t *current_node = tree->root;
  while (key != current_node->key && current_node != tree->nil)
  {
    if (key < current_node->key)
      current_node = current_node->left;
    else
      current_node = current_node->right;
  }

  return (current_node != tree->nil) ? current_node : NULL;
}

node_t *rbtree_min(const rbtree *tree)
{
  node_t *current_node = tree->root;
  while (current_node != tree->nil && current_node->left != tree->nil)
    current_node = current_node->left;

  return current_node;
}

node_t *rbtree_max(const rbtree *tree)
{
  node_t *current_node = tree->root;
  while (current_node != tree->nil && current_node->right != tree->nil)
    current_node = current_node->right;

  return current_node;
}

// erase 리밸런싱 함수
void rbtree_erase_fixup(rbtree *tree, node_t *parent_node, int is_left)
{
  // is_left를 통해 extra black의 위치 확인
  node_t *sibling_node = is_left ? parent_node->right : parent_node->left;
  node_t *outside_child = is_left ? sibling_node->right : sibling_node->left; // sibling node의 바깥쪽 자식
  node_t *inside_child = is_left ? sibling_node->left : sibling_node->right;  // sibling node의 안쪽 자식

  if (sibling_node->color == RBTREE_RED)
  {
    if (is_left)
      left_rotate(tree, sibling_node);
    else
      right_rotate(tree, sibling_node);
    exchange_color(sibling_node, parent_node);
    rbtree_erase_fixup(tree, parent_node, is_left);
  }
  else if (sibling_node->color == RBTREE_BLACK)
  {
    if (outside_child->color == RBTREE_RED)
    {
      if (is_left)
        left_rotate(tree, sibling_node);
      else
        right_rotate(tree, sibling_node);
      exchange_color(sibling_node, parent_node);
      outside_child->color = RBTREE_BLACK;
    }
    else if (outside_child->color == RBTREE_BLACK && inside_child->color == RBTREE_RED)
    {
      if (is_left)
        right_rotate(tree, inside_child);
      else
        left_rotate(tree, inside_child);
      exchange_color(sibling_node, inside_child);
      rbtree_erase_fixup(tree, parent_node, is_left);
    }
    else if (outside_child->color == RBTREE_BLACK && inside_child->color == RBTREE_BLACK)
    {
      sibling_node->color = RBTREE_RED;
      if (parent_node == tree->root)
        return;
      rbtree_erase_fixup(tree, parent_node->parent, is_node_left(parent_node));
    }
  }
}

// remove node를 successor로 대체하는 함수
node_t *replace_to_successor(rbtree *tree, node_t *p, node_t *successor, node_t *removed_node_parent)
{
  node_t *replace_node = successor->right;

  p->key = successor->key;
  if (is_node_left(successor))
    removed_node_parent->left = successor->right;
  else if (!is_node_left(successor))
    removed_node_parent->right = successor->right;
  successor->right->parent = removed_node_parent;

  return replace_node;
}

// remove node를 child로 대체하는 함수
node_t *replace_to_child(rbtree *tree, node_t *p, node_t *removed_node_parent)
{
  node_t *left_node = p->left;
  node_t *right_node = p->right;
  node_t *replace_node;

  replace_node = right_node;
  if (left_node != tree->nil)
    replace_node = left_node;

  if (is_node_left(p))
    removed_node_parent->left = replace_node;
  else
    removed_node_parent->right = replace_node;
  replace_node->parent = removed_node_parent;
  free(p);
  return replace_node;
}

int rbtree_erase(rbtree *tree, node_t *p)
{
  node_t *right_node = p->right;
  node_t *left_node = p->left;
  node_t *removed_node_parent, *successor_node, *replace_node;

  int is_removed_black;
  int is_left;

  // 삭제할 노드가 자식이 둘인 경우
  if (right_node != tree->nil && left_node != tree->nil)
  {
    successor_node = get_successor(tree, p);
    is_left = is_node_left(successor_node);
    is_removed_black = successor_node->color ? 1 : 0;
    removed_node_parent = successor_node->parent;
    replace_node = replace_to_successor(tree, p, successor_node, removed_node_parent);
    free(successor_node);
  }
  // 삭제할 노드가 자식이 하나거나 없는 경우
  else if (right_node == tree->nil || left_node == tree->nil)
  {
    if (p == tree->root)
    {
      tree->root = (left_node == tree->nil) ? right_node : left_node;
      tree->root->color = RBTREE_BLACK;
      free(p);
      return 0;
    }
    is_left = is_node_left(p);
    is_removed_black = p->color ? 1 : 0;
    removed_node_parent = p->parent;
    replace_node = replace_to_child(tree, p, removed_node_parent);
  }
  if (is_removed_black && replace_node->color == RBTREE_RED)
  {
    replace_node->color = RBTREE_BLACK;
    return 0;
  }
  else if (is_removed_black && replace_node->color == RBTREE_BLACK)
    rbtree_erase_fixup(tree, removed_node_parent, is_left);
  return 0;
}

int rbtree_to_array(const rbtree *tree, key_t *arr, const size_t n)
{
  node_t *current_node = rbtree_min(tree);

  arr[0] = current_node->key;
  for (int i = 1; i < n; i++)
  {
    if (current_node == tree->nil)
      break;
    current_node = get_successor(tree, current_node);
    if (current_node == tree->nil)
      break;
    arr[i] = current_node->key;
  }
  return 0;
}
