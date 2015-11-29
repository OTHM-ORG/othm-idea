#include <stdio.h>
#include "othm_idea.h"

OTHM_SYMBOL_INIT(black);
OTHM_SYMBOL_INIT(hood);
OTHM_SYMBOL_INIT(scythe);
OTHM_SYMBOL_INIT(reaper);
OTHM_SYMBOL_INIT(Death);

OTHM_SYMBOL_INIT(context);

OTHM_KEYWORD_INIT(color);
OTHM_KEYWORD_INIT(worn);
OTHM_KEYWORD_INIT(held);
OTHM_KEYWORD_INIT(body);
OTHM_KEYWORD_INIT(being);

OTHM_SYMBOL_INIT(dreadium);

/* OTHM_KEYWORD_INIT(thing); */
/* OTHM_SYMBOL_INIT(abstract); */
/* OTHM_SYMBOL_INIT(concrete); */

/* OTHM_KEYWORD_INIT(description); */


int main(int argc, char **args)
{
	/* struct othm_idea *truth = othm_idea_new(NULL, OTHM_SYMBOL(true), */
	/* 					OTHM_SYMBOL(abstract)); */

	struct othm_idea *context = othm_idea_new(NULL, OTHM_SYMBOL(context),
						  NULL, NULL);

	struct othm_idea *hood = othm_idea_new(NULL, OTHM_SYMBOL(hood),
					       OTHM_KEYWORD(worn), context);

	struct othm_idea *black = othm_idea_new(NULL, OTHM_SYMBOL(black),
						OTHM_KEYWORD(color), context);
	/* othm_idea_add(black, OTHM_KEYWORD(abstract), truth); */
	othm_idea_extensiate(black, NULL);

	struct othm_idea *scythe = othm_idea_new(NULL, OTHM_SYMBOL(scythe),
						 OTHM_KEYWORD(held), context);
	struct othm_idea *reaper = othm_idea_new(NULL, OTHM_SYMBOL(reaper),
						 OTHM_KEYWORD(body), context);

	/* struct othm_idea *personium = othm_idea_new(NULL, NULL, NULL, context); */
	/* othm_idea_new(NULL, OTHM_SYMBOL(reaper), OTHM_KEYWORD(body), personium); */


	/* othm_idea_new(NULL, OTHM_SYMBOL(restrictium), NULL, context); */

	struct othm_idea *dreadium = othm_idea_new(NULL, OTHM_SYMBOL(dreadium),
						   NULL, NULL);
	struct othm_idea *dreadium_restrict =
		othm_idea_new(NULL, NULL, OTHM_KEYWORD(restrictium), dreadium);
	othm_idea_new(NULL, NULL, OTHM_KEYWORD(color), dreadium_restrict);
	othm_idea_new(NULL, NULL, OTHM_KEYWORD(held), dreadium_restrict);
	othm_idea_new(NULL, NULL, OTHM_KEYWORD(worn), dreadium_restrict);


	othm_idea_soup(context, dreadium, NULL, NULL);
	/* othm_idea_print(Death); */
	/* othm_idea_add(hood, OTHM_KEYWORD(color), black); */
	/* othm_idea_add(Death, OTHM_KEYWORD(worn), hood); */
	/* othm_idea_add(Death, OTHM_KEYWORD(held), scythe); */
	/* othm_idea_add(Death, OTHM_KEYWORD(being), reaper); */
	othm_idea_print_recursive(context, 0);
	/* printf("Hello, world!"); */
	othm_idea_free(context, NULL);
	/* othm_idea_free(reaper, NULL); */
	/* othm_idea_free(scythe, NULL); */
	/* othm_idea_free(hood, NULL); */
	/* othm_idea_free(black, NULL); */
	/* othm_idea_free(truth, NULL); */

	return 0;
}
