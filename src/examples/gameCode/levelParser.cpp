



bool isLetter(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');	
}
bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}
bool isPeriod(char c)
{
	return c == '.';
}
bool isComma(char c)
{
	return c == ',';
}





vec2 IndexForLevelCanvasObjectStartingPosition(vec2 objectPosition, vec2 canvasSize, vec2 canvasBottomLeft)
{
	vec2 positionOnCanvas;

	positionOnCanvas.x = canvasBottomLeft.x + (objectPosition.x * (canvasSize.x / 128));
	positionOnCanvas.y = canvasBottomLeft.y + (objectPosition.y * (canvasSize.y / 72));

	return positionOnCanvas;
}

vec2 convertSizeToIndexSize(vec2 sizeOfObject, vec2 canvasSize)
{
	vec2 objectIndexSize;
	objectIndexSize.x = (sizeOfObject.x * (canvasSize.x / 128));
	objectIndexSize.y = (sizeOfObject.y * (canvasSize.y /  72));

	return objectIndexSize;
}


void LoadLevelParse(int32 currentLevel, LevelState* levelState)
{
	DynamicArray<TokenVal> tokens = MakeDynamicArray<TokenVal>(&arena, 100);
	DynamicArray<Room> rooms = MakeDynamicArray<Room>(&roomArena, 12);
	DynamicArray<Monster> monsters = MakeDynamicArray<Monster>(&monsterArena, 40);
	DynamicArray<Door> doors = MakeDynamicArray<Door>(&doorArena, 40);
	DynamicArray<Object> objects = MakeDynamicArray<Object>(&objectArena, 100);

	InitializeEntityBuffers(false);


	FileHandle file;


	char* path[] =
	{
	   "data/levelEditor/level0.txt",
	   "data/levelEditor/level1.txt",
	   "data/levelEditor/level2.txt",
	   "data/levelEditor/level3.txt"
	};

	if (OpenFileForRead(path[currentLevel], &file, &Game->frameMem))
	{
		TokenVal t = {};
		while (file.offset < file.size)
		{
			t = {};
			char c = ReadChar(&file);
			
			while (c == ' ' || c == '\n' || c == '\r') {
				goto Advance;
			}

			if (c == '$') 
			{
				t.start = (char*)&file.data[file.offset - 1];
				t.length = 1;
				t.type = TokenType_DollarSymb;
				goto AddToken;
			}

			while (isLetter(c)) 
			{
				if (t.start == NULL)
				{
					t.type = TokenType_Identifier;
					t.start = (char*)&file.data[file.offset - 1];
					//t.length = 1;
				}
				
				t.length++;
				
				char nextChar = file.data[file.offset];
				if (!isLetter(nextChar))
				{
					goto AddToken;
					break;
				}
				else
				{
					c = ReadChar(&file);
				}
				
			}

			if (c == '(')
			{
				t.start = (char*)&file.data[file.offset - 1];
				t.length = 1;
				t.type = TokenType_LeftParen;
			}
			
			if (c == ')')
			{
				t.start = (char*)&file.data[file.offset - 1];
				t.length = 1;
				t.type = TokenType_RightParen;
			}
			if (c == ',')
			{
				t.type = TokenType_Comma;
				t.start = (char*)&file.data[file.offset - 1];
				t.length = 1;
			}

			while (isDigit(c))
			{
				if (t.start == NULL)
				{
					t.type = TokenType_Integer;
					t.start = (char*)&file.data[file.offset - 1];
				}
				t.length++;
				char nextChar = file.data[file.offset];
				if (!isDigit(nextChar))
				{
					goto AddToken;
					break;
				}
				else
				{
					c = ReadChar(&file);
				}
			}

			if (c == '#')
			{
				t.start = (char*)&file.data[file.offset - 1];
				t.type = TokenType_PoundSymb;
				t.length = 1;
			}
			
		
		AddToken:
			PushBack(&tokens, t);

		Advance:
			int32 baba = 0;

		}
		CloseFile(&file);
	}

	//	READ DEM TOKEs
	int32 tokenIndex = 0;

	while (tokenIndex < tokens.count)
	{
		TokenVal t = tokens[tokenIndex];

		if (t.type == TokenType_DollarSymb)
		{
			tokenIndex++;
			t = tokens[tokenIndex];
			
			if (t.type == TokenType_Identifier)
			{
				if (strncmp(t.start, "room", t.length) == 0)
				{
					tokenIndex++;
					t = tokens[tokenIndex];

					Room r = {};

				    // CREATE ROOM ENTITY
					while (t.type == TokenType_PoundSymb)
					{
						tokenIndex++;
						t = tokens[tokenIndex];

						// GATHER WALL POSITION
						if (strncmp(t.start, "pos", t.length) == 0)
						{
 
							tokenIndex++;
							t = tokens[tokenIndex];

							// LOOP OVER UNTIL DONE WITH POSITIONS
							while (t.type == TokenType_LeftParen)
							{

								tokenIndex++; 
								t = tokens[tokenIndex];
								vec2 position;
								if (t.type == TokenType_Integer)
								{
									if (r.position1.x == NULL)
									{
										r.position1.x = strtoll(t.start, NULL, 10);
									}
									
									tokenIndex++;
									t = tokens[tokenIndex];

								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (r.position1.y == NULL)
									{
										r.position1.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}

						}

						if (strncmp(t.start, "size", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];

							// LOOP OVER UNTIL DONE WITH POSITIONS
							while (t.type == TokenType_LeftParen)
							{
								tokenIndex++;
								t = tokens[tokenIndex];
								vec2 position;
								if (t.type == TokenType_Integer)
								{
									if (r.size.x == NULL)
									{
										r.size.x = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (r.size.y == NULL)
									{
										r.size.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}
						}

						// GATHER ROOM NUMBER
						if (strncmp(t.start, "roomNum", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								r.roomNum = strtoll(t.start, NULL, 10);
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}

						// GATHER INITIAL VISIBILITY
						if (strncmp(t.start, "activeRoom", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								int32 visibleRoom = strtoll(t.start, NULL, 10);
								if (visibleRoom == 0)
								{
									r.activeRoom = false;
								}
								else if (visibleRoom == 1) {
									r.activeRoom = true;
								}
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}
					
					}
					
					PushBack(&rooms, r);
				}

				if (strncmp(t.start, "monster", t.length) == 0)
				{
					tokenIndex++;
					t = tokens[tokenIndex];

					Monster m = {};

					// CREATE MONSTER ENTITY
					while (t.type == TokenType_PoundSymb)
					{
						tokenIndex++;
						t = tokens[tokenIndex];
						// GATHER Monster POSITION
						if (strncmp(t.start, "pos", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];

							// LOOP OVER UNTIL DONE WITH POSITIONS
							while (t.type == TokenType_LeftParen)
							{
								tokenIndex++;
								t = tokens[tokenIndex];
								vec2 position;
								if (t.type == TokenType_Integer)
								{
									if (m.position1.x == NULL)
									{
										m.position1.x = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];

								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (m.position1.y == NULL)
									{
										m.position1.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}




						}
						if (strncmp(t.start, "size", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];

							// LOOP OVER UNTIL DONE WITH POSITIONS
							while (t.type == TokenType_LeftParen)
							{
								tokenIndex++;
								t = tokens[tokenIndex];
								vec2 position;
								if (t.type == TokenType_Integer)
								{
									if (m.size.x == NULL)
									{
										m.size.x = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (m.size.y == NULL)
									{
										m.size.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}
						}
						if (strncmp(t.start, "posEdit", t.length) == 0) 
						{
							if (strncmp(t.start, "posEdit", t.length) == 0) {
								tokenIndex++;
								t = tokens[tokenIndex];

								if (t.type == TokenType_Integer);
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}
						}
						if (strncmp(t.start, "roomNum", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								m.roomNum = strtoll(t.start, NULL, 10);
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}
						if (strncmp(t.start, "activeRoom", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								int32 visibleRoom = strtoll(t.start, NULL, 10);
								if (visibleRoom == 0)
								{
									m.activeRoom = false;
								}
								else if (visibleRoom == 1) {
									m.activeRoom = true;
								}
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}
					}

					PushBack(&monsters, m);
				}

				if (strncmp(t.start, "door", t.length) == 0)
				{
					tokenIndex++;
					t = tokens[tokenIndex];

					Door d = {};

					while (t.type == TokenType_PoundSymb)
					{
						tokenIndex++;
						t = tokens[tokenIndex];

						if (strncmp(t.start, "pos", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							// CREATE DOOR ENTITY

							while (t.type == TokenType_LeftParen)
							{
								tokenIndex++;
								t = tokens[tokenIndex];

								if (t.type == TokenType_Integer)
								{
									if (d.position1.x == NULL)
									{
										d.position1.x = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (d.position1.y == NULL)
									{
										d.position1.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}
						}
						if (strncmp(t.start, "size", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							// CREATE DOOR ENTITY
							while (t.type == TokenType_LeftParen)
							{
								tokenIndex++;
								t = tokens[tokenIndex];

								if (t.type == TokenType_Integer)
								{
									if (d.size.x == NULL)
									{
										d.size.x = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (d.size.y == NULL)
									{
										d.size.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}
						}

						if (strncmp(t.start, "activeRoom", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								int32 visibleRoom = strtoll(t.start, NULL, 10);
								if (visibleRoom == 0)
								{
									d.activeRoom = false;
								}
								else if (visibleRoom == 1) {
									d.activeRoom = true;
								}
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}

						if (strncmp(t.start, "roomFrom", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								d.roomFrom = strtoll(t.start, NULL, 10);
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}

						if (strncmp(t.start, "roomTo", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								d.roomTo = strtoll(t.start, NULL, 10);
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}

						if (strncmp(t.start, "roomType", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							if (t.type == TokenType_Integer)
							{
								d.roomType = strtoll(t.start, NULL, 10);
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}
					}
									
					PushBack(&doors, d);
				}

				if (strncmp(t.start, "object", t.length) == 0)
				{
					tokenIndex++;
					t = tokens[tokenIndex];

					Object o = {};

					while (t.type == TokenType_PoundSymb)
					{
						tokenIndex++;
						t = tokens[tokenIndex];

						if (strncmp(t.start, "pos", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							// CREATE DOOR ENTITY

							while (t.type == TokenType_LeftParen)
							{
								tokenIndex++;
								t = tokens[tokenIndex];

								if (t.type == TokenType_Integer)
								{
									if (o.position1.x == NULL)
									{
										o.position1.x = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (o.position1.y == NULL)
									{
										o.position1.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}
						}
						if (strncmp(t.start, "size", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];
							// CREATE DOOR ENTITY
							while (t.type == TokenType_LeftParen)
							{
								tokenIndex++;
								t = tokens[tokenIndex];

								if (t.type == TokenType_Integer)
								{
									if (o.size.x == NULL)
									{
										o.size.x = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Comma)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
								if (t.type == TokenType_Integer)
								{
									if (o.size.y == NULL)
									{
										o.size.y = strtoll(t.start, NULL, 10);
									}
									tokenIndex++;
									t = tokens[tokenIndex];
								}

								if (t.type == TokenType_RightParen)
								{
									tokenIndex++;
									t = tokens[tokenIndex];
								}
							}
						}
						if (strncmp(t.start, "objectType", t.length) == 0)
						{
							tokenIndex++;
							t = tokens[tokenIndex];

							if (t.type == TokenType_Integer)
							{
								o.objectType = strtoll(t.start, NULL, 10);
								tokenIndex++;
								t = tokens[tokenIndex];
							}
						}
					}

					PushBack(&objects, o);
				}

				
			}
		
		}
		else
		{
			tokenIndex++;
			t = tokens[tokenIndex];
		}
	}

	EntityTypeBuffer* roomBuffer = &Data->em.buffers[EntityType_Room];
	Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;

	EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
	Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;

	EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];
	Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;

	EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
	Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;

	for (int i = 0; i < rooms.count; i++)
	{

		EntityHandle roomHandle = AddEntity(&Data->em, EntityType_Room);
		Room* roomEntity = (Room*)GetEntity(&Data->em, roomHandle);

		roomEntity->position1 = rooms[i].position1;
		roomEntity->size = rooms[i].size;
		roomEntity->roomNumber = rooms[i].roomNumber;
		roomEntity->activeRoom = rooms[i].activeRoom;
		roomEntity->handle = roomHandle;

		//levelState[currentLevel].room[roomEntity->roomNumber].position1 = roomEntity;
		
		//	LEFT WALL
		EntityHandle barrierHandleLeft = AddEntity(&Data->em, EntityType_Barrier);
		Barrier* barrierEntityLeft = (Barrier*)GetEntity(&Data->em, barrierHandleLeft);
		barrierEntityLeft->position1 = V2(rooms[i].position1.x - 1, rooms[i].position1.y - 1);
		barrierEntityLeft->size = V2(1, rooms[i].size.y + 2);
		barrierEntityLeft->handle = barrierHandleLeft;
		//	RIGHT WALL
		EntityHandle barrierHandleRight = AddEntity(&Data->em, EntityType_Barrier);
		Barrier* barrierEntityRight = (Barrier*)GetEntity(&Data->em, barrierHandleRight);
		barrierEntityRight->position1 = V2(rooms[i].position1.x + rooms[i].size.x, rooms[i].position1.y - 1);
		barrierEntityRight->size = V2(1, rooms[i].size.y + 2);
		barrierEntityRight->handle = barrierHandleRight;
		//	TOP WALL
		EntityHandle barrierHandleTop = AddEntity(&Data->em, EntityType_Barrier);
		Barrier* barrierEntityTop = (Barrier*)GetEntity(&Data->em, barrierHandleTop);
		barrierEntityTop->position1 = V2(rooms[i].position1.x - 1, rooms[i].position1.y + rooms[i].size.y);
		barrierEntityTop->size = V2(rooms[i].size.x + 2, 1);
		barrierEntityTop->handle = barrierHandleTop;
		//	BOTTOM WALL
		EntityHandle barrierHandleBottom = AddEntity(&Data->em, EntityType_Barrier);
		Barrier* barrierEntityBottom = (Barrier*)GetEntity(&Data->em, barrierHandleBottom);
		barrierEntityBottom->position1 = V2(rooms[i].position1.x - 1, rooms[i].position1.y - 1);
		barrierEntityBottom->size = V2(rooms[i].size.x + 2, 1);
		barrierEntityBottom->handle = barrierHandleBottom;
	}

	for (int i = 0; i < monsters.count; i++)
	{
		EntityHandle monsterHandle = AddEntity(&Data->em, EntityType_Monster);
		Monster* monsterEntity = (Monster*)GetEntity(&Data->em, monsterHandle);

		monsterEntity->position1 = monsters[i].position1;
		monsterEntity->size = monsters[i].size;
		monsterEntity->handle = monsterHandle;
		strcpy(monsterEntity->typeName, "$monster\n");

	}

	for (int i = 0; i < doors.count; i++)
	{
		EntityHandle doorHandle = AddEntity(&Data->em, EntityType_Door);
		Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);

		doorEntity->position1 = doors[i].position1;
		doorEntity->size = doors[i].size;
		doorEntity->roomTo = doors[i].roomTo;
		doorEntity->roomFrom = doors[i].roomFrom;
		doorEntity->roomType = doors[i].roomType;

		doorEntity->handle = doorHandle;
	}

	for (int i = 0; i < objects.count; i++)
	{
		EntityHandle objectHandle = AddEntity(&Data->em, EntityType_Object);
		Object* objectEntity = (Object*)GetEntity(&Data->em, objectHandle);

		objectEntity->position1 = objects[i].position1;
		objectEntity->size = objects[i].size;
		objectEntity->objectType = objects[i].objectType;
		objectEntity->handle = objectHandle;
	}

	DeallocateDynamicArray(&tokens);
	DeallocateDynamicArray(&rooms);
	DeallocateDynamicArray(&monsters);
	DeallocateDynamicArray(&doors);
	DeallocateDynamicArray(&objects);


	

}




int getSize(char* s) {
	char* t; // first copy the pointer to not change the original
	int size = 0;

	for (t = s; *t != '\0'; t++) {
		size++;
	}

	return size;
}

void WriteValues(char *val, FileHandle *file)
{
	for (int i = 0; i < getSize(val); i++) {

		WriteByte(file, (u8)val[i]);
	}
}

void SaveAndWriteLevel()
{
	int32 test;
	test = 3;
	FileHandle file;


	EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
	Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;

	char* path[] =
	{
	   "data/levelEditor/level0.txt",
	   "data/levelEditor/level1.txt",
	   "data/levelEditor/level2.txt",
	   "data/levelEditor/level3.txt"
	};

	/*char concat(int x, int y) {
		char str1[20];
		char str2[20];

		sprintf(str1, "%d", x);
		sprintf(str2, "%d", y);

		strcat(str1, str2);

		return atoi(str1);
	}*/

	if (OpenFileForWrite(path[Data->currentLevel], &file, &Game->frameMem, sizeof(Entity) * 10000))
	{

		//char str[20] = "fjaefje";
		//float number;
		//number = 33;

		
		EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
		Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;

		EntityTypeBuffer* roomBuffer = &Data->em.buffers[EntityType_Room];
		Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;

		EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
		Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;

		char leftParen[2] = "(";
		char rightParen[2] = ")";
		char comma[2] = ",";
		char newLine[3] = "\n";
		char posToken[10] = "#pos\n";
		char strengthToken[12] = "#strength\n";
		char sizeToken[12] = "#size\n";
		char roomNumberToken[15] = "#roomNum\n";
		char activeRoomToken[25] = "#activeRoom\n";
		char roomFromToken[12] = "#roomFrom\n";
		char roomToToken[12] = "#roomTo\n";
		char roomTypeToken[12] = "#roomType\n";

		for (int i = 0; i < monsterBuffer->count; i++)
		{
			Monster* monsterEntity = (Monster*)GetEntity(&Data->em, monsterEntitiesInBuffer[i].handle);
			char typeName[10] = "$monster\n";
			char pos1x[5];
			char pos1y[5];
			char entityStrength[10];
			char size_x[5];
			char size_y[5];
			char roomNum[5];
			char activeRoom[5];
			
			sprintf(entityStrength, "%d", monsterEntity->strength);

			sprintf(pos1x,  "%.0f", monsterEntity->position1.x);
			sprintf(pos1y,  "%.0f", monsterEntity->position1.y);
			sprintf(size_x, "%.0f", monsterEntity->size.x);
			sprintf(size_y, "%.0f", monsterEntity->size.y);
			sprintf(roomNum, "%d", monsterEntity->roomNum);
			sprintf(activeRoom, "%d", monsterEntity->activeRoom);


			WriteValues(typeName, &file);
			WriteValues(posToken, &file);
			WriteValues(leftParen, &file);
			WriteValues(pos1x, &file);
			WriteValues(comma, &file);
			WriteValues(pos1y, &file);
			WriteValues(rightParen, &file);
			WriteValues(newLine, &file);
			WriteValues(sizeToken, &file);

			WriteValues(leftParen, &file);
			WriteValues(size_x, &file);
			WriteValues(comma, &file);
			WriteValues(size_y, &file);
			WriteValues(rightParen, &file);

			WriteValues(strengthToken, &file);
			WriteValues(entityStrength, &file);
			WriteValues(newLine, &file);

			WriteValues(roomNumberToken, &file);
			WriteValues(roomNum, &file);
			WriteValues(newLine, &file);

			WriteValues(activeRoomToken, &file);
			WriteValues(activeRoom, &file);
			WriteValues(newLine, &file);

			WriteValues(newLine, &file);
			WriteValues(newLine, &file);

		}
		
		for (int i = 0; i < roomBuffer->count; i++)
		{
			Room* roomEntity = (Room*)GetEntity(&Data->em, roomEntitiesInBuffer[i].handle);
			char typeName[10] = "$room\n";
			char pos1x[5];
			char pos1y[5];
			char roomNum[5];
			char activeRoom[5];
			char size_x[5];
			char size_y[5];

			
			sprintf(pos1x, "%.0f", roomEntity->position1.x);
			sprintf(pos1y, "%.0f", roomEntity->position1.y);
			sprintf(size_x, "%.0f", roomEntity->size.x);
			sprintf(size_y, "%.0f", roomEntity->size.y);
			sprintf(roomNum, "%d", roomEntity->roomNum);
			sprintf(activeRoom, "%d", roomEntity->activeRoom);

			WriteValues(typeName, &file);
			WriteValues(posToken, &file);
			WriteValues(leftParen, &file);
			WriteValues(pos1x, &file);
			WriteValues(comma, &file);
			WriteValues(pos1y, &file);
			WriteValues(rightParen, &file);
			WriteValues(newLine, &file);
			WriteValues(sizeToken, &file);

			WriteValues(leftParen, &file);
			WriteValues(size_x, &file);
			WriteValues(comma, &file);
			WriteValues(size_y, &file);
			WriteValues(rightParen, &file);

			WriteValues(roomNumberToken, &file);
			WriteValues(roomNum, &file);
			WriteValues(newLine, &file);

			WriteValues(activeRoomToken, &file);
			WriteValues(activeRoom, &file);
			WriteValues(newLine, &file);

			WriteValues(newLine, &file);
			WriteValues(newLine, &file);
		}

		for (int i = 0; i < doorBuffer->count; i++)
		{
			Door* doorEntity = (Door*)GetEntity(&Data->em, doorEntitiesInBuffer[i].handle);
			char typeName[10] = "$door\n";
			char pos1x[5];
			char pos1y[5];
			char roomFrom[5];
			char roomTo[5];
			char roomType[5];
			char activeRoom[5];
			char size_x[5];
			char size_y[5];


			sprintf(pos1x, "%.0f", doorEntity->position1.x);
			sprintf(pos1y, "%.0f", doorEntity->position1.y);
			sprintf(size_x, "%.0f", doorEntity->size.x);
			sprintf(size_y, "%.0f", doorEntity->size.y);
			sprintf(activeRoom, "%d", doorEntity->activeRoom);
			sprintf(roomFrom, "%d", doorEntity->roomFrom);
			sprintf(roomTo, "%d", doorEntity->roomTo);
			sprintf(roomType, "%d", doorEntity->roomType);

			WriteValues(typeName, &file);
			WriteValues(posToken, &file);
			WriteValues(leftParen, &file);
			WriteValues(pos1x, &file);
			WriteValues(comma, &file);
			WriteValues(pos1y, &file);
			WriteValues(rightParen, &file);
			WriteValues(newLine, &file);
			WriteValues(sizeToken, &file);

			WriteValues(leftParen, &file);
			WriteValues(size_x, &file);
			WriteValues(comma, &file);
			WriteValues(size_y, &file);
			WriteValues(rightParen, &file);

			WriteValues(roomFromToken, &file);
			WriteValues(roomFrom, &file);
			WriteValues(newLine, &file);

			WriteValues(roomToToken, &file);
			WriteValues(roomTo, &file);
			WriteValues(newLine, &file);

			WriteValues(roomTypeToken, &file);
			WriteValues(roomType, &file);
			WriteValues(newLine, &file);

			WriteValues(newLine, &file);
			WriteValues(newLine, &file);
		}
	}
	
	CloseFile(&file);

}