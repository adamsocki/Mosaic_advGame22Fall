//TODO: CREATE LEVEL TRANSITIONER
//TODO: CREATE DOOR CONTROLLER and POPUP
//TODO: Create simple pathfinder for character



void CreateBackdrop(Sprite *spriteName) {
	EntityHandle backgroundHandle = AddEntity(&Data->em, EntityType_Base);
	Base* backgroundEntity = (Base*)GetEntity(&Data->em, backgroundHandle);
	backgroundEntity->sprite = spriteName;
	backgroundEntity->handle = backgroundHandle;
	backgroundEntity->position = V2(0, 0);
	backgroundEntity->size = V2(20, 20);
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
		if (wall.showSprite) {
			wallEntity->sprite = &Data->sprites.suitcaseSprite;
		}
		else {
			wallEntity->sprite = &Data->sprites.blankSprite;
		}
		wallEntity->handle = wallHandle;
	}
}

void CreateRoomFloor(Room room) {
	for (int i = 0; i < room.size.x; i++) 
	{
		for (int j = 0; j < room.size.y; j++) 
		{
			EntityHandle floorHandle = AddEntity(&Data->em, EntityType_Base);
			Base* floorEntity = (Base*)GetEntity(&Data->em, floorHandle);
			floorEntity->size = room.tileSize;
			floorEntity->handle = floorHandle;
			floorEntity->activeRoom = room.activeRoom;
			floorEntity->roomNumber = room.roomNumber;
			floorEntity->position = V2(((room.tileSize.x * 2) * i) + room.startingPosition.x, ((room.tileSize.y * 2) * j) + room.startingPosition.y);
			floorEntity->sprite = &Data->sprites.floor1Sprite;
		}
	}
}

void CreateDoorLinear(Door door) 
{
	EntityHandle doorHandle = AddEntity(&Data->em, EntityType_Door);
	Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
	doorEntity->position = door.startingPosition;
	doorEntity->position.x += (door.length / 2);
	doorEntity->activeRoom = true;
	doorEntity->isDoorOpen = door.isDoorOpen;
	doorEntity->doorActivatesRoom = door.doorActivatesRoom;
	doorEntity->roomDoorActivates = door.roomDoorActivates;

	if (door.horizontal) 
	{
		doorEntity->size.x = door.tileSize.x * door.length;
		doorEntity->size.y = door.tileSize.y;
		doorEntity->sprite = &Data->sprites.doorClosed1hSprite;
	}
	else 
	{
		doorEntity->size.x = door.tileSize.x;
		doorEntity->size.y = door.tileSize.y * door.length;
		doorEntity->sprite = &Data->sprites.doorClosed1vSprite;
	}
	doorEntity->handle = doorHandle;
}


Wall CreateWall2(Room room, int32 length, int32 side, bool showSprite, int32 awayFromStart) {
	Wall wall;
	wall.startingPosition = room.startingPosition;
	wall.roomNumber = room.roomNumber;
	wall.activeRoom = false;
	switch (side) 
	{
		// top
		case 1: 
		{
			wall.startingPosition.x += (room.tileSize.x * 2 * awayFromStart);
			wall.startingPosition.y = room.startingPosition.y + (room.size.y * room.tileSize.y * 2);
			wall.horizontal = true;
			break;
		}
		// bottom
		case 2:
		{
			wall.startingPosition.x += (room.tileSize.x * 2 * awayFromStart);;
			wall.startingPosition.y = room.startingPosition.y - (room.tileSize.y * 2);
			wall.horizontal = true;
			break;
		}
		// left
		case 3:
		{
			wall.startingPosition.y += (room.tileSize.y * 2 * awayFromStart);;
			wall.startingPosition.x = room.startingPosition.x - (room.tileSize.x * 2);
			wall.horizontal = false;
			break;
		}
		// right
		case 4:
		{
			wall.startingPosition.y += (room.tileSize.y * 2 * awayFromStart);;
			wall.startingPosition.x = room.startingPosition.x + (room.size.x * room.tileSize.x * 2);
			wall.horizontal = false;
			break;
		}
	}
	
	wall.tileSize = room.tileSize;
	wall.length = length;
	wall.roomNumber = room.roomNumber;
	wall.levelNumber = room.levelNumber;
	if (showSprite) {
		wall.showSprite = true;
	}
	else {
		wall.showSprite = false;
	}

	return wall;

	// create wall based on room to allow user to move the room around.
}


