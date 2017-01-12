struct MapTile {
    enum Type {
	Ocean,
	Sand,
	Steppe,
        Forest,
	Mountain,
	Count
    };

    enum Resource {
	None
    };
    
    Type type;

    Resource resource;
	
    MapTile() : type(Ocean), resource(None) {}
};
