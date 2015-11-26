all:
	gcc -g test.c othm_idea.c -o test -lothm_base -lothm_hashmap -lothm_symbols
