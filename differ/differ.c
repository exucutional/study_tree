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
	case VAR:
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
	left = MUL(differ(node_copy(tree->left)), tree->right);
	right = MUL(tree->left, differ(node_copy(tree->right)));
	free(tree);
	return PLUS(left, right);
}

struct node_t *dif_div(struct node_t *tree)
{
	struct node_t *left = NULL;
	struct node_t *right = NULL;
	assert(tree);
	assert(tree->left);
	assert(tree->right);
	left = MINUS(MUL(differ(node_copy(tree->left)), tree->right), MUL(tree->left, differ(node_copy(tree->right))));
	right = MUL(node_copy(tree->right), node_copy(tree->right));
	free(tree);
	return DIV(left, right);
}

struct node_t *dif_sin(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	left = tree->left;
	free(tree);
	return MUL(COS(left), differ(node_copy(left)));
}

struct node_t *dif_cos(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	left = tree->left;
	free(tree);
	return MUL(MUL(INT(-1), SIN(left)), differ(node_copy(left)));
}
struct node_t *dif_tan(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	left = tree->left;
	free(tree);
	return DIV(differ(node_copy(left)), MUL(COS(node_copy(left)), COS(left)));
}
struct node_t *dif_ctan(struct node_t *tree)
{
	struct node_t *left = NULL;
	assert(tree);
	assert(tree->left);
	left = tree->left;
	free(tree);
	return DIV(MUL(INT(-1), differ(node_copy(left))), MUL(SIN(node_copy(left)), SIN(left)));
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
	case INT:
		asprintf(eq_text, "%s%d", *eq_text, (int)tree->val);
		break;
	case DOUB:
		asprintf(eq_text, "%s%lf", *eq_text, (double)tree->val);
		break;
	case VAR:
		switch((int)tree->val) {
		#define VARIABLE(sym, name)	\
		case name:	\
			asprintf(eq_text, "%s"#sym, *eq_text);	\
			break;
		#include"variables.h"
		#undef VARIABLE
		}
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
					l_t = tree->left->val;	\
					r_t = tree->right->val;	\
					int left_no_br = (l_t != PLUS && l_t != MINUS && tree->left->val >= 0);	\
					int right_no_br = (r_t != PLUS && r_t != MINUS && tree->right->val >= 0);	\
					if (!left_no_br) {	\
						asprintf(eq_text, "%s(", *eq_text);	\
						func_rec(tree->left, eq_text);	\
						asprintf(eq_text, "%s)", *eq_text);	\
					} else {	\
						func_rec(tree->left, eq_text);	\
					}	\
					asprintf(eq_text, "%s*", *eq_text);	\
					if(!right_no_br) {	\
						asprintf(eq_text, "%s(", *eq_text);	\
						func_rec(tree->right, eq_text);	\
						asprintf(eq_text, "%s)", *eq_text);	\
					} else {	\
						func_rec(tree->right, eq_text);	\
					}	\
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

