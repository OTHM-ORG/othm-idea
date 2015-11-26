#include "othm_idea.h"
#include <stdlib.h>
#include <stdio.h>

struct othm_context *othm_context_new(struct othm_context *(*gen)(void),
				      void (*control_funct)
				      (struct othm_context_control *control,
				       struct othm_idea *to_be,
				       struct othm_idea *a,
				       struct othm_idea *b))
{
	struct othm_context *context;

	if (gen != NULL)
		context = gen();
	else
		context = malloc(sizeof(struct othm_context));

	context->control_funct = control_funct;
	context->things = NULL;

	return context;
}

struct othm_idea *othm_idea_new(struct othm_idea *(*gen)(void),
				struct othm_symbol_struct *symbol,
				struct othm_symbol_struct *keyword)
{
	struct othm_idea *idea;

	if (gen != NULL)
		idea = gen();
	else
		idea = malloc(sizeof(struct othm_idea));

	idea->symbol = symbol;
	idea->keyword = keyword;

	idea->fields = othm_hashmap_new(NULL);

	return idea;
}

struct othm_idea *othm_idea_copy(struct othm_idea *old_idea,
				 struct othm_idea *(*gen)(void))
{
	struct othm_idea *idea;
	struct othm_hashbin *current_hashbin;
	unsigned int hashbin_num;
	unsigned int i;

	if (gen != NULL)
		idea = gen();
	else
		idea = malloc(sizeof(struct othm_idea));

	idea->symbol = old_idea->symbol;
	idea->keyword = old_idea->keyword;
	idea->fields = othm_hashmap_new(NULL);

	current_hashbin = old_idea->fields->hashbins;
	hashbin_num = old_idea->fields->hashbin_num;
	for (i = 0; i != hashbin_num; ++i) {
		struct othm_hashentry *current_hashentry;

		current_hashentry = current_hashbin->first;
		while (current_hashentry != NULL) {
			struct othm_idea *sub_idea;

			sub_idea = othm_idea_copy
				(current_hashentry->storage, gen);

			othm_hashmap_add(idea->fields,
					 OTHMREQUEST(sub_idea->keyword),
					 sub_idea);

			current_hashentry = current_hashentry->next;
		}
		++current_hashbin;
	}

	return idea;
}

void othm_idea_print(struct othm_idea *idea)
{
	struct othm_hashbin *current_hashbin;
	unsigned int hashbin_num;
	unsigned int i;

	current_hashbin = idea->fields->hashbins;
	hashbin_num = idea->fields->hashbin_num;
	for (i = 0; i != hashbin_num; ++i) {
		struct othm_hashentry *current_hashentry;

		current_hashentry = current_hashbin->first;
		while (current_hashentry != NULL) {
			struct othm_idea *sub_idea;

			printf("  ");
			sub_idea = current_hashentry->storage;
			othm_symbol_print(sub_idea->keyword);
			printf(" = ");
			othm_symbol_print(sub_idea->symbol);
			printf("\n");

			current_hashentry = current_hashentry->next;
		}
		++current_hashbin;
	}
}

struct othm_list *othm_context_malloc_list(void)
{
	return malloc(sizeof(struct othm_list));
}

void othm_context_add(struct othm_context *context,
		      struct othm_list *(*list_gen_arg)(void),
		      struct othm_idea *first, ...)
{
	va_list argp;
	struct othm_idea *here;
	struct othm_list *head;
	struct othm_list *tail;
	struct othm_list *(*list_gen)(void);

	if (first == NULL)
		return NULL;
	if (list_gen_arg != NULL)
		list_gen = list_gen_arg;
	else
		list_gen = othm_context_malloc_list;

	here = first;
	head = list_gen();
	tail = head;
	va_start(argp, first);
	do {
		tail->here = here;
	} while ((here = va_arg(argp, struct othm_idea *))
		 ? (tail->next = list_gen(),
		    tail = tail->next, 1)
		 : 0);

	/* This while is used to only allocate another part of the list
	   if the list is not null! it does this using the comma operator
	   and the conditional operator */

	tail->next = NULL;
	va_end(argp);

	context->things = head;
}

void othm_context_print(struct othm_context *context)
{
	struct othm_list *things;

	for (things = context->things;
	     things != NULL;
	     things = things->next) {
		othm_symbol_print(OTHMIDEA(things->here)->symbol);
		printf(" ");
	}
}

void othm_context_manifest(struct othm_context *context,
			   struct othm_idea *idea)
{
	struct othm_context_control control;
	struct othm_list *first_thing;

	control.allow = 1;

	for (first_thing = context->things;
	     first_thing != NULL;
	     first_thing = first_thing->next) {

		struct othm_list *second_thing;

		for (second_thing = context->things;
		     second_thing != NULL;
		     second_thing = second_thing->next) {

			if (first_thing->here == second_thing->here)
				continue;

			context->control_funct(&control, idea,
					       first_thing->here,
					       second_thing->here);

			if (control.allow == 0) {
				control.allow = 1;
				continue;
			}

			struct othm_idea *new_a;
			struct othm_idea *new_b;

			new_a = othm_idea_copy(first_thing->here, NULL);
			new_b = othm_idea_copy(second_thing->here, NULL);

			othm_hashmap_add(new_a->fields,
					 OTHMREQUEST(new_b->keyword),
					 new_b);

			othm_hashmap_add(idea->fields,
					 OTHMREQUEST(new_a->keyword),
					 new_a);
		}
	}
}

