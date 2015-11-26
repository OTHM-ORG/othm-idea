#include <stdio.h>
#include "othm_idea.h"

OTHM_SYMBOL_INIT(black);
OTHM_SYMBOL_INIT(hood);
OTHM_SYMBOL_INIT(scythe);
OTHM_SYMBOL_INIT(reaper);
OTHM_SYMBOL_INIT(Death);

OTHM_KEYWORD_INIT(color);
OTHM_KEYWORD_INIT(worn);
OTHM_KEYWORD_INIT(held);
OTHM_KEYWORD_INIT(body);
OTHM_KEYWORD_INIT(being);


void test_funct(struct othm_context_control *control,
		   struct othm_idea *to_be,
		   struct othm_idea *a,
		   struct othm_idea *b)
{
	control->allow = 1;
}

int main(int argc, char **args)
{
	struct othm_context *c = othm_context_new(NULL, test_funct);
	struct othm_idea *black = othm_idea_new(NULL, OTHM_SYMBOL(black),
						OTHM_KEYWORD(color));
	struct othm_idea *hood = othm_idea_new(NULL, OTHM_SYMBOL(hood),
					       OTHM_KEYWORD(worn));
	struct othm_idea *scythe = othm_idea_new(NULL, OTHM_SYMBOL(scythe),
						 OTHM_KEYWORD(held));
	struct othm_idea *reaper = othm_idea_new(NULL, OTHM_SYMBOL(reaper),
						 OTHM_KEYWORD(body));

	struct othm_idea *Death = othm_idea_new(NULL, OTHM_SYMBOL(Death),
						 OTHM_KEYWORD(being));

	othm_context_add(c, NULL, black, hood, scythe, reaper, NULL);
	othm_context_print(c);
	printf("\n");
	othm_context_manifest(c, Death);
	othm_idea_print(Death);
	/* printf("Hello, world!"); */
	return 0;
}
