.PHONY: all clean

OBJ = bin/main.o bin/main_menu/checkOwnedAccs.o bin/main_menu/main_menu.o bin/main_menu/createAcc.o bin/init_menu/login.o bin/init_menu/init_menu.o bin/init_menu/register.o

all: atm

atm: $(OBJ)
	cc -Wall -g -o $@ $^

bin/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -Wall -g -c $< -o $@

clean:
	rm -rf bin atm
