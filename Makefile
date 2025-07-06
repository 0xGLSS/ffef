OBJ = bin/main.o bin/main_menu/updateAcc.o bin/main_menu/transferOwner.o bin/main_menu/removeAcc.o bin/main_menu/makeTransaction.o bin/main_menu/checkAcc.o bin/main_menu/checkOwnedAccs.o bin/main_menu/main_menu.o bin/main_menu/createAcc.o bin/init_menu/login.o bin/init_menu/init_menu.o bin/init_menu/register.o

atm: $(OBJ)
	cc -o atm $(OBJ)

bin/%.o: src/%.c
	mkdir -p $(dir $@)
	cc -c $< -o $@

clean:
	rm -rf bin atm records.tmp
