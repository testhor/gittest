typedef struct {
	unsigned char walls[4], cntContent, x, y;
	struct cellContent **contents;
	struct map *map;
} mapCell;

typedef struct {
	unsigned char type;
	mapCell *cell;
} cellContent;

typedef struct {
	unsigned char height, width;
	mapCell **cells;
} map;

map genMap(unsigned char height, unsigned char width, unsigned int randSeed);

mapCell *pgenMapCell(unsigned char walls[4], unsigned char cntContent, unsigned char x, unsigned char y, cellContent **contents, map *map);
