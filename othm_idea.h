#ifndef OTHM_IDEA_H
#define OTHM_IDEA_H

#include <othm_hashmap.h>
#include <othm_symbols.h>

#define OTHMIDEA(IDEA) ((struct othm_idea *) (IDEA))

struct othm_idea {
	struct othm_symbol_struct *symbol;
	struct othm_hashmap *fields;
	struct othm_symbol_struct *keyword;
};

struct othm_context_control {
	int allow;
};

struct othm_context {
	void (*control_funct)(struct othm_context_control *control,
			      struct othm_idea *to_be,
			      struct othm_idea *a,
			      struct othm_idea *b);
	struct othm_list *things;
};

struct othm_context *othm_context_new(struct othm_context *(*gen)(void),
				      void (*control_funct)
				      (struct othm_context_control *control,
				       struct othm_idea *to_be,
				       struct othm_idea *a,
				       struct othm_idea *b));

struct othm_idea *othm_idea_new(struct othm_idea *(*gen)(void),
				struct othm_symbol_struct *symbol,
				struct othm_symbol_struct *keyword);

void othm_idea_print(struct othm_idea *idea);


void othm_context_add(struct othm_context *context,
		      struct othm_list *(*list_gen_arg)(void),
		      struct othm_idea *first, ...);

void othm_context_print(struct othm_context *context);

void othm_context_manifest(struct othm_context *context,
			   struct othm_idea *idea);

#endif
