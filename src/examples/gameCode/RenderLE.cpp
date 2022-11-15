



void RenderPlusMinus(vec2 plusPos, vec2 minusPos, vec2 objSize, bool overPlus, bool overMinus)
{
	if (overPlus)
	{
		DrawSprite(plusPos, objSize, 0, &Data->sprites.plusSprite_mosue);
	}
	else 
	{
		DrawSprite(plusPos, objSize, 0, &Data->sprites.plusSprite);
	}
	if (overMinus)
	{
		DrawSprite(minusPos, objSize, 0, &Data->sprites.minusSprite_mouse);
	}
	else
	{
		DrawSprite(minusPos, objSize, 0, &Data->sprites.minusSprite);
	}
}



void RenderCheckBox(bool entityRoomActive, vec2 pos, vec2 size, bool overBox)
{
	if (overBox)
	{
		if (entityRoomActive)
		{
			DrawSprite(pos, size, 0, &Data->sprites.yesCheckYesMouseSprite);
		}
		else
		{
			DrawSprite(pos, size, 0, &Data->sprites.noCheckYesMouseSprite);
		}
	}
	else
	{
		if (!entityRoomActive)
		{
			DrawSprite(pos, size, 0, &Data->sprites.noCheckNoMouseSprite);
		}
		else
		{
			DrawSprite(pos, size, 0, &Data->sprites.yesCheckNoMouseSprite);
		}
	}
}


void RenderMouseWithinMapWithSprite(EntityType type, vec2 mousePosRender) {

	


}