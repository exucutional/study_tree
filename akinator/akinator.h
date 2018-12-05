/*!
 * \brief akinator.h
 * \author Exdev
 * \version 1.0
 */

#ifndef AKINATOR_H_
#define AKINATOR_H_

void akin_run(char *path);
void search(struct node_t *tree);
void tree_increase(struct node_t *tree);
struct node_t *load_database(struct node_t *tree, char *path);
void unload_database(struct node_t *tree, char *path);
struct node_t *tree_load(struct node_t *tree, char **dtbs);
void tree_unload(struct node_t *tree, char **dtbs_out, int indent);
char *getword(char **dtbs);

#endif /* AKINATOR_H_ */
