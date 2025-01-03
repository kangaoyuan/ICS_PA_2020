APP=gen-expr

$(APP): gen-expr.c
	gcc -O2 -Wall -Werror -o $@ $<
	./$(APP) 1000 > input

.PHONY: clean
clean:
	-rm -f $(APP)
