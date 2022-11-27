


void CrosshairCheck()
{
    EntityTypeBuffer* objectBuffer = &Data->em.buffers[EntityType_Object]; 
    EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
    EntityTypeBuffer* eventBuffer = &Data->em.buffers[EntityType_Event];


    Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;
    Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
    Event* eventEntitiesInBuffer = (Event*)eventBuffer->entities;

    vec2 mousePos = Data->mouseCrosshair.position;

    Player playerEntity = playerEntitiesInBuffer[0];


    Rect mouseRect;
    mouseRect.max = V2(0.2f, 0.2f);
    mouseRect.min = -V2(0.2f, 0.2f);
    vec2 dir = V2(0);
    for (int i = 0; i < objectBuffer->count; i++)
    {
        Rect objectRect;
        EntityHandle objectHandle = objectEntitiesInBuffer[i].handle;
        Object* objectEntity = (Object*)GetEntity(&Data->em, objectHandle);
        objectRect.max = objectEntity->size;
        objectRect.min = -objectEntity->size;

        if (RectTest(mouseRect, objectRect, objectEntity->position1, mousePos, &dir)) {
            if (!objectEntity->mouseOverobject)
            {
                PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);

                Data->mouseCrosshair.isLocked = true;
                objectEntity->mouseOverobject = true;
            }
            vec2 positionToSet = V2(objectEntity->position1.x + objectEntity->size.x / 2, objectEntity->position1.y + objectEntity->size.y / 2);
            Data->mouseCrosshair.position = positionToSet;
        }
        else
        {
            objectEntity->mouseOverobject = false;
            objectEntity->canPickUp = false;
            Data->mouseCrosshair.isLocked = false;
        }
    }
    //      Check to Doors
    for (int i = 0; i < doorBuffer->count; i++)
    {
        bool doorCanOpen = false;
        EntityHandle doorHandle = doorEntitiesInBuffer[i].handle;
        Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
        Rect centerDoorRect;
        centerDoorRect.max = doorEntity->size;
        centerDoorRect.min = -doorEntity->size;
        if (RectTest(mouseRect, centerDoorRect, doorEntity->position1, mousePos, &dir))
        {
            if (!doorEntity->mouseIsOver)
            {
                PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);

                doorEntity->mouseIsOver = true;
                Data->mouseCrosshair.isLocked = true;
            }
            vec2 positionToSet = V2(doorEntity->position1.x + doorEntity->size.x / 2, doorEntity->position1.y + doorEntity->size.y / 2);

            Data->mouseCrosshair.position = positionToSet;
        }
        else
        {
            doorEntity->mouseIsOver = false;
            Data->mouseCrosshair.isLocked = false;
        }

        
        // when door has mouse over
        if (doorEntity->mouseIsOver)
        {// calc player distance to door
            real32 distanceToDoor = Distance(playerEntity.position1, doorEntity->position1);
            if (distanceToDoor < 5.0f) 
            {
                doorCanOpen = true;
            }

            //      DOOR OPEN LOGIC
            if (doorCanOpen && InputPressed(Mouse, Input_MouseLeft))
            {
                EntityHandle eventHandle = AddEntity(&Data->em, EntityType_Event);
                Event* eventEntity = (Event*)GetEntity(&Data->em, eventHandle);

                eventEntity->entityHandleForEvent = doorEntity->handle;
                eventEntity->eventType = EventType_OpenDoor;
                eventEntity->handle = eventHandle;




            }
        }
















        /*if (doorEntity->isDoorCenter)
        {

        }*/
        // OPEN DOOR ON CLICK && PLAYER PROXIMITY
        // SHOW HIDDEN ROOM IF DOOR OPEN ACTIVATES ROOM
        if (doorEntity->mouseIsOver)
        {/*
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                // door proximity
                Rect expandedDoorProximityRect;
                expandedDoorProximityRect.max = doorEntity->size * 2;
                expandedDoorProximityRect.min = -doorEntity->size * 2;
                Rect playerRect = playerCollisionRect(1, &playerEntitiesInBuffer[0]);
                if (RectTest(playerRect, expandedDoorProximityRect, playerEntitiesInBuffer[0].position, doorEntity->position, &dir))
                {
                    doorEntity->isDoorOpen = !doorEntity->isDoorOpen;
                    if (doorEntity->doorActivatesRoom)
                    {
                        for (int i = 0; i < baseBuffer->count; i++)
                        {
                            Base* baseEntity = (Base*)GetEntity(&Data->em, baseEntitiesInBuffer[i].handle);
                            if (baseEntity->roomNumber == doorEntity->roomDoorActivates)
                            {
                                baseEntity->activeRoom = true;
                            }
                        }
                    }
                }
            }*/
        }
    }

}