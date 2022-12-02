

void RevealRoom(int32 roomToReveal)
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

	for (int i = 0; i < monsterBuffer->count; i++)
	{
		Monster* monsterEntity = (Monster*)GetEntity(&Data->em, monsterEntitiesInBuffer[i].handle);
		if (monsterEntity->roomNumber == roomToReveal)
		{
			monsterEntity->activeRoom = true;
		}
	}
	for (int i = 0; i < doorBuffer->count; i++)
	{
		Door* doorEntity = (Door*)GetEntity(&Data->em, doorEntitiesInBuffer[i].handle);
		if (doorEntity->roomNumber == roomToReveal)
		{
			doorEntity->activeRoom = true;
		}
	}
	for (int i = 0; i < objectBuffer->count; i++)
	{
		Object* objectEntity = (Object*)GetEntity(&Data->em, objectEntitiesInBuffer[i].handle);
		if (objectEntity->roomNumber == roomToReveal)
		{
			objectEntity->activeRoom = true;
		}
	}
}

void ExecuteEvents()
{
	EntityTypeBuffer* eventBuffer = &Data->em.buffers[EntityType_Event];
	Event* eventEntitiesInBuffer = (Event*)eventBuffer->entities;

	for (int i = 0; i < eventBuffer->count; i++)
	{
		Event* eventEntity = (Event*)GetEntity(&Data->em, eventEntitiesInBuffer[i].handle);

		switch (eventEntity->eventType)
		{
			case EventType_OpenDoor:
			{//		OPEN DOOR LOGIC
				Door* doorEntity = (Door*)GetEntity(&Data->em, eventEntity->entityHandleForEvent);
				doorEntity->isDoorOpen = true;
				eventEntity->toDelete = true;


				Room* roomEntity = (Room*)GetEntity(&Data->em, doorEntity->doorToHandle);
				roomEntity->activeRoom = true;
				RevealRoom(roomEntity->roomNumber);
				
				eventEntity->toDelete = true;

				break;
			}
			case EventType_LevelChange:
			{
				
				// find previous logic to save level
				
				// start level chnage frame buffer animation
				
				// load next level
				
				
			}
			case EventType_ObjectTrigger:
			{
				
				switch (eventEntity->objectType)
				{
					case 1001:
					{
						
						break;
					}
					default:
					{
						break;
					}
				}



			}
			default:
			{
				break;
			}
		}
	}
}