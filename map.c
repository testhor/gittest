#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "map.h"

mapCell *pgenMapCell(unsigned char walls[4], unsigned char cntContent, unsigned char x, unsigned char y, cellContent **contents, map *map) {
	mapCell *cell = malloc(sizeof(mapCell));
	if (cell == NULL) {
		fprintf(stderr, "Speicherallokation fehlgeschlagen in Funktionsaufruf pgenMapCell(%p, %hu, %hu, %hu, %p, %p).", walls, cntContent, x, y, contents,map);
		return NULL;
	}
	cell->walls[0] = walls[0];
	cell->walls[1] = walls[1];
	cell->walls[2] = walls[2];
	cell->walls[3] = walls[3];
	cell->cntContent = cntContent;
	cell->x = x;
	cell->y = y;
	cell->contents = contents;
	cell->map = map;
	return cell;
}

mapCell *getNeighbour(mapCell *cell, unsigned char dir) {
	switch (dir) {
	case 0:
		if (cell->x+1 < cell->map->height) {
			return cell->map->cells[cell->x+(cell->map->height*cell->y)+1];
		}
		break;
	case 1:
		if (cell->y+1 < cell->map->width) {
			return cell->map->cells[cell->x+((cell->y+1)*cell->map->height)];
		}
		break;
	case 2:
		if (cell->x > 0) {
			return cell->map->cells[cell->x+(cell->y*cell->map->height)-1];
		}
		break;
	case 3:
		if (cell->y > 0) {
			return cell->map->cells[cell->x+((cell->y-1)*cell->map->height)];
		}
		break;
	}
	return NULL;
}

/* Erstellt einen Irrgarten auf der gegebenen Karte ähnlich dem Algorithmus von Prim. Somit ist der entstehende Irrgarten zyklenfrei. 
 * Die Prozedur geht davon aus, dass randomize(), srand() oder etwas ähnliches ausgeführt wurde.
 * Die Prozedur geht davon aus, dass alle Wände als unpassierbar markiert sind. */
void genMazePrim1cnt(map *map, unsigned int cellsSoll) {
	mapCell *cell, *neighbour;
	unsigned char height = map->height, width = map->width, w;
	unsigned int cntCells = height*width, pos, cellsHaben;
	map->cells[0]->walls[w = (rand()%2)] = 255;
	if (w) {
		map->cells[height]->walls[3]=255;
	} else {
		map->cells[1]->walls[2] = 255;
	}
	cellsHaben = 2;
	while (cellsHaben < cellsSoll) {
		pos = (rand()%cntCells);
		do {
			cell = map->cells[(++pos) % (cntCells)];
		} while (!(cell->walls[0] | cell->walls[1] | cell->walls[2] | cell->walls[3]));
		for (w = 0; w < 4; w++) {
			if ((neighbour = getNeighbour(cell, w)) && !(neighbour->walls[0] | neighbour->walls[1] | neighbour->walls[2] | neighbour->walls[3])) {
				cell->walls[w] = neighbour->walls[(w+2)%4] = 255;
				cellsHaben++;
			}
		}
	}
	return;
}

void genMazePrim2cnt(map *map, unsigned int cellsSoll) {
	mapCell *cell, *neighbour;
	unsigned char height = map->height, width = map->width, w;
	unsigned int cntCells = height*width, cellsHaben;
	map->cells[0]->walls[w = (rand()%2)] = 255;
	if (w) {
		map->cells[height]->walls[3]=255;
	} else {
		map->cells[1]->walls[2] = 255;
	}
	cellsHaben = 2;
	while (cellsHaben < cellsSoll) {
		do {
			cell = map->cells[rand()%cntCells];
		} while (!(cell->walls[0] | cell->walls[1] | cell->walls[2] | cell->walls[3]));
		for (w = 0; w < 4; w++) {
			if ((neighbour = getNeighbour(cell, w)) && !(neighbour->walls[0] | neighbour->walls[1] | neighbour->walls[2] | neighbour->walls[3])) {
				cell->walls[w] = neighbour->walls[(w+2)%4] = 255;
				cellsHaben++;
			}
		}
	}
	return;
}

void genMazePrim3cnt(map *map, unsigned int cellsSoll) {
	mapCell *cell, *neighbour;
	unsigned char height = map->height, width = map->width, w;
	unsigned int cntCells = height*width, cellsHaben = 0;
	if (cntCells <= 1) {
		return;
	}
	do {
		cell = map->cells[rand()%cntCells];
		w = rand() % 4;	
		if (neighbour = getNeighbour(cell, w)) {
			cell->walls[w] = neighbour->walls[(w+2)%4] = 255;
			cellsHaben = 2;
		}
	} while (cellsHaben == 0);
	while (cellsHaben < cellsSoll) {
		do {
			cell = map->cells[rand()%cntCells];
		} while ((cell->walls[0] | cell->walls[1] | cell->walls[2] | cell->walls[3]) == 0);
		w = rand()%4;
		for (int i = 0; i < 4; i++) {
			if ((neighbour = getNeighbour(cell, w+i)) && ((neighbour->walls[0] | neighbour->walls[1] | neighbour->walls[2] | neighbour->walls[3]) == 0)) {
				cell->walls[w+i] = neighbour->walls[(w+i+2)%4] = 255;
				cellsHaben++;
				i = 4;
			}
		}
	}
	return;
}

void genMazePrim1(map *map) {
	genMazePrim1cnt(map, map->height*map->width);
	return;
}

void genMazePrim2(map *map) {
	genMazePrim2cnt(map, map->height*map->width);
	return;
}

void genMazePrim3(map *map) {
	genMazePrim3cnt(map, map->height*map->width);
	return;
}

