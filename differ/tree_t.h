/*!
 * \brief tree.h
 * \author Exdev
 * \version 1.0
 */

#ifndef TREE_T_H_
#define TREE_T_H_

#include <stdint.h>

typedef double val_t;

enum OPERATORS
{
	#define OPERATOR(sym, name, act, outform) name,
	OPERATOR_NULL = 0,
	#include "operators.h"
	#undef OPERATOR
};

enum DATA_TYPE
{
	TYPE_NULL = 0,
	X,
	OPER,
	INT,
	DOUB,
};

struct node_t
{
	uint8_t data_type;
	val_t val;
	struct node_t *left;
	struct node_t *right;
};

struct node_t *node_ctor();
struct node_t *node_ch_init(int type, char *name);
struct node_t *node_int_init(int val);
struct node_t *node_init(uint8_t type, val_t val);
struct node_t *node_doub_init(double val);
struct node_t *node_inread(struct node_t *tree, char **intext);
struct node_t *node_copy(struct node_t *tree);
void node_dtor(struct node_t *tree);
void node_set_data_str(struct node_t *tree, char* val);
void node_set_left_str(struct node_t *tree, char* val);
void node_set_right_str(struct node_t *tree, char* val);
val_t node_get_data(struct node_t *tree);
char *node_get_data_ch(struct node_t *tree);
val_t node_get_left(struct node_t *tree);
val_t node_get_right(struct node_t *tree);

void node_dump(struct node_t *tree);
void dump_rec(struct node_t *tree, char **dump_text, int lbl_cur);
void dump_write(struct node_t *tree, char**dump_text);

#endif /* TREE_T_H_ */
