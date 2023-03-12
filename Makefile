obj= battle.o grid.o hero_products.o living.o product.o rpg.o
IFlags= -I include
Flags=-Wall $(IFlags)
CC=g++
Programs=rpg

#Link
all: $(obj)
	$(CC) $(Flags) $(obj) -o $(Programs)

#Compile seperately each source file

battle.o: src/battle.cpp
	$(CC) $(Flags) -c src/battle.cpp

grid.o: src/grid.cpp
	$(CC) $(Flags) -c src/grid.cpp -o $@

hero_products.o: src/hero_products.cpp
	$(CC) $(Flags) -c src/hero_products.cpp -o $@

living.o: src/living.cpp
	$(CC) $(Flags) -c src/living.cpp -o $@

product.o: src/product.cpp
	$(CC) $(Flags) -c src/product.cpp -o $@

rpg.o: src/rpg.cpp
	$(CC) $(Flags) -c src/rpg.cpp -o $@

#Clean, compile, link, clean objective files and run
run: clean all clean_obj
	./$(Programs)
	
#Clean objective files and executables
clean : 
	rm -f $(obj) $(Programs)

#Clean objective files
clean_obj:
	rm -f $(obj)

