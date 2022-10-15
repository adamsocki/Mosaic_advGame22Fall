

MyData *Data = {};

int32 freeList[10000];
int32 freeListCount = 0;


void* GetEntity(EntityManager* em, EntityHandle handle) {
	if (handle.indexInInfo >= em->entityCapacity) {
		return NULL;
	}

	EntityInfo* info = &Data->em.entities[handle.indexInInfo];

	if (info->generation != handle.generation) {
		return NULL;
	}
	if (info->type != handle.type) {
		return NULL;
	}

	EntityTypeBuffer* buffer = &em->buffers[info->type][em->currentLevel];

	return ((u8*)buffer->entities + (buffer->sizeInBytes * info->indexInBuffer));
}

void DeleteEntity(EntityManager* em, EntityHandle handle) {
	freeList[freeListCount] = handle.indexInInfo;
	freeListCount++;

	EntityTypeBuffer* buffer = &em->buffers[handle.type][em->currentLevel];
	EntityInfo* info = &em->entities[handle.indexInInfo];
	EntityHandle handleOfSwappedEntity;

	buffer->count--;
}

EntityHandle AddEntity(EntityManager *em, EntityType type) {
	int32 nextFreeIdInIndex = em->nextID;
	
	if (freeListCount > 0) {
		nextFreeIdInIndex = freeList[freeListCount - 1];
		freeListCount--;
	}
	else {
		em->nextID++;
	}
	
	EntityInfo* info = &em->entities[nextFreeIdInIndex];
	info->type = type;
	info->generation++;

	EntityTypeBuffer* buffer = &em->buffers[type][em->currentLevel];
	info->indexInBuffer = buffer->count;
	buffer->count++;

	EntityHandle handle = {};
	handle.generation = info->generation;
	handle.indexInInfo = nextFreeIdInIndex;
	handle.type = type;

	return handle;
}

void InitializeEntityManager() {
	// Entity Manager
	Data->em.entityCapacity = 1000000;
	Data->em.entities = (EntityInfo*)malloc(sizeof(EntityInfo) * Data->em.entityCapacity);
	memset(Data->em.entities, 0, sizeof(EntityInfo) * Data->em.entityCapacity);
	Data->em.nextID = 0;
	Data->em.currentLevel = 2;
}

void InitializeEntityBuffers() {
	for (int i = 0; i < 4; i++) {
		EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base][i];
		baseBuffer->capacity = 2000;
		baseBuffer->sizeInBytes = sizeof(Base);
		baseBuffer->count = 0;
		baseBuffer->entities = (Base*)malloc(baseBuffer->capacity * baseBuffer->sizeInBytes);
		memset(baseBuffer->entities, 0, sizeof(Base) * baseBuffer->capacity);


		// PlayerBuffer
		EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player][i];
		playerBuffer->capacity = 1;
		playerBuffer->sizeInBytes = sizeof(Player);
		playerBuffer->count = 0;
		playerBuffer->entities = (Player*)malloc(playerBuffer->capacity * playerBuffer->sizeInBytes);
		memset(playerBuffer->entities, 0, sizeof(Player) * playerBuffer->capacity);

		// LevelPortalBuffer
		EntityTypeBuffer* levelPortalBuffer = &Data->em.buffers[EntityType_LevelPortal][i];
		levelPortalBuffer->capacity = 50;
		levelPortalBuffer->sizeInBytes = sizeof(LevelPortal);
		levelPortalBuffer->count = 0;
		levelPortalBuffer->entities = (LevelPortal*)malloc(levelPortalBuffer->capacity * levelPortalBuffer->sizeInBytes);
		//memset(levelPortalBuffer->entities, 0, sizeof(LevelPortal) * levelPortalBuffer->capacity);

		// PlayerCarryBuffer
		EntityTypeBuffer* playerCarryBuffer = &Data->em.buffers[EntityType_PlayerCarry][i];
		playerCarryBuffer->capacity = 100;
		playerCarryBuffer->sizeInBytes = sizeof(PlayerCarry);
		playerCarryBuffer->count = 0;
		playerCarryBuffer->entities = (PlayerCarry*)malloc(playerCarryBuffer->capacity * playerCarryBuffer->sizeInBytes);
		//memset(playerCarryBuffer->entities, 0, sizeof(PlayerCarry) * playerCarryBuffer->capacity);


		// BarrierBuffer
		EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier][i];
		barrierBuffer->capacity = 1000;
		barrierBuffer->sizeInBytes = sizeof(Barrier);
		barrierBuffer->count = 0;
		barrierBuffer->entities = (Barrier*)malloc(barrierBuffer->capacity * barrierBuffer->sizeInBytes);
		memset(barrierBuffer->entities, 0, sizeof(Barrier) * barrierBuffer->capacity);

		// DoorBuffer
		EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door][i];
		doorBuffer->capacity = 1000;
		doorBuffer->sizeInBytes = sizeof(Door);
		doorBuffer->count = 0;
		doorBuffer->entities = (Door*)malloc(doorBuffer->capacity * doorBuffer->sizeInBytes);
		memset(doorBuffer->entities, 0, sizeof(Door) * doorBuffer->capacity);

		// RoomTriggerBuffer
		EntityTypeBuffer* roomTriggerBuffer = &Data->em.buffers[EntityType_RoomTrigger][i];
		roomTriggerBuffer->capacity = 1000;
		roomTriggerBuffer->sizeInBytes = sizeof(RoomTrigger);
		roomTriggerBuffer->count = 0;
		roomTriggerBuffer->entities = (RoomTrigger*)malloc(roomTriggerBuffer->capacity * roomTriggerBuffer->sizeInBytes);
		memset(roomTriggerBuffer->entities, 0, sizeof(RoomTrigger) * roomTriggerBuffer->capacity);

	}
	// BaseBuffer
	
}

void CreatePlayer() {
	EntityHandle playerHandle = AddEntity(&Data->em, EntityType_Player);
	Player* playerEntity = (Player*)GetEntity(&Data->em, playerHandle);
	playerEntity->handle = playerHandle;
	playerEntity->position = V2(-5.5f, -2);
	playerEntity->speed = 1.5f;
	playerEntity->sprite = &Data->sprites.playerSprite;
	playerEntity->size = V2(0.2f, 0.24f);
	playerEntity->toDelete = false;
}
