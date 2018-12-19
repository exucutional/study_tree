/*!
 * \brief operators.h
 * \author Exdev
 * \version 0.8
 */

#ifndef OPERATOR_H_

#define NO_BR 0
#define BR 1
#define DBR 2
#define TBR 3

#endif //OPERATOR_H_

OPERATOR(+, PLUS, dif_plus, NO_BR)
OPERATOR(-, MINUS, dif_minus, NO_BR)
OPERATOR(*, MUL, dif_mul, BR)
OPERATOR(/, DIV, dif_div, DBR)
OPERATOR(sin, SIN, dif_sin, TBR)
OPERATOR(tg, TAN, dif_tan, TBR)
OPERATOR(cos, COS, dif_cos, TBR)
OPERATOR(ctg, CTAN, dif_ctan, TBR)
