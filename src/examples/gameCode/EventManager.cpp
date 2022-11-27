

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

				if (doorEntity->doorActivatesRoom)
				{
					Room* roomEntity = (Room*)GetEntity(&Data->em, doorEntity->doorToHandle);
					roomEntity->activeRoom = true;
				}

				break;
			}
			default:
			{
				break;
			}
		}
	}
}