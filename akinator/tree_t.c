/*!
 * \brief tree.c
 * \author Exdev
 * \version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree_t.h"


struct node_t *node_ctor(struct node_t *tree, data_t init)
{
	tree = (struct node_t *)calloc(1, sizeof(struct node_t));
	if (tree == NULL)
		return tree;
	node_set_data(tree, init);
	return tree;
}

void node_dtor(struct node_t *tree)
{
	if (tree != NULL) {
		node_dtor(tree->left);
		node_dtor(tree->right);
		free(tree);
	}
}

void node_set_data(struct node_t *tree, data_t val)
{
	if(tree->data != NULL)
		free(tree->data);
	tree->data = strdup(val);
}

void node_set_left(struct node_t *tree, data_t val)
{
	assert(tree);
	tree->left = (struct node_t *)calloc(1, sizeof(struct node_t));
	node_set_data(tree->left, val);
}

void node_set_right(struct node_t *tree, data_t val)
{
	assert(tree);
	tree->right = (struct node_t *)calloc(1, sizeof(struct node_t));
	node_set_data(tree->right, val);
}

data_t node_get_data(struct node_t *tree)
{
	assert(tree);
	return(tree->data);
}

data_t node_get_left(struct node_t *tree)
{
	assert(tree);
	return node_get_data(tree->left);
}

data_t node_get_right(struct node_t *tree)
{
	assert(tree);
	return node_get_data(tree->right);
}

void node_dump(struct node_t *tree)
{
	FILE *fout = fopen("tree.dot", "w");
	assert(fout);
	fprintf(fout,"digraph tree {\n");
	dump_write(tree, fout);
	fprintf(fout,"}");
	fclose(fout);
	system("dot -Tpng tree.dot -o tree.png");
	system("xdg-open tree.png");
}

void dump_write(struct node_t *tree, FILE *fout)
{
	assert(tree);
	if(tree->left != NULL && tree->right != NULL) {
		fprintf(fout, "%s -> %s;\n", tree->data, tree->left->data);
		fprintf(fout, "%s -> %s;\n", tree->data, tree->right->data);
		dump_write(tree->left, fout);
		dump_write(tree->right, fout);
	}
}
