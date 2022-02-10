default: trigger test argc_zero

trigger: trigger.c
	clang -w $@.c -o $@

test: test.c
	clang $@.c -o $@

argc_zero: argc_zero.c
	clang $@.c -o $@

install:

clean:
	rm -f trigger argc_zero test
