default: trigger test argc_zero

trigger: trigger.c
	clang -w -static $@.c -o $@

test: test.c
	clang -static $@.c -o $@

argc_zero: argc_zero.c
	clang -static $@.c -o $@

install:

clean:
	rm -f trigger argc_zero test
