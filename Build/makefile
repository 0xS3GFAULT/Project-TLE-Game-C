# Module List & Files
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

moduleList = Game List Turrets Robots Core Maps UI Language
cFilesWnode = $(call rwildcard, ../Source/,*.c)
hFiles = $(call rwildcard, ../Source/,*.h)
oFilesWnode = $(subst .c,.o,$(cFilesWnode))
cFiles = $(filter-out ../Source/nodeMap.c,$(cFilesWnode))
cFilesDebug = $(filter-out ../Source/main.c,$(cFiles))

oFiles = $(filter-out ../Source/nodeMap.o,$(oFilesWnode))
oFilesNode = $(filter-out ../Source/main.o,$(oFilesWnode))

flags = -lm $(shell pkg-config --cflags --libs sdl2) -lSDL2_ttf -lSDL2_mixer -lSDL2_image -Og -ljson-c

# EXECUTABLE
# cp ../Source/main.c $(moduleListPathC) $(moduleListPathH) ./
TheLastEngineer: $(oFiles)
	gcc -Wall -o TheLastEngineer $(oFiles) $(flags) -g3

debug:
	gcc -Wall -Wextra ../Source/main.c $(cFilesDebug) $(hFiles) -o TheLastEngineer $(flags) -g3

nodeMap: $(oFilesNode)
	gcc -Wall -o nodeMap $(oFilesNode) $(flags) -g3

debugNodeMap: 
	gcc -Wall -Wextra ../Source/nodeMap.c $(cFilesDebug) $(hFiles) -o nodeMap $(flags) -g3

clear:
	rm TheLastEngineer $(oFiles)

test:
	echo $(oFiles)

%o : %c
	gcc -Wall -c -o $@ $< $(flags)