


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

	EntityTypeBuffer* buffer = &em->buffers[info->type];

	return ((u8*)buffer->entities + (buffer->sizeInBytes * info->indexInBuffer));
}

void DeleteEntity(EntityManager* em, EntityHandle handle) {
	freeList[freeListCount] = handle.indexInInfo;
	freeListCount++;

	EntityTypeBuffer* buffer = &em->buffers[handle.type];
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

	EntityTypeBuffer* buffer = &em->buffers[type];
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
	Data->em.entityCapacity = 1000;
	Data->em.entities = (EntityInfo*)malloc(sizeof(EntityInfo) * Data->em.entityCapacity);
	memset(Data->em.entities, 0, sizeof(EntityInfo) * Data->em.entityCapacity);
	Data->em.nextID = 0;
}

void InitializeEntityBuffers() {
	// BaseBuffer
	EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base];
	baseBuffer->capacity = 200;
	baseBuffer->sizeInBytes = sizeof(Base);
	baseBuffer->count = 0;
	baseBuffer->entities = (Base*)malloc(baseBuffer->capacity * baseBuffer->sizeInBytes);
	
	// PlayerBuffer
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	playerBuffer->capacity = 1;
	playerBuffer->sizeInBytes = sizeof(Player);
	playerBuffer->count = 0;
	playerBuffer->entities = (Player*)malloc(playerBuffer->capacity * playerBuffer->sizeInBytes);

}


void CreatePlayer() {
	EntityHandle playerHandle = AddEntity(&Data->em, EntityType_Player);
	Player* playerEntity = (Player*)GetEntity(&Data->em, playerHandle);
	playerEntity->handle = playerHandle;
	playerEntity->position = V2(1, 1);
	playerEntity->speed = 1.5f;
	playerEntity->sprite = &Data->sprites.playerSprite;
	playerEntity->size = V2(0.2f, 0.24f);
	playerEntity->toDelete = false;
}