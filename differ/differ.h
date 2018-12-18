/*!
 * \brief differ.h
 * \author Exdev
 * \version 0.7
 */

#ifndef DIFFER_H_
#define DIFFER_H_

void diff_run(FILE *fin);
void func_dump(char *eq_text);
void func_rec(struct node_t *tree, char **eq_text);
struct node_t *differ(struct node_t *tree);
struct node_t *dif_add(struct node_t *tree);
struct node_t *dif_sub(struct node_t *tree);
struct node_t *dif_mul(struct node_t *tree);
struct node_t *dif_div(struct node_t *tree);
struct node_t *dif_sin(struct node_t *tree);
struct node_t *dif_cos(struct node_t *tree);
struct node_t *dif_tan(struct node_t *tree);
struct node_t *dif_ctan(struct node_t *tree);
char *read_file(FILE *fin);

#endif /* DIFFER_H_ */
