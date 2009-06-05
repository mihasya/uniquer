all: mihabox

mihabox:
	gcc -o uniquer uniquer_lib.c uniquer.c -lm
test_lib:
	gcc -o test uniquer_lib.c uniquer_lib_test.c