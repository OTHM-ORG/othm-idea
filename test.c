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
OTHM_SYMBOL_INIT(blue);
OTHM_SYMBOL_INIT(red);

/* OTHM_KEYWORD_INIT(thing); */
/* OTHM_SYMBOL_INIT(abstract); */
/* OTHM_SYMBOL_INIT(concrete); */

/* OTHM_KEYWORD_INIT(description); */


int main(int argc, char **args)
{
	/* othm_symbol_print(OTHM_SYMBOL(restrictium)); */
	struct othm_idea *Death = othm_idea_new(NULL, NULL,
						NULL, NULL);

	struct othm_idea *context = othm_idea_new(NULL, OTHM_SYMBOL(context),
						  NULL, NULL);

	struct othm_idea *hood = othm_idea_new(NULL, OTHM_SYMBOL(hood),
					       OTHM_KEYWORD(worn), context);

	struct othm_idea *black = othm_idea_new(NULL, OTHM_SYMBOL(black),
						OTHM_KEYWORD(color), context);
	othm_idea_extensiate(black, NULL);

	struct othm_idea *scythe = othm_idea_new(NULL, OTHM_SYMBOL(scythe),
						 OTHM_KEYWORD(held), context);
	struct othm_idea *reaper = othm_idea_new(NULL, OTHM_SYMBOL(reaper),
						 OTHM_KEYWORD(body), context);

	struct othm_idea *dreadium = othm_idea_new(NULL, OTHM_SYMBOL(dreadium),
						   NULL, NULL);
	struct othm_idea *dreadium_restrict =
		othm_idea_new(NULL, NULL, OTHM_SYMBOL(restrictium), dreadium);
	othm_idea_new(NULL, NULL, OTHM_KEYWORD(color), dreadium_restrict);
	othm_idea_new(NULL, NULL, OTHM_KEYWORD(held), dreadium_restrict);
	othm_idea_new(NULL, NULL, OTHM_KEYWORD(worn), dreadium_restrict);


	othm_idea_soup(context, dreadium, NULL, NULL);
	othm_idea_print_recursive(context, 0);

	/* othm_idea_manifest(context, Death, NULL); */
	/* othm_idea_print_recursive(Death, 0); */

	othm_idea_free(context, NULL);
	othm_idea_free(dreadium, NULL);
	othm_idea_free(Death, NULL);

	/* struct othm_idea *hood = othm_idea_new(NULL, OTHM_SYMBOL(hood), */
	/* 				       NULL, NULL); */
	/* othm_idea_new(NULL, OTHM_SYMBOL(black), OTHM_KEYWORD(color), hood); */

	/* struct othm_idea *assorted_colors2 = othm_idea_new(NULL, NULL, NULL, */
	/* 						  hood); */
	/* othm_idea_new(NULL, OTHM_SYMBOL(red), OTHM_KEYWORD(color), */
	/* 	      assorted_colors2); */

	/* struct othm_idea *assorted_colors = othm_idea_new(NULL, NULL, NULL, */
	/* 						  hood); */
	/* othm_idea_new(NULL, OTHM_SYMBOL(blue), OTHM_KEYWORD(color), */
	/* 	      assorted_colors); */

	/* othm_idea_mixup(hood); */
	/* othm_idea_print_recursive(hood, 0); */
	/* othm_idea_free(hood, NULL); */

	return 0;
}
