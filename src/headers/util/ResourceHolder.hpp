#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <memory>
#include <assert.h>


using namespace std;

namespace Fonts{
	enum ID{Pacifico};
}

template < typename Resource, typename Identifier >
class ResourceHolder{

public:

	void load(Identifier id, const std::string& filename){

		unique_ptr<Resource> resource(new Resource());
		//printf("default");
		if (!resource->loadFromFile(filename))
			throw std::runtime_error("TextureHolder::load - failed to load " + filename);

		auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
		assert(inserted.second);
	}

	template <typename Parameter>
	void load(Identifier id, const std::string& filename,const Parameter& secondParam){
		unique_ptr<Resource> resource(new Resource());
		//printf("param");
		if (!resource->loadFromFile(filename,secondParam))
			throw std::runtime_error("TextureHolder::load - failed to load " + filename);

		auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
		assert(inserted.second);
	}

	Resource& get(Identifier id){
		auto found = mResourceMap.find(id);
		assert(found != mResourceMap.end());

		return *found->second;
	}

	const Resource& get(Identifier id) const{
		auto found = mResourceMap.find(id);
		assert(found != mResourceMap.end());

		return *found->second;
	}

private:
	std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

#endif