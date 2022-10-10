//TODO: CREATE LEVEL TRANSITIONER
//TODO: CREATE DOOR CONTROLLER and POPUP
//TODO: Create simple pathfinder for character

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

//void CreateWall(int32 length, vec2 startingPosition, bool horizontal, vec2 size) {
//	
//	for (int i = 0; i < length - 1; i++) {
//		EntityHandle barrierHandle = AddEntity(&Data->em, EntityType_Barrier);
//		Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
//		wallEntity->handle = barrierHandle;
//		wallEntity->size = size;
//		wallEntity->position = startingPosition;
//		if (horizontal) {
//			wallEntity->position.x += i * wallEntity->size.x ;
//		}
//		wallEntity->isDoor = false;
//		wallEntity->sprite = &Data->sprites.wall1Sprite;
//		wallEntity->toDelete = false;
//	}
//}

void CreateBackdrop(Sprite *spriteName) {
	EntityHandle backgroundHandle = AddEntity(&Data->em, EntityType_Base);
	Base* backgroundEntity = (Base*)GetEntity(&Data->em, backgroundHandle);
	backgroundEntity->sprite = spriteName;
	backgroundEntity->handle = backgroundHandle;
	backgroundEntity->position = V2(0, 0);
	backgroundEntity->size = V2(20, 20);
}

//void CreateRoomFloor(Room room) {
//	for (int i = 0; i < room.size.x; i += room.tileSize.x) {
//		for (int j = 0; j < room.size.y; j += room.tileSize.y) {
//			EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
//			Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
//			floorEntity->size = room.tileSize;
//			floorEntity->handle = floorHandle;
//			floorEntity->position = V2(i * room.tileSize.x * 2 + room.startingPosition.x, j * room.tileSize.y * 2 + room.startingPosition.y);
//			floorEntity->sprite = &Data->sprites.floor1Sprite;
//			floorEntity->isQuad = false;
//		}
//	}
//}

