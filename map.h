typedef struct {
	unsigned char walls[4], cntContent;
	struct cellContent **contents;
} mapCell;

typedef struct {
	unsigned char type;
	struct mapCell *cell;
} cellContent;

typedef struct {
	unsigned char width, height;
	struct mapCell **cells;
} map;
