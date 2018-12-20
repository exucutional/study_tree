/*!
 * \brief differ.h
 * \author Exdev
 * \version 0.8
 */

#ifndef DIFFER_H_
#define DIFFER_H_

#define PLUS(left, right)   node_init(OPER, PLUS,  left, right)
#define MINUS(left, right)  node_init(OPER, MINUS, left, right)
#define DIV(left, right)    node_init(OPER, DIV,   left, right)
#define MUL(left, right)    node_init(OPER, MUL,   left, right)
#define SIN(left)          node_init(OPER, SIN,   left, NULL)
#define COS(left)          node_init(OPER, COS,   left, NULL)
#define TAN(left)          node_init(OPER, COS,   left, NULL)
#define CTAN(left)    node_init(OPER, CTAN,  left, NULL)
#define INT(val)    node_init(INT,  val,   NULL, NULL)
#define DOUB(val)   node_init(DOUB, val,   NULL, NULL)
#define VAR(name)   node_init(VAR,  name,  NULL, NULL)

void diff_run(FILE *fin);
void func_dump(char *eq_text);
void func_rec(struct node_t *tree, char **eq_text);
struct node_t *differ(struct node_t *tree);
struct node_t *dif_plus(struct node_t *tree);
struct node_t *dif_minus(struct node_t *tree);
struct node_t *dif_mul(struct node_t *tree);
struct node_t *dif_div(struct node_t *tree);
struct node_t *dif_sin(struct node_t *tree);
struct node_t *dif_cos(struct node_t *tree);
struct node_t *dif_tan(struct node_t *tree);
struct node_t *dif_ctan(struct node_t *tree);
char *read_file(FILE *fin);

#endif /* DIFFER_H_ */