void CreateRoom(Room room) {
	//bottom
	//CreateWallWithDoor(room.size.x, room.startingPosition, true, V2(room.tileSize.x, 0.1f), 2, V2(room.tileSize.x, 0.1f));
	// floor
	//CreateRoomFloor(room);
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

void CreateRoomFloor8th(Room room) {

	for (int i = 0; i < room.size.x; i++) {
		for (int j = 0; j < room.size.y; j++) {
			
			if (i == 0 || j == 0 || i == room.size.x - 1 || j == room.size.y - 1) {
				EntityHandle edgeHandle = AddEntity(&Data->em, EntityType_Barrier);
				Barrier* edgeEntity = (Barrier*)GetEntity(&Data->em, edgeHandle);
				edgeEntity->size = room.tileSize;
				edgeEntity->handle = edgeHandle;
				edgeEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
				edgeEntity->sprite = &Data->sprites.blankSprite;
			}
			else {
				EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
				Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
				floorEntity->size = room.tileSize;
				floorEntity->handle = floorHandle;
				floorEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
				floorEntity->sprite = &Data->sprites.floor1Sprite;
			}
		}
	}
}

void CreateRoomFloor8thWithDoor(Room room, Door *door, uint32 numOfDoors) {
	for (int i = 0; i < room.size.x; i++) {
		for (int j = 0; j < room.size.y; j++) {
			if (i == 0 || j == 0 || i == room.size.x - 1 || j == room.size.y - 1) {
				/*for (int k = 0; k < numOfDoors; k++) {
					if (door[k].startingPosition == V2(i, j)) {

					}
				}*/
				if ((door[0].startingPosition.x == i && door[0].startingPosition.y == j) ||
					(door[1].startingPosition.x == i && door[1].startingPosition.y == j) ||
					(door[2].startingPosition.x == i && door[2].startingPosition.y == j)) {

					EntityHandle doorHandle = AddEntity(&Data->em, EntityType_Door);
					Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
					doorEntity->size = room.tileSize;
					doorEntity->handle = doorHandle;
					//doorEntity->doorNumber = door.doorNumber;
					//doorEntity->isDoor = true;
					doorEntity->isDoorOpen = false;
					doorEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
					doorEntity->sprite = &Data->sprites.doorClosed1Sprite;

				}
				else {
					EntityHandle edgeHandle = AddEntity(&Data->em, EntityType_Barrier);
					Barrier* edgeEntity = (Barrier*)GetEntity(&Data->em, edgeHandle);
					//// WALL CREATION
					////EntityHandle edgeHandle = AddEntity(&Data->em, EntityType_Barrier);
					////Barrier* edgeEntity = (Barrier*)GetEntity(&Data->em, edgeHandle);
					edgeEntity->size = room.tileSize;
					edgeEntity->handle = edgeHandle;
					edgeEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
					edgeEntity->sprite = &Data->sprites.suitcaseSprite;
				}
				for (int k = 0; k < numOfDoors; k++) {
					
					if (door[k].startingPosition.x == i && door[k].startingPosition.y == j) {
					
					}
					
					//if (door[k].startingPosition.x == i && door[k].startingPosition.y == j) {
					//	// DOOR CREATION
					//	EntityHandle doorHandle = AddEntity(&Data->em, EntityType_Barrier);
					//	Barrier* doorEntity = (Barrier*)GetEntity(&Data->em, doorHandle);
					//	doorEntity->size = room.tileSize;
					//	doorEntity->handle = doorHandle;
					//	doorEntity->doorNumber = door[k].doorNumber;
					//	if (door[k].isDoorCenter) {
					//		doorEntity->isDoorCenter = true;
					//	}
					//	//if (door[k].count == 0) {
					//	//	doorEntity->handleForOpenDoorLeft = doorHandle;
					//	//	door[k].count++;
					//	//}
					//	//else if (door[k].count == 1) {
					//	//	doorEntity->isDoorCenter = true;
					//	//	door[k].count++;
					//	//}
					//	//else if (door[k].count == 2) {
					//	//	doorEntity->handleForOpenDoorRight = doorHandle;
					//	//}
					//	//if (door[k].isDoorCenter) {
					//	//	//
					//	//}
					//	//else {
					//	//	doorEntity->isDoorCenter = false;
					//	//}
					//	//doorEntity->size = V2(room.tileSize.x * door[k].doorSize.x, room.tileSize.y * door[k].doorSize.y);
					//	doorEntity->isDoor = true;
					//	doorEntity->isOpen = false;
					//	doorEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
					//	doorEntity->sprite = &Data->sprites.doorClosed1Sprite;
					//	
					//}
				}
			}
			else {
				EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
				Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
				floorEntity->size = room.tileSize;
				floorEntity->handle = floorHandle;
				floorEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
				floorEntity->sprite = &Data->sprites.floor1Sprite;
			}
		}
	}
}

//void CreateDoor(Door door) {
//
//	EntityHandle doorHandle = AddEntity(&Data->em, EntityType_Door);
//	Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
//	doorEntity->position = V2(door.startingPosition.x, door.startingPosition.y + (door.tileSize.y));
//	doorEntity->handle = doorHandle;
//	doorEntity->sprite = &Data->sprites.doorClosed1Sprite;
//	doorEntity->size = door.tileSize;
//	for (int i = 0; i < door.size.x; i++) {
//		for (int j = 0; j < door.size.y; j++) {
//			
//		}
//	}
//}



void CreateRoomV2(Room room, Door* door, int32 numOfDoors) {
	int32 test;
	int32 val;
	for (int i = 0; i < 8; i++) {

	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < room.size.y; j++) {
			
				if (i == 0 || j == 0 || i == room.size.x - 1 || j == room.size.y - 1) {
					for (int k = 0; k < numOfDoors; k++) {
						if (i == door[k].startingPosition.x) {
							break;
						}

						else {
							EntityHandle edgeHandle = AddEntity(&Data->em, EntityType_Barrier);
							Barrier* edgeEntity = (Barrier*)GetEntity(&Data->em, edgeHandle);
							edgeEntity->size = room.tileSize;
							edgeEntity->handle = edgeHandle;
							edgeEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
							edgeEntity->sprite = &Data->sprites.suitcaseSprite;

						}
					}
				
				}
				else {
					EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
					Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
					floorEntity->size = room.tileSize;
					floorEntity->handle = floorHandle;
					floorEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
					floorEntity->sprite = &Data->sprites.floor1Sprite;
				}

		}
		test = i;
	}

	
	val = test;

}

void CreateBarrierLinear(Wall wall) {
	for (int i = 0; i < wall.length; i++) {
		EntityHandle wallHandle = AddEntity(&Data->em, EntityType_Barrier);
		Barrier* wallEntity = (Barrier*)GetEntity(&Data->em, wallHandle);
		wallEntity->position = wall.startingPosition;
		wallEntity->size = wall.tileSize;
		if (wall.horizontal) {
			wallEntity->position.x += (i * (wall.tileSize.x * 2));
		}
		else {
			wallEntity->position.y += (i * (wall.tileSize.y * 2));
		}
		wallEntity->sprite = &Data->sprites.blankSprite;
		wallEntity->handle = wallHandle;
	}
}

