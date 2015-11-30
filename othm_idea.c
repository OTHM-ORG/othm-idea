#include "othm_idea.h"
#include <stdlib.h>
#include <stdio.h>

char othm_idea_key_type[] = "idea";

OTHM_SYMBOL_INIT(restrictium);

#define OTHM_SEMICOLON_INDUCER do {} while(0)

#define OTHM_IDEA_FORALL(IDEA, CODE)					\
	{								\
		struct othm_hashbin *current_hashbin;			\
		unsigned int hashbin_num;				\
		unsigned int for_all_i;					\
		struct othm_idea *forall_idea;				\
									\
		forall_idea = (IDEA);					\
		current_hashbin = forall_idea->fields->hashbins;	\
		hashbin_num = forall_idea->fields->hashbin_num;		\
		for (for_all_i = 0;					\
		     for_all_i != hashbin_num;				\
		     ++for_all_i) {					\
			struct othm_hashentry *current_hashentry;	\
			current_hashentry = current_hashbin->first;	\
			while (current_hashentry != NULL) {		\
				struct othm_idea *sub_idea;		\
				sub_idea = current_hashentry->storage;	\
				CODE;					\
				current_hashentry =			\
					current_hashentry->next;	\
			}						\
			++current_hashbin;				\
		}							\
	}

struct othm_idea *othm_idea_new(struct othm_idea *(*gen)(void),
				struct othm_symbol_struct *name,
				struct othm_symbol_struct *relation,
				struct othm_idea *context)
{
	struct othm_idea *idea;

	if (gen != NULL)
		idea = gen();
	else
		idea = malloc(sizeof(struct othm_idea));

	idea->name = name;
	idea->relation = relation;
	idea->context = context;

	idea->self_request.data = idea;
	idea->self_request.data_size = sizeof(struct othm_idea *);
	idea->self_request.key_type = othm_idea_key_type;
	idea->self_request.check_key = othm_symbol_pointer_compare;

	idea->fields = othm_hashmap_new(NULL);

	if (context != NULL)
		if (relation != NULL)
			othm_hashmap_add(context->fields,
					 OTHMREQUEST(relation),
					 idea);
		else
			othm_hashmap_add(context->fields,
					 OTHMREQUEST(idea),
					 idea);

	return idea;
}

void othm_idea_add(struct othm_idea *idea,
		   struct othm_symbol_struct *relation,
		   struct othm_idea *sub_idea)
{
	if (sub_idea->context != NULL)
		if (sub_idea->relation != NULL)
			othm_hashmap_remove(sub_idea->context->fields,
				    OTHMREQUEST(sub_idea->relation));
		else
			othm_hashmap_remove(sub_idea->context->fields,
					    OTHMREQUEST(sub_idea));

	sub_idea->context = idea;
	sub_idea->relation = relation;

	if (relation != NULL)
		othm_hashmap_add(idea->fields,
				 OTHMREQUEST(relation),
				 sub_idea);
	else
		othm_hashmap_add(idea->fields,
				 OTHMREQUEST(sub_idea),
				 sub_idea);
}

void othm_idea_merge_to(struct othm_idea *to_idea,
			struct othm_idea *from_idea,
			void (*destroy)(struct othm_idea *))
{
	OTHM_IDEA_FORALL
		(from_idea,
		 othm_idea_add(to_idea, sub_idea->relation, sub_idea));

	othm_hashmap_free(from_idea->fields, NULL);
	if (destroy != NULL)
		destroy(from_idea);
	else
		free(from_idea);
}

struct othm_idea *othm_idea_get(struct othm_idea *idea,
				struct othm_symbol_struct *relation)
{
	struct othm_idea *sub_idea;

	sub_idea = othm_hashmap_get(idea->fields, OTHMREQUEST(relation));
	if (sub_idea != NULL) {
		othm_hashmap_remove(idea->fields, OTHMREQUEST(relation));
		sub_idea->context = NULL;
	}
	return sub_idea;
}

struct othm_idea *othm_idea_copy(struct othm_idea *old_idea,
				 struct othm_idea *(*gen)(void))
{
	struct othm_idea *idea;

	if (gen != NULL)
		idea = gen();
	else
		idea = malloc(sizeof(struct othm_idea));

	idea->name = old_idea->name;
	idea->relation = old_idea->relation;
	idea->fields = othm_hashmap_new(NULL);
	idea->context = NULL;

	idea->self_request.data = idea;
	idea->self_request.data_size = sizeof(struct othm_idea *);
	idea->self_request.key_type = othm_idea_key_type;
	idea->self_request.check_key = othm_symbol_pointer_compare;

	OTHM_IDEA_FORALL
		(old_idea,
		 othm_idea_add(idea, sub_idea->relation,
			       othm_idea_copy(sub_idea, gen)));
	return idea;
}

