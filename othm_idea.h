#ifndef OTHM_IDEA_H
#define OTHM_IDEA_H

#include <othm_hashmap.h>
#include <othm_symbols.h>

#define OTHMIDEA(IDEA) ((struct othm_idea *) (IDEA))

struct othm_idea {
	struct othm_request self_request;
	struct othm_symbol_struct *name;
	struct othm_hashmap *fields;
	struct othm_symbol_struct *relation;

	struct othm_idea *context;
};

OTHM_SYMBOL_EXPORT(restrictium);

struct othm_idea *othm_idea_new(struct othm_idea *(*gen)(void),
				struct othm_symbol_struct *symbol,
				struct othm_symbol_struct *keyword,
				struct othm_idea *context);

void othm_idea_free(struct othm_idea *idea,
		    void (*destroy)(struct othm_idea *));

void othm_idea_add(struct othm_idea *idea,
		   struct othm_symbol_struct *keyword,
		   struct othm_idea *sub_idea);

void othm_idea_merge_to(struct othm_idea *to_idea,
			struct othm_idea *from_idea,
			void (*destroy)(struct othm_idea *));

struct othm_idea *othm_idea_get(struct othm_idea *idea,
				struct othm_symbol_struct *keyword);

struct othm_idea *othm_idea_copy(struct othm_idea *old_idea,
				 struct othm_idea *(*gen)(void));

void othm_idea_extensiate(struct othm_idea *idea,
			  struct othm_idea *(*gen)(void));

void othm_idea_soup(struct othm_idea *context,
		    struct othm_idea *ium,
		    struct othm_idea *(*gen)(void),
		    void (*destroy)(struct othm_idea *));

void othm_idea_print_recursive(struct othm_idea *idea, int indent);

void othm_idea_manifest(struct othm_idea *context,
			struct othm_idea *(*gen)(void));

void othm_idea_mixup(struct othm_idea *context);


#endif
