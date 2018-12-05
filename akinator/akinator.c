/*!
 * \brief akinator.c
 * \author Exdev
 * \version 1.0
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include "tree_t.h"
#include "akinator.h"

const int DATA_SIZE = 1024;

int main(int argc, char *argv[])
{
	akin_run(argv[1]);
	return 0;
}

void akin_run(char *path)
{
	printf("---Running---\n");
	struct node_t *tree = NULL;
	tree = load_database(tree, path);
	search(tree);
	node_dump(tree);
	unload_database(tree, path);
	node_dtor(tree);
	printf("---End---\n");
}

struct node_t *load_database(struct node_t *tree, char *path)
{
	FILE *fin;
	fin = fopen(path, "r");
	assert(fin);
	char *dtbs = (char*)calloc(DATA_SIZE, sizeof(char));
	fread(dtbs, DATA_SIZE, 1, fin);
	tree = tree_load(tree, &dtbs);
	assert(tree);
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
				node_set_data(tree, getword(dtbs));
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
		node_set_right(tree, tree->data);
		node_set_data(tree, newquest);
		node_set_left(tree, newans);
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
	char *dtbs_out = NULL;
	tree_unload(tree, &dtbs_out, 0);
	fprintf(fout, "%s", dtbs_out);
	fclose(fout);
}

void tree_unload(struct node_t *tree, char **dtbs_out, int indent)
{
	if (tree != NULL) {
		if (*dtbs_out != NULL)
			asprintf(dtbs_out, "%s%*s{%s\n", *dtbs_out, indent, "", tree->data);
		else
			asprintf(dtbs_out, "%*s{%s\n", indent, "", tree->data);
		tree_unload(tree->left, dtbs_out, indent + 4);
		tree_unload(tree->right, dtbs_out, indent + 4);
		asprintf(dtbs_out, "%s%*s}\n", *dtbs_out, indent, "");
	}
}




