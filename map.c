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

map genMap(unsigned char height, unsigned char width, unsigned int randSeed) {
	map map = {height, width, malloc(height*width*sizeof(mapCell *))};
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
	return map;
}
