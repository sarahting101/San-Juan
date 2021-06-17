#include <stdio.h>
#include <stdint.h>

typedef struct _sCard{
	uint8_t cost;
	char name[32];
	uint8_t role;
	/*
	1: builder phase
	2: producer phase
	3: trader phase
	4: councilor phase
	5: prospector phase
	6: during game
	7: library
	8: monument
	9: after finish
	*/
	char detail[128];
	uint8_t point;
	uint8_t piece;
}sCard;

int main(void) {
	//builder phase
	sCard smithy={
		.cost = 1,
		.name = "Smithy",
		.role = 1,
		.detail = "owner pays 1 card less when building a production building",
		.point = 1,
		.piece = 3
	};
	sCard poor={
		.cost = 2,
		.name = "Poor house",
		.role = 1,
		.detail = "owner takes 1 card from the supply if he has only 0 or 1 hand card after building",
		.point = 1,
		.piece = 3
	};
	sCard black={
		.cost = 2,
		.name = "Black market",
		.role = 1,
		.detail = "owner may discard any 1 or 2 goods and reduce the building cost by 1 or 2 cards",
		.point = 1,
		.piece = 3
	};
	sCard crane={
		.cost = 2,
		.name = "Crane",
		.role = 1,
		.detail = "owner may build over one of his buildings (and pay the difference)",
		.point = 1,
		.piece = 3
	};
	sCard carpenter={
		.cost = 3,
		.name = "Carpenter",
		.role = 1,
		.detail = "owner takes 1 card from the supply after he builds a violet building",
		.point = 2,
		.piece = 3
	};
	sCard quarry={
		.cost = 4,
		.name = "Quarry",
		.role = 1,
		.detail = "owner pays 1 card less when building a violet building",
		.point = 2,
		.piece = 3
	};
	//producer phase
	sCard indigo={
		.cost = 1,
		.name = "Indigo plant",
		.role = 2,
		.detail = "owner produces 1 sugar",
		.point = 1,
		.piece = 10
	};
	sCard sugar={
		.cost = 2,
		.name = "Sugar mill",
		.role = 2,
		.detail = "owner produces 1 sugar",
		.point = 1,
		.piece = 8
	};
	sCard tobacco={
		.cost = 3,
		.name = "Tobacco storage",
		.role = 2,
		.detail = "owner produces 1 tobacco",
		.point = 2,
		.piece = 8
	};
	sCard coffee={
		.cost = 4,
		.name = "Coffee roaster",
		.role = 2,
		.detail = "owner produces 1 coffee",
		.point = 1,
		.piece = 8
	};
	sCard silver={
		.cost = 5,
		.name = "Silver smelter",
		.role = 2,
		.detail = "owner produces 1 silver",
		.point = 3,
		.piece = 8
	};
	sCard well={
		.cost = 2,
		.name = "Well",
		.role = 2,
		.detail = "owner takes 1 card from the supply when he produces at least 2 goods",
		.point = 1,
		.piece = 3
	};
	sCard aqueduct={
		.cost = 3,
		.name = "Aqueduct",
		.role = 2,
		.detail = "owner produces 1 good more",
		.point = 2,
		.piece = 3
	};
	//trader phase
	sCard market_stand={
		.cost = 2,
		.name = "Market stand",
		.role = 3,
		.detail = "owner takes 1 card from the supply when he sells at least 2 goods",
		.point = 1,
		.piece = 3
	};
	sCard market_hall={
		.cost = 4,
		.name = "Market hall",
		.role = 3,
		.detail = "owner takes 1 card more for selling one good",
		.point = 2,
		.piece = 3
	};
	sCard trading={
		.cost = 2,
		.name = "Trading post",
		.role = 3,
		.detail = "owner may sell 1 additional good",
		.point = 1,
		.piece = 3
	};
	//councilor phase
	sCard archive={
		.cost = 1,
		.name = "Archive",
		.role = 4,
		.detail = "owner may discard hand cards in addition to drawn cards",
		.point = 1,
		.piece = 3
	};
	sCard prefecture={
		.cost = 3,
		.name = "Prefecture",
		.role = 4,
		.detail = "owner keeps 1 card more from those drawn",
		.point = 2,
		.piece = 3
	};
	//prospector phase
	sCard gold={
		.cost = 1,
		.name = "Gold mine",
		.role = 5,
		.detail = "owner turns up 4 cards from the supply, if all have different building costs, he may add one to his hand",
		.point = 1,
		.piece = 3
	};
	//during game
	sCard tower={
		.cost = 3,
		.name = "Tower",
		.role = 6,
		.detail = "owner may have up to 12 cards in his hand",
		.point = 2,
		.piece = 3
	};
	sCard chapel={
		.cost = 3,
		.name = "Chapel",
		.role = 6,
		.detail = "owner may place 1 hand card under his chapel (each worth 1 VP at game end)",
		.point = 2,
		.piece = 3
	};
	//library
	sCard library={
		.cost = 5,
		.name = "Library",
		.role = 7,
		.detail = "owner uses the privilege of his role twice",
		.point = 3,
		.piece = 3
	};
	//monument
	sCard statue={
		.cost = 3,
		.name = "Statue",
		.role = 8,
		.detail = "monument(no special function)",
		.point = 3,
		.piece = 3
	};
	sCard victory={
		.cost = 4,
		.name = "Victory column",
		.role = 8,
		.detail = "monument(no special function)",
		.point = 4,
		.piece = 3
	};
	sCard hero={
		.cost = 5,
		.name = "Hero",
		.role = 8,
		.detail = "monument(no special function)",
		.point = 5,
		.piece = 3
	};
	//after finish
	sCard guild={
		.cost = 6,
		.name = "Guild hall",
		.role = 9,
		.detail = "owner earns 2 additional victory points for each of his production buildings",
		.point = 0,
		.piece = 2
	};
	sCard city={
		.cost = 6,
		.name = "City hall",
		.role = 9,
		.detail = "owner earns 1 additional victory point for each of his violet buildings",
		.point = 0,
		.piece = 2
	};
	sCard triumhal={
		.cost = 6,
		.name = "Triumhal arch",
		.role = 9,
		.detail = "owner earns an additional 4-6-8 victory points for 1-2-3 monuments",
		.point = 0,
		.piece = 2
	};
	sCard palace={
		.cost = 6,
		.name = "Palace",
		.role = 9,
		.detail = "owner earns 1 additional victory point for every 4 victory points",
		.point = 0,
		.piece = 2
	};

	printf("You are Player1.\n");

	return 0;
}