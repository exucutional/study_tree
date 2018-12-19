/*!
 * \brief differ.c
 * \author Exdev
 * \version 0.7
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include "tree_t.h"
#include "differ.h"

int main(int argc, char *argv[])
{
	FILE *fin = fopen(argv[1], "r");
	diff_run(fin);
	fclose(fin);
	return 0;
}

void diff_run(FILE *fin)
{
	assert(fin);
	struct node_t *tree = NULL;
	char *intext = NULL;
	intext = read_file(fin);
	assert(intext);
	tree = node_inread(tree, &intext);
	assert(tree);
	char *eq_text = NULL;
	asprintf(&eq_text, "\\documentclass[12pt,a4paper]{article}\n\\usepackage{amsmath}\n\\begin{document}\n$$f(x) = ");
	func_rec(tree, &eq_text);
	asprintf(&eq_text, "%s$$\n", eq_text);
	tree = differ(tree);
	assert(tree);
	asprintf(&eq_text, "%s$$f'(x) = ", eq_text);
	func_rec(tree, &eq_text);
	asprintf(&eq_text,"%s$$\n\\end{document}", eq_text);
	node_dump(tree);
	func_dump(eq_text);
	node_dtor(tree);
	free(eq_text);
}

char *read_file(FILE *fin)
{
	assert(fin);
	struct stat buff;
	fstat(fileno(fin), &buff);
	char *intext = calloc(buff.st_size, sizeof(char));
	fread(intext, buff.st_size, 1, fin);
	return intext;
}

struct node_t *differ(struct node_t *tree)
{
	assert(tree);
	switch(tree->data_type) {
	case INT:
		tree->val = 0;
		break;
	case DOUB:
		tree->val = 0;
		tree->data_type = INT;
		break;
	case X:
		tree->val = 1;
		tree->data_type = INT;
		break;
	case OPER:
		switch((int)tree->val) {
		#define OPERATOR(sym, name, act, outform)	\
		case name:	\
			return act(tree);
		#include"operators.h"
		#undef OPERATOR
		default:
			fprintf(stderr, "Unknown operator %d\n", (int)tree->val);
			assert(0);
		}
		break;
	default:
		fprintf(stderr, "Unknown data type %d\n", tree->data_type);
		assert(0);
	}
	return tree;
}

struct node_t *dif_plus(struct node_t *tree)
{
	assert(tree);
	assert(tree->right);
	assert(tree->left);
	tree->left = differ(tree->left);
	tree->right = differ(tree->right);
	return tree;
}

struct node_t *dif_minus(struct node_t *tree)
{
	assert(tree);
	assert(tree->right);
	assert(tree->left);
	tree->left = differ(tree->left);
	tree->right = differ(tree->right);
	return tree;
}

struct node_t *dif_mul(struct node_t *tree)
{
	struct node_t *left = NULL;
	struct node_t *right = NULL;
	assert(tree);
	assert(tree->right);
	assert(tree->left);
	left = tree->left;
	right = tree->right;
	tree->val = MUL;
	tree->left = node_init(OPER, MUL);
	tree->right = node_init(OPER, MUL);
	tree->left->left = differ(node_copy(left));
	tree->left->right = node_copy(right);
	tree->right->right = differ(node_copy(right));
	tree->right->left = node_copy(left);
	free(left);
	free(right);
	return tree;
}

struct node_t *dif_div(struct node_t *tree)
{
	struct node_t *left = NULL;
	struct node_t *right = NULL;
	assert(tree);
	assert(tree->left);
	assert(tree->right);
	left = tree->left;
	right = tree->right;
	tree->left = node_init(OPER, MINUS);
	tree->right = node_init(OPER, MUL);
	tree->left->left = node_init(OPER, MUL);
	tree->left->right = node_init(OPER, MUL);
	tree->left->left->left = differ(node_copy(left));
	tree->left->left->right = node_copy(right);
	tree->left->right->left = node_copy(left);
	tree->left->right->right = differ(node_copy(right));
	tree->right->left = node_copy(right);
	tree->right->right = node_copy(right);
	free(left);
	free(right);
	return tree;
}

struct node_t *dif_sin(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	tree->val = MUL;
	left = tree->left;
	tree->left = node_init(OPER, COS);
	tree->left->left = node_copy(left);
	tree->right = differ(left);
	return tree;
}

struct node_t *dif_cos(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	tree->val = MUL;
	left = tree->left;
	tree->left = node_init(OPER, MUL);
	tree->left->left = node_init(INT, -1);
	tree->left->right = node_init(OPER, SIN);
	tree->left->right->left = node_copy(left);
	tree->right = differ(left);
	return tree;
}
struct node_t *dif_tan(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	tree->val = DIV;
	left = tree->left;
	tree->left = differ(node_copy(left));
	tree->right = node_init(OPER, MUL);
	tree->right->left = node_init(OPER, COS);
	tree->right->left->left = node_copy(left);
	tree->right->right = node_init(OPER, COS);
	tree->right->right->left = node_copy(left);
	free(left);
	return tree;
}
struct node_t *dif_ctan(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	tree->val = DIV;
	left = tree->left;
	tree->left = node_init(OPER, MUL);
	tree->left->left = node_init(INT, -1);
	tree->left->right = differ(node_copy(left));
	tree->right = node_init(OPER, MUL);
	tree->right->left = node_init(OPER, SIN);
	tree->right->left->left = node_copy(left);
	tree->right->right = node_init(OPER, SIN);
	tree->right->right->left = node_copy(left);
	free(left);
	return tree;
}

void func_dump(char *eq_text)
{
	FILE *fout = fopen("func.tex", "w");
	assert(fout);
	fprintf(fout, "%s", eq_text);
	system("pdftex -interaction=nonstopmode func.tex");
	system("xdg-open func.pdf");
	fclose(fout);
}

void func_rec(struct node_t *tree, char **eq_text)
{
	if (!tree)
		return;
	switch(tree->data_type) {
	case INT:
		asprintf(eq_text, "%s%d", *eq_text, (int)tree->val);
		break;
	case DOUB:
		asprintf(eq_text, "%s%lf", *eq_text, (double)tree->val);
		break;
	case X:
		asprintf(eq_text, "%sx", *eq_text);
		break;
	case OPER:
		switch((int)tree->val) {
		#define OPERATOR(sym, name, act, outform)	\
		case name:	\
			switch(outform) {	\
			case NO_BR:	\
				func_rec(tree->left, eq_text);	\
				asprintf(eq_text, "%s"#sym, *eq_text);	\
				func_rec(tree->right, eq_text);	\
				break;	\
			case BR:	\
				asprintf(eq_text, "%s(", *eq_text);	\
				func_rec(tree->left, eq_text);	\
				asprintf(eq_text, ")%s"#sym"(", *eq_text);	\
				func_rec(tree->right, eq_text);	\
				asprintf(eq_text, "%s)", *eq_text);	\
				break;	\
			case DBR:	\
				asprintf(eq_text, "%s\\frac{", *eq_text);	\
				func_rec(tree->left, eq_text);	\
				asprintf(eq_text, "%s}{", *eq_text);	\
				func_rec(tree->right, eq_text);	\
				asprintf(eq_text, "%s}", *eq_text);	\
				break;	\
			case TBR:	\
				asprintf(eq_text, "%s"#sym"(", *eq_text);	\
				func_rec(tree->left, eq_text);	\
				asprintf(eq_text, "%s)", *eq_text);	\
				break;	\
			default:	\
				fprintf(stderr, "Unknown outform %d\n", outform);	\
				assert(0);	\
			}	\
			break;
			#include "operators.h"
			break;
		default:
			fprintf(stderr, "Unknown operator %d\n", (int)tree->val);
			assert(0);
		}
		break;
	default:
		fprintf(stderr, "Unknown data type %d\n", tree->data_type);
		assert(0);
	}
}

