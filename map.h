struct mapCell {
	unsigned char walls[4], cntContent;
	struct cellContent **contents;
};

struct cellContent {
	unsigned char type;
	struct mapCell *cell;
};