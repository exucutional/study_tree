/*!
 * \brief tree.h
 * \author Exdev
 * \version 0.9
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

struct node_t *tree_ctor(struct node_t *tree, data_t init);
void tree_dtor(struct node_t *tree);
void set_data(struct node_t *tree, data_t val);
void set_left(struct node_t *tree, data_t val);
void set_right(struct node_t *tree, data_t val);
data_t get_data(struct node_t *tree);
data_t get_left(struct node_t *tree);
data_t get_right(struct node_t *tree);

void tree_dump(struct node_t *tree);
void dump_write(struct node_t *tree, FILE *fout);

#endif /* TREE_T_H_ */