struct othm_idea *othm_idea_get_copy(struct othm_idea *idea,
				     struct othm_symbol_struct *relation,
				     struct othm_idea *(*gen)(void))
{
	struct othm_idea *sub_idea;

	sub_idea = othm_hashmap_get(idea->fields, OTHMREQUEST(relation));
	if (sub_idea != NULL) {
		sub_idea = othm_idea_copy(sub_idea, gen);
		sub_idea->context = NULL;
	}
	return sub_idea;
}

void othm_idea_destroy(struct othm_idea *idea)
{
	free(idea);
}

void othm_idea_free(struct othm_idea *idea,
		    void (*destroy)(struct othm_idea *))
{
	OTHM_IDEA_FORALL
		(idea,
		 othm_idea_free(sub_idea, destroy));
	othm_hashmap_free(idea->fields, NULL);
	if (destroy != NULL)
		destroy(idea);
	else
		free(idea);
}

void othm_idea_extensiate(struct othm_idea *idea,
			  struct othm_idea *(*gen)(void))
{
	othm_idea_new(gen, idea->name, NULL, idea);
	idea->name = NULL;
}

void othm_idea_soup(struct othm_idea *context,
		    struct othm_idea *ium,
		    struct othm_idea *(*gen)(void),
		    void (*destroy)(struct othm_idea *))
{
	struct othm_idea *transfer;
	struct othm_idea *context_restrictium;

	transfer = othm_idea_get_copy(ium, OTHM_SYMBOL(restrictium), gen);
	if (transfer == NULL)
		return;
	context_restrictium = othm_idea_get(context, OTHM_SYMBOL(restrictium));
	if (context_restrictium != NULL) {
		othm_idea_merge_to(context_restrictium, transfer, destroy);
		othm_idea_add(context, OTHM_SYMBOL(restrictium),
			      context_restrictium);
	} else {
		othm_idea_add(context, OTHM_SYMBOL(restrictium), transfer);
	}
}

/* void othm_idea_manifest(struct othm_idea *context, */
/* 			struct othm_idea *(*gen)(void)) */
/* { */
/* 	struct othm_idea *restrictium; */

/* 	restrictium = othm_hashmap_get(context->fields, */
/* 				       OTHMREQUEST */
/* 				       (OTHM_SYMBOL(restrictium))); */
/* 	OTHM_IDEA_FORALL */
/* 		(restrictium, */
/* 		 othm_idea_add(host, sub_idea->relation, */
/* 			       othm_idea_get_copy(context, */
/* 						  sub_idea->relation, */
/* 						  gen))); */

/* } */

void othm_idea_mixup(struct othm_idea *context)
{
	OTHM_IDEA_FORALL
		(context,
		 struct othm_idea *top_idea;

		 top_idea = sub_idea;
		 if (top_idea->relation == NULL) {
			 OTHM_IDEA_FORALL
				 (top_idea,
				  if (sub_idea->relation == NULL)
					  goto next;
				  struct othm_idea *top_get;
				  struct othm_idea *sub_get;

				  top_get =
				  othm_idea_get(context,
						sub_idea->relation);
				  if (top_get == NULL)
					  goto next;
				  /* current_hashentry =			\ */
				  /* 	current_hashentry->next;	\ */
				  /* othm_symbol_print(top_get->relation); */
				  /* othm_symbol_print(top_get->name); */
				  sub_get =
				  othm_idea_get(top_idea,
				  		sub_idea->relation);

				  othm_idea_add(top_idea,
				  		sub_idea->relation,
				  		top_get);
				  othm_idea_add(context,
				  		sub_idea->relation,
				  		sub_get);
				 next:
				  OTHM_SEMICOLON_INDUCER)
				 }
		 OTHM_SEMICOLON_INDUCER);
}


void othm_idea_print_recursive(struct othm_idea *idea, int indent)
{
	unsigned int j;
	int inside_indent = indent + 4;

	if (idea->fields->entries_num == 0) {
		if (idea->name != NULL)
			othm_symbol_print(idea->name);
		else
			printf("...");
		printf("\n");
		return;
	}
	printf("[ ");
	if (idea->name != NULL)
		othm_symbol_print(idea->name);
	else
		printf("...");
	printf(" ] {\n");

	OTHM_IDEA_FORALL
		(idea,
		 for (j = 0; j != inside_indent; ++j)
			 printf(" ");
		 if (sub_idea->relation != NULL) {
			 othm_symbol_print(sub_idea->relation);
			 printf(" = ");
		 } else {
			 printf("-> ");
		 }
		 othm_idea_print_recursive(sub_idea,
					   indent + 4));

	for (j = 0; j != indent; ++j)
		printf(" ");
	printf("}\n");
}