void SetPlayerLevelStart(vec2 startingPosition) {

	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
	for (int i = 0; i < playerBuffer->count; i++) {
		Player* playerEntity = (Player*)GetEntity(&Data->em, playerEntitiesInBuffer[i].handle);
		playerEntity->position = startingPosition;
	}
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


void CreateLevel(int32 level, LevelState levelState) 
{	
	vec2 tileSize = V2(0.125f, 0.125f);
	int32 top = 1;
	int32 bottom = 2;
	int32 left = 3;
	int32 right = 4;

	if (level == 0) 
	{
		SetPlayerLevelStart(V2(-6, 0));

		Room room1;

		room1.size = V2(53, 10);
		room1.tileSize = tileSize;
		room1.startingPosition = V2(-6.5f, -1);
		room1.roomNumber = 1;
		room1.levelNumber = level;
		room1.activeRoom = true;


		CreateRoomFloor(room1);

		Door door1;

		door1.startingPosition = V2(-5.75f, -2.75f);
		door1.count = 0;
		door1.tileSize = tileSize;
		door1.level = level;
		door1.horizontal = false;
		door1.length = 3;
		door1.doorNumber = 1;
		door1.isDoorCenter = true;
		door1.doorActivatesRoom = false;
		door1.doorCenterSeq = 1;
		door1.isDoorOpen = levelState.door[0].isDoorOpen;

		// finish making walls
		Wall wall1 = CreateWall2(room1, 30, bottom, true, 0);
		Wall wall2 = CreateWall2(room1, 8, left, true, 0);
		Wall wall3 = CreateWall2(room1, 8, top, true, 0);
		Wall wall4 = CreateWall2(room1, 8, right, true, 0);
	}

	if (level == 1) {
		SetPlayerLevelStart(V2(1, 1));


	}

	if (level == 2) {
		
		SetPlayerLevelStart(V2(-5, -2));

		
		Room room1;

		room1.size = V2(8,8);
		room1.tileSize = tileSize;
		room1.startingPosition = V2(-6.25f, -2.5f);
		room1.roomNumber = 1;
		room1.levelNumber = level;
		room1.activeRoom = true;

		Door door1;

		door1.startingPosition = V2(-5.75f,-2.75f);
		door1.count = 0;
		door1.tileSize = V2(0.125f, 0.125f);
		door1.level = level;
		door1.horizontal = true;
		door1.length = 3;
		door1.doorNumber = 1;
		door1.isDoorCenter = true;
		door1.doorActivatesRoom = true;
		door1.roomDoorActivates = 2;

		door1.doorCenterSeq = 1;
		door1.isDoorOpen = levelState.door[0].isDoorOpen;

		int32 top = 1;
		int32 bottom = 2;
		int32 left = 3;
		int32 right = 4;

		Wall wall1 = CreateWall2(room1, 5, bottom, true, 0);
		Wall wall2 = CreateWall2(room1, 8, left, true, 0);
		Wall wall3 = CreateWall2(room1, 8, top, true,0);
		Wall wall4 = CreateWall2(room1, 8, right, true,0);

		Door doors[] = { door1 };
		uint32 numberOfDoors = sizeof(doors) / sizeof(doors[0]);

		CreateRoomFloor(room1);
		CreateBarrierLinear(wall1);
		CreateBarrierLinear(wall2);
		CreateBarrierLinear(wall3);
		CreateBarrierLinear(wall4);
		CreateDoorLinear(door1);
		CreateAndPlaceRoomTrigger(V2(0,0), level, 2, tileSize);

		// ROOM 2
		Room room2;
		room2.size = V2(30, 5);
		room2.tileSize = tileSize;
		room2.startingPosition = V2(-5, -4);
		room2.roomNumber = 2;

		uint32 numberOfFloors = sizeof(levelState.base) / sizeof(levelState.base[0]);
		for (int i = 0; i < numberOfFloors; i++) 
		{
			if (levelState.base[i].roomNumber == room2.roomNumber) {
				room2.activeRoom = levelState.base[i].activeRoom;
			}
		}

		room2.levelNumber = level;

		CreateRoomFloor(room2);
		wall1 = CreateWall2(room2, 28, bottom, true,0);
		wall2 = CreateWall2(room2, 5, left, true,0);
		wall3 = CreateWall2(room2, 28, top, true,3);
		wall4 = CreateWall2(room2, 5, right, true,0);
		CreateBarrierLinear(wall1);
		CreateBarrierLinear(wall2);
		CreateBarrierLinear(wall3);
		CreateBarrierLinear(wall4);
	}
}





void LevelTransition(LevelState *levelState, bool forward)
{

	// Entity Buffer Setup
		// Door
	EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
	Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;	
		// Player
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
		// Base
	EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base];
	Base* baseEntitiesInBuffer = (Base*)baseBuffer->entities;
		// Barrier
	EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];
	Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;


	// SET SAVE STATE FOR CURRENT LEVEL
	if (Data->currentLevel == 2)
	{

		// Apply Door Save & Delete
		for (int i = 0; i < doorBuffer->count; i++)
		{
			Door* doorEntity = (Door*)GetEntity(&Data->em, doorEntitiesInBuffer[i].handle);
			levelState[Data->currentLevel].door[i].isDoorOpen = doorEntity->isDoorOpen;
			DeleteEntity(&Data->em, doorEntity->handle);
		}

		// Apply Base Save & Delete
		for (int i = 0; i < baseBuffer->count; i++) {
			Base* baseEntity = (Base*)GetEntity(&Data->em, baseEntitiesInBuffer[i].handle);
			levelState[Data->currentLevel].base[i].activeRoom = baseEntity->activeRoom;
			levelState[Data->currentLevel].base[i].roomNumber = baseEntity->roomNumber;
			DeleteEntity(&Data->em, baseEntity->handle);
		}

		// Apply Room Save
		

		// Apply Barrier Delete
		for (int i = 0; i < barrierBuffer->count; i++) {
			Barrier* barrierEntity = (Barrier*)GetEntity(&Data->em, barrierEntitiesInBuffer[i].handle);
			//levelState[Data->currentLevel].barrier[i].isbarrierOpen = barrierEntity->isbarrierOpen;
			DeleteEntity(&Data->em, barrierEntity->handle);
		}

		// Apply Player Save and Delete
		for (int i = 0; i < playerBuffer->count; i++)
		{
			//Player* playerEntity = (Player*)GetEntity(&Data->em, playerEntitiesInBuffer[i].handle);
			//	levelState[Data->currentLevel].player[i].position = playerEntity->position;
			//DeleteEntity(&Data->em, playerEntity->handle);
		}

	}


	// Advance Level

	// 1. Reinitialize Buffers for things in level
	InitializeEntityBuffers(true);

	if (forward)
	{
		Data->currentLevel++;
	}
	else
	{
		Data->currentLevel--;
	}
	CreateLevel(Data->currentLevel, levelState[Data->currentLevel]);
	// Apply saved level state to level

	




}




