

void ObjectLogic()
{
    //EntityTypeBuffer* objectBuffer = &Data->em.buffers[EntityType_Object];
    //Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;
    
    // for (int i = 0; i < objectBuffer->count; i++)
    // {
    //     Object* objectEntity = (Object*)GetEntity(&Data->em, objectEntitiesInBuffer[i].handle);

        // switch(objectEntity->objectType)
        // {
        //     case 1001:
        //     {
                
        //         break;
        //     }
        //     default:
        //     {
        //         break;
        //     }
        // }
   // }

}


void ObjectRender()
{
    EntityTypeBuffer* objectBuffer = &Data->em.buffers[EntityType_Object];
    Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;
    
    for (int i = 0; i < objectBuffer->count; i++)
    {
        Object* objectEntity = (Object*)GetEntity(&Data->em, objectEntitiesInBuffer[i].handle);
        if (objectEntity != NULL)
        {
            if (objectEntity->activeRoom)
            {
                DrawSpriteBottomLeft(objectEntity->position1, objectEntity->size, 0, &Data->sprites.crosshairUnlocked1Sprite);
            }
        }
    }
    // for (int i = 0; i < objectBuffer->count; i++) {
    //     Object* objectEntity = (Object*)GetEntity(&Data->em, objectEntitiesInBuffer[i].handle);
    //     if (objectEntity != NULL) {
    //         if (objectEntity->activeRoom) {
    //             DrawSpriteBottomLeft(objectEntity->position1, objectEntity->size, 0, &Data->sprites.crosshairUnlocked1Sprite);
    //         }
    //         // if (objectEntity->isPlayingSound)
    //         // {
                
    //         // }
    //         // if (objectEntity->isDisplayingText)
    //         // {
    //         //     for (int j = 0; j < objectEntity->textValuesCount; j++)
    //         //     {
    //         //         DrawTextScreenPixel(&Game->monoFont, V2(530, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), objectEntity->text, objectEntity->textPosition);    
    //         //     }
                
    //         // }
    //     }
    // }
}
