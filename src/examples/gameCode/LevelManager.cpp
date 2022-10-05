

void CreateWallWithDoorHorizontal(bool top, Room room, int32 length, vec2 startingPosition, bool horizontal, vec2 wallSize, int32 doorIndex, vec2 doorSize) {
	
	// bottom
	if (!top) {
		for (int i = 0; i < room.size.x; i += room.tileSize.x) {
			EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
			Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
			wallEntity->handle = barrierHandle;
			//startingPosition.y = -room.tileSize.y;
			wallEntity->position = startingPosition;
			wallEntity->position.y -= room.tileSize.y;
			if (horizontal) {
				if (i == doorIndex) {
					wallEntity->size = doorSize;
					wallEntity->position.x = i * room.tileSize.x * 2 + room.startingPosition.x;
				}
				else {
					wallEntity->size = wallSize;
					wallEntity->position.x = i * room.tileSize.x * 2 + room.startingPosition.x;
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
		
	// top
	if (top) {
		for (int i = 0; i < room.size.x; i += room.tileSize.x) {
			EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
			Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
			wallEntity->handle = barrierHandle;
			//startingPosition.y = -room.tileSize.y;
			wallEntity->position = startingPosition;
			wallEntity->position.y += (room.size.y * 2 - room.tileSize.y);
			if (horizontal) {
				if (i == doorIndex) {
					wallEntity->size = doorSize;
					wallEntity->position.x = i * room.tileSize.x * 2 + room.startingPosition.x;
				}
				else {
					wallEntity->size = wallSize;
					wallEntity->position.x = i * room.tileSize.x * 2 + room.startingPosition.x;
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
}

void CreateWallWithDoorVertical(bool left, Room room, int32 length, vec2 startingPosition, vec2 wallSize, int32 doorIndex, vec2 doorSize) {
	if (left) {
		for (int i = 0; i < room.size.y; i += room.tileSize.y) {
			EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
			Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
			wallEntity->handle = barrierHandle;
			//startingPosition.y = -room.tileSize.y;
			wallEntity->position = room.startingPosition;
			wallEntity->position.x -= room.tileSize.x;
			if (i == doorIndex) {
				wallEntity->size = doorSize;
				wallEntity->position.y = i * room.tileSize.y * 2 + room.startingPosition.y;
				wallEntity->isDoor = true;
				wallEntity->sprite = &Data->sprites.door1Sprite;
			}
			else {
				wallEntity->size = wallSize;
				wallEntity->position.y = i * room.tileSize.y * 2 + room.startingPosition.y;
				wallEntity->isDoor = false;
				wallEntity->sprite = &Data->sprites.wall1_vertSprite;
			}
			wallEntity->toDelete = false;
		}
	}
	if (!left) {
		for (int i = 0; i < room.size.y; i += room.tileSize.y) {
			EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
			Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
			wallEntity->handle = barrierHandle;
			//startingPosition.y = -room.tileSize.y;
			wallEntity->position = room.startingPosition;
			wallEntity->position.x += room.size.x * 2 - room.tileSize.x;
			if (i == doorIndex) {
				wallEntity->size = doorSize;
				wallEntity->position.y = i * room.tileSize.y * 2 + room.startingPosition.y;
				wallEntity->isDoor = true;
				wallEntity->sprite = &Data->sprites.door1Sprite;
			}
			else {
				wallEntity->size = wallSize;
				wallEntity->position.y = i * room.tileSize.y * 2 + room.startingPosition.y;
				wallEntity->isDoor = false;
				wallEntity->sprite = &Data->sprites.wall1_vertSprite;
			}
			wallEntity->toDelete = false;
		}
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
	for (int i = 0; i < room.size.x; i += room.tileSize.x) {
		for (int j = 0; j < room.size.y; j += room.tileSize.y) {
			EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
			Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
			floorEntity->size = room.tileSize;
			floorEntity->handle = floorHandle;
			floorEntity->position = V2(i * room.tileSize.x * 2 + room.startingPosition.x, j * room.tileSize.y * 2 + room.startingPosition.y);
			floorEntity->sprite = &Data->sprites.floor1Sprite;
			floorEntity->isQuad = false;
		}
	}
}

void CreateRoom(Room room) {
	//bottom
	//CreateWallWithDoor(room.size.x, room.startingPosition, true, V2(room.tileSize.x, 0.1f), 2, V2(room.tileSize.x, 0.1f));
	// floor
	CreateRoomFloor(room);
	//CreateWallWithDoor(room.size.x, )

}

void CreateRoad(Road road) {
	for (int i = 0; i < road.size.x; i++) {
		EntityHandle baseHandle = AddEntity(&Data->em, EntityType_Base);
		Base* roadEntity = (Base*)GetEntity(&Data->em, baseHandle);
		roadEntity->handle = baseHandle;
		roadEntity->sprite = &Data->sprites.street_Asprite;
	 
		if (road.horizontal) {
			
		}
			
	}
}

void CreateLevelPortal(vec2 position) {
	EntityHandle portalHandle = AddEntity(&Data->em, EntityType_LevelPortal);
	LevelPortal* portalEntity = (LevelPortal*)GetEntity(&Data->em, portalHandle);
	portalEntity->isLevelPortal = true;
	portalEntity->position = position;
	portalEntity->sprite = &Data->sprites.stairsDown1Sprite;
	portalEntity->size = V2(0.5f, 0.5f);
	portalEntity->handle = portalHandle;
}



void CreateLevel(int32 level) {

	if (level == 0) {

		Room room1;
		room1.size = V2(10, 7);
		room1.tileSize = V2(1, 1);
		room1.startingPosition = V2(-7, -4);

		CreateRoomFloor(room1);
		Road road1;
		road1.size = V2(5, 1);
		road1.startingPosition = V2(-4, 0);
		//road1.tileSize = V2(1, 1
		road1.horizontal = true;


		CreateRoad(road1);

		CreateLevelPortal(V2(3, 0));
	}

	if (level == 1) {



		//Level level1;


		//Rooms level1.rooms;

		Room room1;

		room1.size = V2(1, 3);
		room1.tileSize = V2(1,1);
		room1.startingPosition = V2(-6, -3);

		
		

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

		CreateWallWithDoorHorizontal(false, room1, room1.size.x, room1.startingPosition, true, V2(room1.tileSize.x, 0.01f), 100, V2(room1.tileSize.x, 0.01f));

		CreateRoomFloor(room1);
		CreateWallWithDoorHorizontal(true, room1, room1.size.x, room1.startingPosition, true, V2(room1.tileSize.x, 0.1f), 99, V2(room1.tileSize.x, 0.01f));
		CreateWallWithDoorVertical(true, room1, room1.size.y, room1.startingPosition, V2(0.01f, room1.tileSize.y), 99, V2(0.01f, room1.tileSize.y));
		CreateWallWithDoorVertical(false, room1, room1.size.y, room1.startingPosition, V2(0.01f, room1.tileSize.y), 0, V2(0.01f, room1.tileSize.y));

		//CreateWallWithDoor(room1, room1.size.x, V2(room1.startingPosition.x, room1.startingPosition.y + room1.size.y + room1.tileSize.x), true, V2(room1.tileSize.x, 0.1f), 2, V2(room1.tileSize.x, 0.1f));
		

	}
}

