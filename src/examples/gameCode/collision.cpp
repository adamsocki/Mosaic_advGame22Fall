

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
	p->currentRoom = false;
	p->inDoorway = false;
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
			// TODO: Make something like p->canWalk = false ?
			// check if the next space is the door? See if this works
			Rect doorRect;
			for (int i = 0; i < doorBuffer->count; i++)
			{
				Door* doorEntity = (Door*)GetEntity(&Data->em, doorEntitiesInBuffer[i].handle);
				
				if (doorEntity->size.y > doorEntity->size.x)
				{	// if door horizontal ...
					doorRect.min = V2(-0.1f, -1.0f);
					doorRect.max = V2(doorEntity->size.x + 1.25f, doorEntity->size.y);
				}
				else if (doorEntity->size.y < doorEntity->size.x)
				{	// if door vertical ...
					doorRect.min = V2(-1.0f, -0.1f);
					doorRect.max = V2(doorEntity->size.x , doorEntity->size.y + 1.25f);
				}
				
				if (RectTest(doorRect, playerRect, doorEntity->position1, p->position1, &dir))
				{
					if (doorEntity->isDoorOpen)
					{
						p->inDoorway = true;
						break;
					}
				}			
			}
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
	if (p->currentRoom == NULL && !p->inDoorway)
	{
		p->position1 = p->previousPosition;
	}

	// check for exceptions
		// allow passage exeptions
		// extra blocking methods
	

}