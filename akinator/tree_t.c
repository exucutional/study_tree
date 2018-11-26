/*!
 * \brief tree.c
 * \author Exdev
 * \version 0.9
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree_t.h"


struct node_t *tree_ctor(struct node_t *tree, data_t init)
{
	tree = (struct node_t *)calloc(1, sizeof(struct node_t));
	if (tree == NULL)
		return tree;
	set_data(tree, init);
	return tree;
}

void tree_dtor(struct node_t *tree)
{
	if (tree != NULL) {
		tree_dtor(tree->left);
		tree_dtor(tree->right);
		free(tree);
	}
}

void set_data(struct node_t *tree, data_t val)
{
	if(tree->data != NULL)
		free(tree->data);
	tree->data = strdup(val);
}

void set_left(struct node_t *tree, data_t val)
{
	assert(tree);
	tree->left = (struct node_t *)calloc(1, sizeof(struct node_t));
	set_data(tree->left, val);
}

void set_right(struct node_t *tree, data_t val)
{
	assert(tree);
	tree->right = (struct node_t *)calloc(1, sizeof(struct node_t));
	set_data(tree->right, val);
}

data_t get_data(struct node_t *tree)
{
	assert(tree);
	return(tree->data);
}

data_t get_left(struct node_t *tree)
{
	assert(tree);
	return get_data(tree->left);
}

data_t get_right(struct node_t *tree)
{
	assert(tree);
	return get_data(tree->right);
}

void tree_dump(struct node_t *tree)
{
	FILE *fout = fopen("tree.dot", "w");
	assert(fout);
	fprintf(fout,"digraph tree {\n");
	dump_write(tree, fout);
	fprintf(fout,"}");
	fclose(fout);
	system("dot -Tpng tree.dot -o tree.png");
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
