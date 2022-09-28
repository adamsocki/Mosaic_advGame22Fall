enum EntityType {
	EntityType_Base,
	EntityType_Player,


	EntityType_Count,
};

struct EntityHandle {
	int32 generation;
	int32 indexInInfo;
	EntityType type;
};


struct EntityInfo {
	int32 generation;
	int32 indexInBuffer;
	
	EntityType type;
};

struct EntityTypeBuffer {
	int32 count;
	int32 capacity;
	int32 sizeInBytes;

	void* entities;
};


struct EntityManager {
	EntityTypeBuffer buffers[EntityType_Count];
	EntityInfo *entities;
	int32 entityCapacity;
	int32 nextID;
};


struct EntitySprites {
	Sprite playerSprite;
	Sprite street_Asprite;
	Sprite street_Bsprite;
};

struct MyData {

	EntitySprites sprites;

	EntityManager em;

};



struct Entity {
	vec2 position;
	Sprite* sprite;
	bool toDelete;
	vec2 size;

	EntityHandle handle;
};

struct Base : Entity {
	int32 id;
};

struct Player : Entity {
	real32 speed;
};



