/*!
 * \brief tree.h
 * \author Exdev
 * \version 1.0
 */

#ifndef TREE_T_H_
#define TREE_T_H_



#include <stdint.h>

typedef char* data_t;

enum DATA_TYPES
{
	X = 'x',
	ADD = '+',
	SUB = '-',
	MUL = '*',
	DIV = '/',
	INT = 0,
	DOUB,
	SIN,
	COS,
	TAN,
	CTAN,
};

struct node_t
{
	uint8_t data_type;
	data_t data;
	struct node_t *left;
	struct node_t *right;
};

struct node_t *node_ctor();
struct node_t *node_ch_init(int type, char *name);
struct node_t *node_int_init(int val);
struct node_t *node_doub_init(double val);
struct node_t *node_inread(struct node_t *tree, char **intext);
struct node_t *node_copy(struct node_t *tree);
void node_dtor(struct node_t *tree);
void node_set_data_str(struct node_t *tree, char* val);
void node_set_left_str(struct node_t *tree, char* val);
void node_set_right_str(struct node_t *tree, char* val);
data_t node_get_data(struct node_t *tree);
char *node_get_data_ch(struct node_t *tree);
data_t node_get_left(struct node_t *tree);
data_t node_get_right(struct node_t *tree);

void node_dump(struct node_t *tree);
void dump_rec(struct node_t *tree, char **dump_text, int lbl_cur);

#endif /* TREE_T_H_ */
