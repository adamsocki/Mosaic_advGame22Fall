

void CreateWallWithDoor(int32 length, vec2 startingPosition, bool horizontal, vec2 wallSize, int32 doorIndex, vec2 doorSize) {
	
		for (int i = 0; i < length; i++) {
			EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
			Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
			wallEntity->handle = barrierHandle;
			
			wallEntity->position = startingPosition;
			if (horizontal) {
				if (i == doorIndex) {
					wallEntity->size = doorSize;
					wallEntity->position.x += i * doorSize.x;
				}
				else {
					wallEntity->size = wallSize;
					wallEntity->position.x += i * wallSize.x;
				}
				
			}
			if (i == doorIndex) {
				wallEntity->isDoor = true;
				wallEntity->sprite = &Data->sprites.door1Sprite;
			}
			else {
				wallEntity->isDoor = false;
				wallEntity->sprite = &Data->sprites.wall1Sprite;
			}
			wallEntity->toDelete = false;
		}
}

void CreateWall(int32 length, vec2 startingPosition, bool horizontal, vec2 size) {
	
	for (int i = 0; i < length; i++) {
		EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
		Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
		wallEntity->handle = barrierHandle;
		wallEntity->size = size;
		wallEntity->position = startingPosition;
		if (horizontal) {
			wallEntity->position.x += i * wallEntity->size.x ;
		}
		wallEntity->isDoor = false;
		wallEntity->sprite = &Data->sprites.wall1Sprite;
		wallEntity->toDelete = false;
	}
}

void CreateBackdrop(Sprite *spriteName) {
	EntityHandle backgroundHandle = AddEntity(&Data->em, EntityType_Base);
	Base* backgroundEntity = (Base*)GetEntity(&Data->em, backgroundHandle);
	backgroundEntity->sprite = spriteName;
	backgroundEntity->handle = backgroundHandle;
	backgroundEntity->position = V2(0, 0);
	backgroundEntity->size = V2(20, 20);
}


void CreateLevel(int32 level) {

	if (level == 1) {

		Level level1;


		

		EntityHandle playerCarryHandle = AddEntity(&Data->em, EntityType_PlayerCarry);
		PlayerCarry* playerCarryEntity = (PlayerCarry*)GetEntity(&Data->em, playerCarryHandle);
		playerCarryEntity->handle = playerCarryHandle;
		playerCarryEntity->sprite = &Data->sprites.suitcaseSprite;
		playerCarryEntity->position = V2(2, 1);
		playerCarryEntity->size = V2(0.1f, 0.1f);
		playerCarryEntity->toDelete = false;
		playerCarryEntity->canBePickedUp = false;
		playerCarryEntity->soundPlayed = false;

		CreateBackdrop(&Data->sprites.suitcaseSprite);
		//CreateBackdrop(&Data->sprites.suitcaseSprite);
		CreateWallWithDoor(8, V2(-4, 0), true, V2(3.5f, 0.05f), 2, V2(0.5f, 0.05f));
		CreateWall(8, V2(-4, -2), true, V2(3.5f, 0.05f));


	}
}


