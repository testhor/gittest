#ifndef MAP_H
#define MAP_H

typedef struct mapCell {
	unsigned char walls[4], cntContent, x, y;
	struct cellContent **contents;
	struct map *map;
} mapCell;

typedef struct cellContent {
	unsigned char type;
	mapCell *cell;
} cellContent;

typedef struct map {
	unsigned char height, width;
	mapCell **cells;
} map;

map genMap(unsigned char height, unsigned char width, unsigned int randSeed);

#endif
