default: trigger test

trigger: trigger.c
	clang -w -static $@.c -o $@

test: test.c
	clang -static $@.c -o $@

install:

clean:
	rm -f trigger test
