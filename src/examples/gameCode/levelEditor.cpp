






void InitializeLevelEditor()
{
	Data->levelEditor.editorType = {};
	Data->levelEditor.objectCapacity = 100;
	Data->levelEditor.count = 0;
	Data->levelEditor.levelObjects = (EditorPlacementObject*)malloc(sizeof(EditorPlacementObject) * Data->levelEditor.objectCapacity);
	memset(Data->levelEditor.levelObjects, 0, sizeof(EditorPlacementObject) * Data->levelEditor.objectCapacity);
}



void StartObjectCreation(EntityType entityType, vec2 startIndexPosition, int32 currentLevel)
{

	EditorPlacementObject object = {};

	object.entityType = entityType;
	object.indexPositionStart = startIndexPosition;
	object.currentLevel = currentLevel;


	vec2 levelPosition = ConvertIndexToPositionInEditor( startIndexPosition);


	EntityHandle entityHandle = AddEntity(&Data->em, entityType);

	switch (entityType) {
		case EntityType_Monster:
		{
			Monster* monsterEntity = (Monster*)GetEntity(&Data->em, entityHandle);
			monsterEntity->handle = entityHandle;
			monsterEntity->position1 = startIndexPosition;
			monsterEntity->size = V2(1,1);
			monsterEntity->power = 6;
			monsterEntity->size = V2(1, 1);
			//monsterEntity->posEdit = 1;
		}



	}
	 

	//Data->levelEditor.levelObjects[Data->levelEditor.count].entityType = entityType;
	//Data->levelEditor.levelObjects[Data->levelEditor.count].currentLevel = currentLevel;


	// convert startIndexPosition to mapPosition

	//Data->levelEditor.levelObjects[Data->levelEditor.count].position1 = startIndex;
}


void ObjectSizer(vec2 mouseIndex)
{

	




}




void LoadLevel()
{
	FileHandle file;
	if (OpenFileForRead("data/levelEditor/level1.txt", &file, &Game->frameMem))
	{
		while (file.offset < file.size)
		{
			char c = ReadChar(&file);
			if (c == 13)
			{
				c = ReadChar(&file);
			}
			//if(c)
		}

	}

}


void ChangeLevel(bool forward) {


	/*if (forward)
	{

	}*/





}




//

//void RenderSpritePalatteEntities(vec2 pos, bool mouseOver, Sprite* spriteOver, Sprite* spriteNotOver, EntityType type)
//{
//	 if (Data->levelEditor.editorType == EntityType_Base || overBase)
//    { 
//        DrawSprite(baseSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.floor1Sprite);
//    }
//    else
//    {
//        DrawSprite(baseSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.wall1Sprite);
//    }
//}


