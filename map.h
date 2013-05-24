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
	unsigned char width, height;
	mapCell **cells;
} map;
