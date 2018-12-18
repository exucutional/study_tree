/*!
 * \brief tree.c
 * \author Exdev
 * \version 1.2
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tree_t.h"
#include <ctype.h>

#define WRITE_DATA(type, data)	\
	do {	\
		switch (type) {	\
		case INT:	\
			asprintf(dump_text, "%s%d];\n", *dump_text, *(int*)data);	\
			break;	\
		case DOUB:	\
			asprintf(dump_text, "%s%lf];\n", *dump_text, *(double*)data);	\
			break;	\
		default:	\
			asprintf(dump_text, "%s\"%s\"];\n", *dump_text, (char*)data);	\
			break;	\
		}	\
	} while (0)

#define NODE_CH_WRT(type, name)	\
	do {	\
		tree->data = strdup(name);	\
		tree->data_type = type;	\
	} while (0)

struct node_t *node_ctor()
{
	struct node_t *tree = calloc(1, sizeof(struct node_t));
	if (tree == NULL)
		return tree;
	return tree;
}

struct node_t *node_ch_init(int type, char* name)
{
	struct node_t *tree = calloc(1, sizeof(struct node_t));
	tree->data = strdup(name);
	tree->data_type = type;
	return tree;
}

struct node_t *node_int_init(int val)
{
	struct node_t *tree = calloc(1, sizeof(struct node_t));
	tree->data = calloc(1, sizeof(int));
	*(int *)tree->data = val;
	tree->data_type = INT;
	return tree;
}

struct node_t *node_doub_init(double val)
{
	struct node_t *tree = calloc(1, sizeof(struct node_t));
	tree->data = calloc(1, sizeof(double));
	*tree->data = val;
	tree->data_type = DOUB;
	return tree;
}

struct node_t *node_copy(struct node_t *tree)
{
	if (!tree)
		return NULL;
	struct node_t *copy = NULL;
	switch(tree->data_type) {
	case INT:
		copy = node_int_init(*(int*)tree->data);
		break;
	case DOUB:
		copy = node_doub_init(*(double*)tree->data);
		break;
	case X:
		copy = node_ch_init(X, "x");
		break;
	case ADD:
		copy = node_ch_init(ADD, "+");
		break;
	case SUB:
		copy = node_ch_init(SUB, "-");
		break;
	case MUL:
		copy = node_ch_init(MUL, "*");
		break;
	case DIV:
		copy = node_ch_init(DIV, "/");
		break;
	case SIN:
		copy = node_ch_init(SIN, "sin");
		break;
	case COS:
		copy = node_ch_init(COS, "cos");
		break;
	case TAN:
		copy = node_ch_init(TAN, "tg");
		break;
	case CTAN:
		copy = node_ch_init(CTAN, "ctg");
		break;
	default:
		assert(0);
		break;
	}
	copy->left = node_copy(tree->left);
	copy->right = node_copy(tree->right);
	return copy;
}



struct node_t *node_inread(struct node_t *tree, char **intext)
{
	//((1)+(4))
	double tmp = 0;
	assert(*intext);
	while (**intext != '\0') {
		//printf("%s ", *intext);
		switch(**intext) {
		case '(':
			if (tree == NULL) {
				(*intext)++;
				tree = calloc(1, sizeof(struct node_t));
				break;
			}
			if (tree->left == NULL) {
				tree->left = node_inread(tree->left, intext);
				break;
			}
			tree->right = node_inread(tree->right, intext);
			break;
		case ')':
			(*intext)++;
			return tree;
			break;
		case '+':
			NODE_CH_WRT(ADD, "+");
			(*intext)++;
			break;
		case '-':
			NODE_CH_WRT(SUB, "-");
			(*intext)++;
			break;
		case '*':
			NODE_CH_WRT(MUL, "*");
			(*intext)++;
			break;
		case '/':
			NODE_CH_WRT(DIV, "/");
			(*intext)++;
			break;
		case 'x':
			NODE_CH_WRT(X, "x");
			(*intext)++;
			break;
		case 's':
			if (strncmp((*intext), "sin", 3) == 0) {
				NODE_CH_WRT(SIN, "sin");
				(*intext) += 3;
			}
			break;
		case 'c':
			if (strncmp((*intext), "cos", 3) == 0) {
				NODE_CH_WRT(COS, "cos");
				(*intext) += 3;
			}
			if (strncmp((*intext), "ctg", 3) == 0) {
				NODE_CH_WRT(CTAN, "ctg");
				(*intext) += 3;
			}
			break;
		case 't':
			if (strncmp((*intext), "tg", 2) == 0) {
				NODE_CH_WRT(TAN, "tg");
				(*intext) += 2;
			}
			break;
		default:
			if (isdigit(**intext)) {
				sscanf(*intext ,"%lf", &tmp);
				if (tmp - (int)tmp != 0) {
					tree->data = calloc(1, sizeof(double));
					*(double*)tree->data = tmp;
					tree->data_type = DOUB;
				} else {
					tree->data = calloc(1, sizeof(int));
					*(int*)tree->data = tmp;
					tree->data_type = INT;
				}
				while(isdigit(**intext))
					(*intext)++;
			}
			else {
				(*intext)++;
			}
			break;
		}
	}
	return tree;
}

void node_dtor(struct node_t *tree)
{
	if (tree != NULL) {
		node_dtor(tree->left);
		node_dtor(tree->right);
		free(tree->data);
		free(tree);
	}
}

void node_set_data_str(struct node_t *tree, char *val)
{
	if(tree->data != NULL)
		free(tree->data);
	tree->data = strdup(val);
}

void node_set_left_str(struct node_t *tree, char *val)
{
	assert(tree);
	tree->left = (struct node_t *)calloc(1, sizeof(struct node_t));
	node_set_data_str(tree->left, val);
}

void node_set_right_str(struct node_t *tree, char *val)
{
	assert(tree);
	tree->right = (struct node_t *)calloc(1, sizeof(struct node_t));
	node_set_data_str(tree->right, val);
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
	char *dump_text = NULL;
	dump_rec(tree, &dump_text, 0);
	assert(dump_text);
	fprintf(fout, dump_text);
	fprintf(fout,"}");
	fclose(fout);
	free(dump_text);
	system("dot -Tpng tree.dot -o tree.png");
	system("xdg-open tree.png");
}

void dump_rec(struct node_t *tree, char **dump_text, int lbl_cur)
{
	static int lbl_max;
	if (!tree)
		return;
	if (*dump_text == NULL)
		*dump_text = calloc(1, (sizeof(char)));
	if(tree->left != NULL || tree->right != NULL) {
		asprintf(dump_text, "%s%d [label=", *dump_text, lbl_cur);
		WRITE_DATA(tree->data_type, tree->data);
		if (tree->left != NULL) {
			asprintf(dump_text, "%s%d [label=", *dump_text, lbl_max + 1);
			WRITE_DATA(tree->left->data_type, tree->left->data);
			asprintf(dump_text, "%s%d -> %d;\n", *dump_text, lbl_cur, lbl_max + 1);
		}
		if (tree->right != NULL) {
			asprintf(dump_text, "%s%d [label=", *dump_text, lbl_max + 2);
			WRITE_DATA(tree->right->data_type, tree->right->data);
			asprintf(dump_text, "%s%d -> %d;\n", *dump_text, lbl_cur, lbl_max + 2);
		}
		//asprintf(dump_text, "%s%d -> %d;\n", *dump_text, lbl_cur, lbl_max + 1);
		//asprintf(dump_text, "%s%d -> %d;\n", *dump_text, lbl_cur, lbl_max + 2);
		lbl_cur = lbl_max;
		lbl_max += 3;
		dump_rec(tree->left, dump_text, lbl_cur + 1);
		dump_rec(tree->right, dump_text, lbl_cur + 2);
	}
}
