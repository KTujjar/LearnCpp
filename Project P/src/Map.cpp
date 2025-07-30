#include "Map.h"


Map::Map()
{
    std::vector<std::unique_ptr<SDL_Texture>> textures;
    tmx::Map map;
    if(map.load("../assets/Tiles/level3.tmx"))
    {
        // const auto& tileSets = map.getTilesets();
        // assert(!tileSets.empty());
        // for (const auto& ts : tileSets)
        // {
        //         textures.emplace_back(std::make_unique<SDL_Texture>());
        //         if (!textures.back()->loadFromFile(ts.getImagePath(), renderer))
        //         {
        //             std::cerr << "Failed opening " << ts.getImagePath() << "\n";
        //         }
        // }

        // //load the layers
        // const auto& mapLayers = map.getLayers();
        // for (auto i = 0u; i < mapLayers.size(); ++i)
        // {
        //     if (mapLayers[i]->getType() == tmx::Layer::Type::Tile)
        //     {
        //         renderLayers.emplace_back(std::make_unique<MapLayer>());
        //         renderLayers.back()->create(map, i, textures); //just cos we're using C++14
        //     }
        // }
    
    }
}