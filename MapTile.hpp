struct MapTile {
    enum Type {
	Ocean,
	Sand,
	Steppe,
	Plains,
	Mountain
    };

    enum Resource {
	None
    };
    
    Type type;

    Resource resource;
	
    MapTile() : type(Ocean), resource(None) {}
};