void CreateRoomFloor(Room room) {
	for (int i = 0; i < room.size.x; i++) {
		for (int j = 0; j < room.size.y; j++) {
			EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
			Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
			floorEntity->size = room.tileSize;
			floorEntity->handle = floorHandle;
			

			floorEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
			floorEntity->sprite = &Data->sprites.floor1Sprite;
		}
	}
}

void CreateDoorLinear(Door door) {

	EntityHandle doorHandle = AddEntity(&Data->em, EntityType_Door);
	Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
	doorEntity->position = door.startingPosition;
	doorEntity->position.x += (door.length / 2);
	
	if (door.horizontal) {
		doorEntity->size.x = door.tileSize.x * door.length;
		doorEntity->size.y = door.tileSize.y;
		doorEntity->sprite = &Data->sprites.doorClosed1hSprite;
	}
	else {
		doorEntity->size.x = door.tileSize.x;
		doorEntity->size.y = door.tileSize.y * door.length;
		doorEntity->sprite = &Data->sprites.doorClosed1vSprite;
	}
	doorEntity->handle = doorHandle;

	/*for (int i = 0; i < door.length; i++) {
		EntityHandle doorHandle = AddEntity(&Data->em, EntityType_Door);
		Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
		doorEntity->position = door.startingPosition;
		doorEntity->size = door.tileSize;
		doorEntity->isDoorOpen = false;
		if (door.horizontal) {
			doorEntity->position.x += (i * (door.tileSize.x * 2));
		}
		else {
			doorEntity->position.y += (i * (door.tileSize.y * 2));
		}
		if (i == door.doorCenterSeq) {
			doorEntity->isDoorCenter = true;
		}
		else {
			doorEntity->isDoorCenter = false;
		}
		doorEntity->sprite = &Data->sprites.doorClosed1Sprite;
		doorEntity->handle = doorHandle;
	}*/
}

Wall CreateWall(vec2 pos, vec2 tileSize, int32 length, bool isHorizontal, int32 rowNum, int32 levNum) {
	Wall wall;

	wall.startingPosition = pos;
	wall.tileSize = tileSize;
	wall.length = length;
	wall.horizontal = isHorizontal;
	wall.roomNumber = rowNum;
	wall.levelNumber = levNum;

	return wall;
}

Door CreateDoor() {
	Door door;
	return door;
}

void CreateAndPlaceRoomTrigger(vec2 pos, int32 level, int32 roomToTrigger, vec2 tileSize) {

	EntityHandle roomTriggerHandle = AddEntity(&Data->em, EntityType_RoomTrigger);
	RoomTrigger* roomTriggerEntity = (RoomTrigger*)GetEntity(&Data->em, roomTriggerHandle);
	roomTriggerEntity->position = pos;
	roomTriggerEntity->level = level;
	roomTriggerEntity->roomToTrigger = roomToTrigger;
	roomTriggerEntity->size = tileSize;
	roomTriggerEntity->sprite = &Data->sprites.suitcaseSprite;


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

	if (level == 2) {

		// Create 0.1 tile size
		Room room1;
		room1.size = V2(8,8);
		room1.tileSize = V2(0.125f, 0.125f);
		room1.startingPosition = V2(-6, -3);
		room1.roomNumber = 1;
		room1.levelNumber = level;

		Door door1;
		door1.startingPosition = V2(-5.5f,-3.25f);
		//door1.sizeOfDoor = 3;
		//door1.doorPositions[0] = V2(2, 0);
		//door1.doorPositions[1] = V2(3, 0);
		//door1.horizontal = true;
		door1.count = 0;
		door1.tileSize = V2(0.125f, 0.125f);
		door1.level = level;
		door1.horizontal = true;
		door1.length = 3;
		door1.doorNumber = 1;
		door1.isDoorCenter = false;
		door1.doorCenterSeq = 1;

		vec2 tileSize = V2(0.125f, 0.125f);

		Wall wall1 = CreateWall(V2(-6, -3.25f), tileSize, 5, true, 1, level);
		Wall wall2 = CreateWall(V2(-6.25f, -3.25f), tileSize, 10, false, 1, level);
		Wall wall3 = CreateWall(V2(-6, -1), tileSize, 8, true, 1, level);
		Wall wall4 = CreateWall(V2(-4,-3.25f), tileSize, 10, false, 1, level);

		Door doors[] = { door1 };
		uint32 numberOfDoors = sizeof(doors) / sizeof(doors[0]);

		CreateRoomFloor(room1);
		CreateBarrierLinear(wall1);
		CreateBarrierLinear(wall2);
		CreateBarrierLinear(wall3);
		CreateBarrierLinear(wall4);
		CreateDoorLinear(door1);
		CreateAndPlaceRoomTrigger(V2(0,0), level, 2, tileSize);

	}
}

