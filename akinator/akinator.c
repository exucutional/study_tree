/*!
 * \brief akinator.c
 * \author Exdev
 * \version 0.9
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "tree_t.h"
#include "akinator.h"

const int DATA_SIZE = 1024;

int main(int argc, char *argv[])
{
	struct node_t *tree = NULL;
	tree = load_database(tree, argv[1]);
	search(tree);
	tree_dump(tree);
	unload_database(tree, argv[1]);
	tree_dtor(tree);
	printf("End.");
	return 0;
}

struct node_t *load_database(struct node_t *tree, char *path)
{
	FILE *fin;
	fin = fopen(path, "r");
	assert(fin);
	char *dtbs = (char*)calloc(DATA_SIZE, sizeof(char));
	fread(dtbs, DATA_SIZE, 1, fin);
	tree = tree_load(tree, &dtbs);
	fclose(fin);
	return tree;
}

struct node_t *tree_load(struct node_t *tree, char **dtbs)
{
	while(**dtbs != '\0') {
		switch(**dtbs) {
		case '{':
			if (tree == NULL) {
				tree = (struct node_t *)calloc(1, sizeof(struct node_t));
				set_data(tree, getword(dtbs));
				break;
			}
			if (tree->left == NULL) {
				tree->left = tree_load(tree->left, dtbs);
				break;
			}
			tree->right = tree_load(tree->right, dtbs);
			break;
		case '}':
			if (tree != NULL)
				*dtbs+= 1;
			return tree;
			break;
		default:
			*dtbs+= 1;
			break;
		}
	}
	return tree;
}

char *getword(char **dtbs)
{
	*dtbs += 1;
	int i = 0;
	char *word = (char *)calloc(128, sizeof(char));
	while(!isspace(**dtbs)) {
		word[i] = **dtbs;
		i++;
		*dtbs += 1;
	}
	word[i] = '\0';
	return word;
}

void search(struct node_t *tree)
{
	char ans = ' ';
	if(tree->left != NULL || tree->right != NULL)
	{
		printf("%s? [y/n] \n", tree->data);
		ans = getchar();
		getchar();
		switch(ans) {
		case 'y':
			search(tree->left);
			break;
		case 'n':
			search(tree->right);
			break;
		default:
			fputs("Wrong answer\n", stderr);
			break;
		}
	}
	else {
		printf("Answer : %s\n", tree->data);
		tree_increase(tree);
	}

}

void tree_increase(struct node_t *tree)
{
	char ans = ' ';
	char *newans = (char*)calloc(128, sizeof(char));
	char *newquest = (char*)calloc(128, sizeof(char));
	printf("Is it right? [y/n]\n");
	ans = getchar();
	getchar();
	switch(ans) {
	case 'y':
		break;
	case 'n':
		printf("What is the right answer?[no space]\n");
		scanf("%s", newans);
		printf("What is the difference between %s and %s?[no space]\n", tree->data, newans);
		scanf("%s", newquest);
		set_right(tree, tree->data);
		set_data(tree, newquest);
		set_left(tree, newans);
		printf("Database increased.\n");
		break;
	default:
		fputs("Wrong answer\n", stderr);
		break;
	}
}

void unload_database(struct node_t *tree, char *path)
{
	FILE *fout;
	fout = fopen(path, "w");
	assert(fout);
	tree_unload(tree, fout, 0);
}

void tree_unload(struct node_t *tree, FILE *fout, int indent)
{
	if (tree != NULL) {
		fprintf(fout,"%*s{%s\n", indent, "", tree->data);
		tree_unload(tree->left, fout, indent + 4);
		tree_unload(tree->right, fout, indent + 4);
		fprintf(fout, "%*s}\n", indent, "");
	}
}




