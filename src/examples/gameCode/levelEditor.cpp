






void InitializeLevelEditor()
{
	Data->levelEditor.editorType = {};


}


void SaveLevel()
{
	FileHandle file;
	if (OpenFileForWrite("data/levelEditor/level1.txt", &file, &Game->frameMem, 10))
	{
		//while ()
	}

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



