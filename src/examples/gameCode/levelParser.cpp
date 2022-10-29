



bool isLetter(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');	
}
bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}



void LoadLevelParse(int32 currentLevel)
{

	DynamicArray<TokenVal> tokens = MakeDynamicArray<TokenVal>(&arena, 100);


	FileHandle file;

	if (OpenFileForRead("data/levelEditor/level1.txt", &file, &Game->frameMem))
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
				t.start = (char*)&file.data[file.offset];
				t.length = 1;
				t.type = TokenType_AmpSymb;
				goto AddToken;
			}

			while (isLetter(c)) 
			{
				t.type = TokenType_Identifier;
				t.length++;
				
				char nextChar = file.data[file.offset + 1];
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
				t.start = (char*)&file.data[file.offset];
				t.length = 1;
				t.type = TokenType_LeftParen;
			}
			
			if (c == ')')
			{
				t.start = (char*)&file.data[file.offset];
				t.length = 1;
				t.type = TokenType_RightParen;
			}

			while (isDigit(c))
			{
				if (t.start == NULL)
				{
					t.type = TokenType_Integer;
					t.start = (char*)&file.data[file.offset];
				}
				t.length++;
				char nextChar = file.data[file.offset + 1];
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
				t.start = (char*)&file.data[file.offset];
				t.type = TokenType_PoundSymb;
				t.length = 1;
			}
			
			PushBack(&tokens, t);

		Advance:

		AddToken:
			PushBack(&tokens, t);
		}

	}

	//	READ DEM TOKENS

	for (int i = 0; i < tokens.count; i++)
	{
		Print("HI");
		TokenVal t = tokens[i];
		switch (t.type)
		{

			case TokenType_Identifier:
			{
				if (strncmp(t.start, "walls", t.length) == 0)
				{

				}
				if (strncmp(t.start, "walls", t.length) == 0)
				{

				}
			}



			case TokenType_LeftParen:
			{

			}
			case TokenType_RightParen:
			{

			}
			case TokenType_AmpSymb:
			{

			}
			case TokenType_PoundSymb:
			{

			}
			case TokenType_Integer:
			{

			}
			
			case TokenType_Count:
			{

			}



		}
	}

}