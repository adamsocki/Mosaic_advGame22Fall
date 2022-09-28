void CreateLevel(int32 level) {

	if (level == 1) {
		// Create Street
		for (int i = 0; i < 10; i++) {
			EntityHandle baseHandle = AddEntity(&Data->em, EntityType_Base);
			Base* baseEntity = (Base*)GetEntity(&Data->em, baseHandle);
			baseEntity->handle = baseHandle;
			if (i == 1 || i == 5 || i == 9) {
				baseEntity->sprite = &Data->sprites.street_Bsprite;
			}
			else {
				baseEntity->sprite = &Data->sprites.street_Asprite;
			}
			baseEntity->position = V2((-4 + i), 1);
			baseEntity->size = V2(0.5f, 0.4f);
			baseEntity->toDelete = false;
		}
		
	}

}