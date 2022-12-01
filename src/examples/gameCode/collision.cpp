

Rect playerCollisionRect(int32 expansionFactor, Player* p)
{
	Rect playerRect;


	playerRect.max = p->size * expansionFactor;
	playerRect.min = -p->size * expansionFactor;

	return playerRect;
}




void AssignCurrentRoom()
{
	// get entities
	 //      CREATE ENTITY BUFFER REFERENCES
    //          BUFFER
    EntityTypeBuffer* roomBuffer = &Data->em.buffers[EntityType_Room];
    EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
    EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];
    EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
    EntityTypeBuffer* objectBuffer = &Data->em.buffers[EntityType_Object];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
    //          ENTITIES
    Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;
    Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;
    Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;
    Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;
    Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;

	Player* p = (Player*)GetEntity(&Data->em,playerEntitiesInBuffer[0].handle);
	// what room the player is in
	Rect playerRect;
	playerRect.min = -V2(0);
	//playerRect.min.y = -p->size.y;
	playerRect.max = V2(0);
	//playerRect.max = V2(p->size.x * 2, p->size.y * 2);
	//playerRect.max.y = p->size.y;
	vec2 dir = V2(0);
	Rect roomRect;

	// set the current Room for the player
	for (int i = 0; i < roomBuffer->count; i++)
	{
		Room* roomEntity = (Room*)GetEntity(&Data->em, roomEntitiesInBuffer[i].handle);
		roomRect.max = V2(roomEntity->size.x - 1, roomEntity->size.y - 1);
		//roomRect.min = -V2(roomEntity->size.x / 2, roomEntity->size.y / 2);
		roomRect.min = V2(0);
		// // assign to player
		//vec2 posTest.x = roomEntity->position.x + (room->size)
		if (RectTest(roomRect, playerRect, roomEntity->position1, p->position1, &dir))
		{
			p->currentRoom = roomEntity->roomNumber;
			break;
		}
		else 
		{
			p->currentRoom = NULL;
		}	
	}
}

void BarrierCheck()
{

	// get entities
	 //      CREATE ENTITY BUFFER REFERENCES
    //          BUFFER
    EntityTypeBuffer* roomBuffer = &Data->em.buffers[EntityType_Room];
    EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
    EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];
    EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
    EntityTypeBuffer* objectBuffer = &Data->em.buffers[EntityType_Object];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
    //          ENTITIES
    Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;
    Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;
    Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;
    Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;
    Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;

	Player* p = (Player*)GetEntity(&Data->em, playerEntitiesInBuffer[0].handle);
	// check if player within room
	if (p->currentRoom == NULL)
	{
		p->position1 = p->previousPosition;
	}

	// check for exceptions
		// allow passage exeptions
		// extra blocking methods
	

}