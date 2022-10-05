

void CreateWallWithDoor(int32 length, vec2 startingPosition, bool horizontal, vec2 wallSize, int32 doorIndex, vec2 doorSize) {
	
		for (int i = 0; i < length; i++) {
			EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
			Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
			wallEntity->handle = barrierHandle;
			
			wallEntity->position = startingPosition;
			if (horizontal) {
				if (i == doorIndex) {
					wallEntity->size = doorSize;
					wallEntity->position.x += i * doorSize.x * (doorSize.x + doorSize.x);
				}
				else {
					wallEntity->size = wallSize;
					wallEntity->position.x += i * wallSize.x * (wallSize.x + wallSize.x);
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
	
	for (int i = 0; i < length - 1; i++) {
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

void CreateRoomFloor(Room room) {
	for (int i = room.startingPosition.x; i <= (room.startingPosition.x + room.size.x); i++) {
		for (int j = room.startingPosition.y; j < (room.startingPosition.y + room.size.y); j++) {
			EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
			Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
			floorEntity->size = room.tileSize;
			floorEntity->handle = floorHandle;
			floorEntity->position = V2(i , j + (room.tileSize.y));
			floorEntity->sprite = &Data->sprites.floor1Sprite;
			//floorEntity->isQuad = true;
		}
	}
}

void CreateRoom(Room room) {
	CreateWallWithDoor(room.size.x, room.startingPosition, true, V2(room.tileSize.x, 0.1f), 2, V2(room.tileSize.x, 0.1f));

	CreateRoomFloor(room);
}


void CreateLevel(int32 level) {

	

	if (level == 1) {

		//Level level1;


		//Rooms level1.rooms;

		Room room1;

		room1.size = V2(2,2);
		room1.tileSize = V2(1,1);
		room1.startingPosition = V2(-6, -4);
		

		EntityHandle playerCarryHandle = AddEntity(&Data->em, EntityType_PlayerCarry);
		PlayerCarry* playerCarryEntity = (PlayerCarry*)GetEntity(&Data->em, playerCarryHandle);
		playerCarryEntity->handle = playerCarryHandle;
		playerCarryEntity->sprite = &Data->sprites.suitcaseSprite;
		playerCarryEntity->position = V2(2, 1);
		playerCarryEntity->size = V2(0.1f, 0.1f);
		playerCarryEntity->toDelete = false;
		playerCarryEntity->canBePickedUp = false;
		playerCarryEntity->mouseOverPlayerCarry = false;

		//CreateBackdrop(&Data->sprites.suitcaseSprite);
		//CreateBackdrop(&Data->sprites.suitcaseSprite);


		
		//CreateWall(room1.size.x, , true, V2(3.5f, 0.05f));

	//	CreateRoom(room1);

		CreateWallWithDoor(room1.size.x, room1.startingPosition, true, V2(room1.tileSize.x, 0.1f), 2, V2(room1.tileSize.x, 0.1f));

		CreateRoomFloor(room1);

		

	}
}


