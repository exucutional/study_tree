/*!
 * \brief tree.h
 * \author Exdev
 * \version 1.0
 */

#ifndef TREE_T_H_
#define TREE_T_H_

typedef char* data_t;

struct node_t
{
	data_t data;
	struct node_t *left;
	struct node_t *right;
};

struct node_t *node_ctor(struct node_t *tree, data_t init);
void node_dtor(struct node_t *tree);
void node_set_data(struct node_t *tree, data_t val);
void node_set_left(struct node_t *tree, data_t val);
void node_set_right(struct node_t *tree, data_t val);
data_t node_get_data(struct node_t *tree);
data_t node_get_left(struct node_t *tree);
data_t node_get_right(struct node_t *tree);

void node_dump(struct node_t *tree);
void dump_write(struct node_t *tree, FILE *fout);

#endif /* TREE_T_H_ */