void genMazePrim(map *map, unsigned char version, unsigned int cellsSoll) {
	cellsSoll += cellsSoll?0:(map->height*map->width);
	switch (version) {
	case 1:
		genMazePrim1cnt(map, cellsSoll);
		break;
	case 2:
		genMazePrim1cnt(map, cellsSoll);
		break;
	case 3:
	default:
		genMazePrim3cnt(map, cellsSoll);
		break;
	}
	return;
}

void genMaze(map *map, void *algorithm, unsigned char version, unsigned int cellsSoll) {
	switch (*(unsigned int *)algorithm) {
	case 0x6D697250:
		genMazePrim(map, version, cellsSoll);
		break;
	default:
		printf("Panic!!\n");
		break;
	}
	return;
}

map truncMap(map map) {
	unsigned char xOffset, newHeight, yOffset, newWidth;
	struct map reMap = {0, 0, NULL};
	char used = 0;
	for (xOffset = 0; !used && (xOffset < map.height); xOffset++) {
		for (unsigned char y = 0; (y < map.width) && !used ; y++) {
			used |= (0 != *(unsigned int *)map.cells[xOffset+map.height*y]->walls);
		}
	}
	if (!used) {
		return reMap;
	} else {
		xOffset--;
	}
	used = 0;
	for (newHeight = map.height-1; !used; newHeight--) {
		for (unsigned char y = 0; (y < map.width) && !used ; y++) {
			used |= (0 != *(unsigned int *)map.cells[newHeight+map.height*y]->walls);
		}
	}
	newHeight += 2;
	newHeight -= xOffset;
	used = 0;
	for (yOffset = 0; !used; yOffset++) {
		for (unsigned char x = 0; (x < map.height) && !used ; x++) {
			used |= (0 != *(unsigned int *)map.cells[x+map.height*yOffset]->walls);
		}
	}
	yOffset--;
	used = 0;
	for (newWidth = map.width-1; !used; newWidth--) {
		for (unsigned char x = 0; (x < map.height) && !used ; x++) {
			used |= (0 != *(unsigned int *)map.cells[x+map.height*newWidth]->walls);
		}
	}
	newWidth += 2;
	newWidth -= yOffset;
	reMap.cells = malloc(newHeight*newWidth*sizeof(mapCell *));
	if (reMap.cells == NULL) {
		fprintf(stderr, "Speicherallokation fehlgeschlagen in Funktionsaufruf truncMap({%d, %d, %p}).", map.height, map.width, (void *)map.cells);
		return reMap;
	}
	reMap.width = newWidth;
	reMap.height = newHeight;
	for (unsigned char x = 0; x < newHeight; x++) {
		for (unsigned char y = 0; y < newWidth; y++) {
			reMap.cells[x+newHeight*y] = map.cells[x+xOffset+map.height*(y+yOffset)];
		}
	}
	return reMap;
}

/* Erstellt eine Karte der Höhe height und der Breite width und füllt sie mit einem pseudo-zufälligen Wegemuster, welches von randSeed abhängt.
 * Falls randSeed = 0 ist, so wird statt dessen time(NULL) genommen. */
map genMap(unsigned char height, unsigned char width, unsigned int randSeed) {
	map map = {height, width, malloc(height*width*sizeof(mapCell *))}, tmap;
	if (map.cells == NULL) {
		fprintf(stderr, "Speicherallokation fehlgeschlagen in Funktionsaufruf genMap(%hu, %hu, %u).", height, width, randSeed);
		map.height = map.width = 0;
		return map;
	}
	unsigned char walls[4] = {0, 0, 0, 0};
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			map.cells[x+height*y] = pgenMapCell(walls, 0, x, y, NULL, &map);
		}
	}
	srand(randSeed?randSeed:time(NULL));
	genMaze(&map, "Prim", 3, height*width/20+(rand()%(height*width/10)));
	tmap = truncMap(map);
	for (unsigned char x = 0; x < height; x++) {
		for (unsigned char y = 0; y < width; y++) {
			if ((x < tmap.height) && (y < tmap.width) && (*(unsigned int *)tmap.cells[x+tmap.height*y]->walls == 0)) {
				tmap.cells[x+tmap.height*y] = NULL;
			}
			if (*(unsigned int *)map.cells[x+map.height*y]->walls == 0) {
				free(map.cells[x+height*y]);
			}
		}
	}
	free(map.cells);
	return tmap;
}

void printMap(struct map map) {
	for (int x = 0; x < map.height; x++) {
		for (int y = 0; y < map.width; y++) {
			unsigned char w = map.cells[x+map.height*y]?((map.cells[x+map.height*y]->walls[0] & 1) | (map.cells[x+map.height*y]->walls[1] & 2) | (map.cells[x+map.height*y]->walls[2] & 4) | (map.cells[x+map.height*y]->walls[3] & 8)):0;
			switch (w) {
			case  1: {w = 0xBB; break;};
			case  2: {w = 0xBA; break;};
			case  3: {w = 0x94; break;};
			case  4: {w = 0xB9; break;};
			case  5: {w = 0x91; break;};
			case  6: {w = 0x9A; break;};
			case  7: {w = 0xA0; break;};
			case  8: {w = 0xB8; break;};
			case  9: {w = 0x97; break;};
			case 10: {w = 0x90; break;};
			case 11: {w = 0xA6; break;};
			case 12: {w = 0x9D; break;};
			case 13: {w = 0xA3; break;};
			case 14: {w = 0xA9; break;};
			case 15: {w = 0xAC; break;};
			default: w = ' ';
			}
			if (w != ' ') {
				putchar(0xE2);
				putchar(0x95);
			}
			putchar(w);
		}
		putchar('\n');
	}
	return;
}
