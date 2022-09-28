enum EntityType {
	
	EntityType_Count;
};

struct EntityHandle {
	int32 generation;
	EntityType type;
};

struct Entity {
	vec3 position;
	Sprite* sprite;
	EntityHandle handle;
};

struct EntityInfo {
	int32 generation;
	int32 indexInBuffer;
	
	EntityType type;
};


struct EntityManager {
	EntityInfo entities;
	int32 entityCapacity;
	int32 nextID;
};


struct MyData {

	EntityManager em;

};

