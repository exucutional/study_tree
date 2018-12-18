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

#define TRG_WRT(name)	\
	do {	\
		asprintf(eq_text, "%s"#name"(", *eq_text);	\
		func_rec(tree->left, eq_text);	\
		asprintf(eq_text, "%s)", *eq_text);	\
	}	\
	while (0)

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
	//free(intext);
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
		*tree->data = 0;
		return tree;
	case DOUB:
		*tree->data = 0;
		tree->data_type = INT;
		return tree;
	case X:
		free(tree->data);
		tree->data = calloc(1, sizeof(int));
		*tree->data = 1;
		tree->data_type = INT;
		return tree;
	case ADD:
		return dif_add(tree);
	case SUB:
		return dif_sub(tree);
	case MUL:
		return dif_mul(tree);
	case DIV:
		return dif_div(tree);
	case SIN:
		return dif_sin(tree);
	case COS:
		return dif_cos(tree);
	case TAN:
		return dif_tan(tree);
	case CTAN:
		return dif_ctan(tree);
	default:
		assert(0);
		break;
	}
	return tree;
}

struct node_t *dif_add(struct node_t *tree)
{
	assert(tree);
	assert(tree->right);
	assert(tree->left);
	tree->left = differ(tree->left);
	tree->right = differ(tree->right);
	return tree;
}

struct node_t *dif_sub(struct node_t *tree)
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
	tree->data_type = ADD;
	*tree->data = '+';
	tree->left = node_ch_init(MUL, "*");
	tree->right = node_ch_init(MUL, "*");
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
	tree->left = node_ch_init(SUB,"-");
	tree->right = node_ch_init(MUL, "*");
	tree->left->left = node_ch_init(MUL, "*");
	tree->left->right = node_ch_init(MUL, "*");
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
	tree->data_type = MUL;
	free(tree->data);
	tree->data = strdup("*");
	left = tree->left;
	tree->left = node_ch_init(COS, "cos");
	tree->left->left = node_copy(left);
	tree->right = differ(left);
	return tree;
}

struct node_t *dif_cos(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	tree->data_type = MUL;
	free(tree->data);
	tree->data = strdup("*");
	left = tree->left;
	tree->left = node_ch_init(MUL, "*");
	tree->left->left = node_int_init(-1);
	tree->left->right = node_ch_init(SIN, "sin");
	tree->left->right->left = node_copy(left);
	tree->right = differ(left);
	return tree;
}
struct node_t *dif_tan(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	tree->data_type = DIV;
	free(tree->data);
	tree->data = strdup("/");
	left = tree->left;
	tree->left = differ(node_copy(left));
	tree->right = node_ch_init(MUL, "*");
	tree->right->left = node_ch_init(COS, "cos");
	tree->right->left->left = node_copy(left);
	tree->right->right = node_ch_init(COS, "cos");
	tree->right->right->left = node_copy(left);
	free(left);
	return tree;
}
struct node_t *dif_ctan(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	tree->data_type = DIV;
	free(tree->data);
	tree->data = strdup("/");
	left = tree->left;
	tree->left = node_ch_init(MUL, "*");
	tree->left->left = node_int_init(-1);
	tree->left->right = differ(node_copy(left));
	tree->right = node_ch_init(MUL, "*");
	tree->right->left = node_ch_init(SIN, "sin");
	tree->right->left->left = node_copy(left);
	tree->right->right = node_ch_init(SIN, "sin");
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
	int l_t = 0;
	int r_t = 0;
	if (!tree)
		return;
	switch(tree->data_type) {
	case ADD:
		func_rec(tree->left, eq_text);
		asprintf(eq_text, "%s+", *eq_text);
		func_rec(tree->right, eq_text);
		break;
	case SUB:
		func_rec(tree->left, eq_text);
		asprintf(eq_text, "%s-", *eq_text);
		func_rec(tree->right, eq_text);
		break;
	case MUL:
		l_t = tree->left->data_type;
		r_t = tree->right->data_type;
		//brackets
		int left_no_br = (l_t != ADD && l_t != SUB);
		int right_no_br = (r_t != ADD && r_t != SUB);
		if (!left_no_br) {
			asprintf(eq_text, "%s(", *eq_text);
			func_rec(tree->left, eq_text);
			asprintf(eq_text, "%s)", *eq_text);
		} else {
			func_rec(tree->left, eq_text);
		}
		asprintf(eq_text, "%s*", *eq_text);
		if(!right_no_br) {
			asprintf(eq_text, "%s(", *eq_text);
			func_rec(tree->right, eq_text);
			asprintf(eq_text, "%s)", *eq_text);
		} else {
			func_rec(tree->right, eq_text);
		}
		break;
	case DIV:
		asprintf(eq_text, "%s\\frac{", *eq_text);
		func_rec(tree->left, eq_text);
		asprintf(eq_text, "%s}{", *eq_text);
		func_rec(tree->right, eq_text);
		asprintf(eq_text, "%s}", *eq_text);
		break;
	case INT:
		asprintf(eq_text, "%s%d", *eq_text, *(int*)tree->data);
		break;
	case DOUB:
		asprintf(eq_text, "%s%lf", *eq_text, *(double*)tree->data);
		break;
	case X:
		asprintf(eq_text, "%sx", *eq_text);
		break;
	case SIN:
		TRG_WRT(sin);
		break;
	case COS:
		TRG_WRT(cos);
		break;
	case TAN:
		TRG_WRT(tg);
		break;
	case CTAN:
		TRG_WRT(ctg);
		break;
	default:
		break;
	}
}
