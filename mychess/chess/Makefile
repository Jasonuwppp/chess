CXX = g++-5
CXXFLAGS = -g -std=c++14 -Wall -MMD -Werror=vla
EXEC = chess -lX11
OBJECTS =  main.o window.o controller.o graphicsdisplay.o piece.o pawn.o bishop.o king.o queen.o rook.o knight.o cell.o board.o textdisplay.o player.o human.o computer.o level1.o level2.o level3.o level4.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
