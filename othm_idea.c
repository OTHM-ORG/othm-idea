#include "othm_idea.h"
#include <stdlib.h>
#include <stdio.h>

char othm_idea_key_type[] = "idea";

OTHM_KEYWORD_INIT(restrictium);

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
				struct othm_symbol_struct *symbol,
				struct othm_symbol_struct *keyword,
				struct othm_idea *context)
{
	struct othm_idea *idea;

	if (gen != NULL)
		idea = gen();
	else
		idea = malloc(sizeof(struct othm_idea));

	idea->symbol = symbol;
	idea->keyword = keyword;
	idea->context = context;

	idea->self_request.data = idea;
	idea->self_request.data_size = sizeof(struct othm_idea *);
	idea->self_request.key_type = othm_idea_key_type;
	idea->self_request.check_key = othm_symbol_pointer_compare;

	idea->fields = othm_hashmap_new(NULL);

	if (context != NULL)
		if (keyword != NULL)
			othm_hashmap_add(context->fields,
					 OTHMREQUEST(keyword),
					 idea);
		else
			othm_hashmap_add(context->fields,
					 OTHMREQUEST(idea),
					 idea);

	return idea;
}

void othm_idea_add(struct othm_idea *idea,
		   struct othm_symbol_struct *keyword,
		   struct othm_idea *sub_idea)
{
	if (sub_idea->context != NULL)
		if (sub_idea->keyword != NULL)
			othm_hashmap_remove(sub_idea->context->fields,
				    OTHMREQUEST(sub_idea->keyword));
		else
			othm_hashmap_remove(sub_idea->context->fields,
					    OTHMREQUEST(sub_idea));

	sub_idea->context = idea;
	sub_idea->keyword = keyword;

	if (keyword != NULL)
		othm_hashmap_add(idea->fields,
				 OTHMREQUEST(keyword),
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
		 othm_idea_add(to_idea, sub_idea->keyword, sub_idea));

	othm_hashmap_free(from_idea->fields, NULL);
	if (destroy != NULL)
		destroy(from_idea);
	else
		free(from_idea);
}

struct othm_idea *othm_idea_get(struct othm_idea *idea,
				struct othm_symbol_struct *keyword)
{
	struct othm_idea *sub_idea;

	sub_idea = othm_hashmap_get(idea->fields, OTHMREQUEST(keyword));
	if (sub_idea != NULL) {
		othm_hashmap_remove(idea->fields, OTHMREQUEST(keyword));
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

	idea->symbol = old_idea->symbol;
	idea->keyword = old_idea->keyword;
	idea->fields = othm_hashmap_new(NULL);
	idea->context = NULL;

	idea->self_request.data = idea;
	idea->self_request.data_size = sizeof(struct othm_idea *);
	idea->self_request.key_type = othm_idea_key_type;
	idea->self_request.check_key = othm_symbol_pointer_compare;

	OTHM_IDEA_FORALL
		(old_idea,
		 othm_idea_add(idea, sub_idea->keyword,
			       othm_idea_copy(sub_idea, gen)));
	return idea;
}

struct othm_idea *othm_idea_get_copy(struct othm_idea *idea,
				     struct othm_symbol_struct *keyword,
				     struct othm_idea *(*gen)(void))
{
	struct othm_idea *sub_idea;

	sub_idea = othm_hashmap_get(idea->fields, OTHMREQUEST(keyword));
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
	othm_idea_new(gen, idea->symbol, NULL, idea);
	idea->symbol = NULL;
}

void othm_idea_soup(struct othm_idea *context,
		    struct othm_idea *ium,
		    struct othm_idea *(*gen)(void),
		    void (*destroy)(struct othm_idea *))
{
	struct othm_idea *transfer;
	struct othm_idea *context_restrictium;

	transfer = othm_idea_get_copy(ium, OTHM_KEYWORD(restrictium), gen);
	context_restrictium = othm_idea_get(context, OTHM_KEYWORD(restrictium));
	if (context_restrictium != NULL) {
		othm_idea_merge_to(context_restrictium, transfer, destroy);
		othm_idea_add(context, OTHM_KEYWORD(restrictium),
			      context_restrictium);
	} else {
		othm_idea_add(context, OTHM_KEYWORD(restrictium), transfer);
	}
}

void othm_idea_manifest(struct othm_idea *context,
			struct othm_idea *host)
{
	struct othm_idea *restrictium;

	restrictium = othm_hashmap_get(context->fields,
				       OTHMREQUEST
				       (OTHM_KEYWORD(restrictium)));

}

/* void othm_idea_print(struct othm_idea *idea) */
/* { */
/* 	struct othm_hashbin *current_hashbin; */
/* 	unsigned int hashbin_num; */
/* 	unsigned int i; */

/* 	current_hashbin = idea->fields->hashbins; */
/* 	hashbin_num = idea->fields->hashbin_num; */
/* 	printf("[ "); */
/* 	if (idea->symbol != NULL) */
/* 		othm_symbol_print(idea->symbol); */
/* 	else */
/* 		printf("'{ ... }"); */
/* 	printf(" ] {\n"); */
/* 	for (i = 0; i != hashbin_num; ++i) { */
/* 		struct othm_hashentry *current_hashentry; */

/* 		current_hashentry = current_hashbin->first; */
/* 		while (current_hashentry != NULL) { */
/* 			struct othm_idea *sub_idea; */

/* 			printf("  "); */
/* 			sub_idea = current_hashentry->storage; */
/* 			othm_symbol_print(sub_idea->keyword); */
/* 			printf(" = "); */
/* 			if (sub_idea->symbol != NULL) */
/* 				othm_symbol_print(sub_idea->symbol); */
/* 			else */
/* 				printf("'{ ... }"); */
/* 			printf("\n"); */

/* 			current_hashentry = current_hashentry->next; */
/* 		} */
/* 		++current_hashbin; */
/* 	} */

/* 	printf("}\n"); */
/* } */

void othm_idea_print_recursive(struct othm_idea *idea, int indent)
{
	unsigned int j;
	int inside_indent = indent + 4;

	if (idea->fields->entries_num == 0) {
		if (idea->symbol != NULL)
			othm_symbol_print(idea->symbol);
		else
			printf("...");
		printf("\n");
		return;
	}
	printf("[ ");
	if (idea->symbol != NULL)
		othm_symbol_print(idea->symbol);
	else
		printf("...");
	printf(" ] {\n");

	OTHM_IDEA_FORALL
		(idea,
		 for (j = 0; j != inside_indent; ++j)
			 printf(" ");
		 if (sub_idea->keyword != NULL) {
			 othm_symbol_print(sub_idea->keyword);
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
