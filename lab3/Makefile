NAME := lab3

$(NAME): $(NAME).c
	gcc -O0 -Wall $(NAME).c -lm -lgsl -lgslcblas -lrt -o $(NAME)

clean:
	rm $(NAME)

test: $(NAME)
	./$(NAME)
