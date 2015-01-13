#include "stdafx.h"
#include "SphericalVoxelComponent.h"

#include <Vorb/io/IOManager.h>

#include "SphericalTerrainPatch.h"
#include "ChunkManager.h"
#include "ParticleEngine.h"
#include "PhysicsEngine.h"
#include "SphericalTerrainGenerator.h"
#include "VoxelPlanetMapper.h"


void SphericalVoxelComponent::init(const SphericalTerrainData* sphericalTerrainData, const vio::IOManager* saveFileIom,
                                   SphericalTerrainGenerator* terrainGenerator,
                                   const glm::dvec3 &gpos, vvox::VoxelMapData* startingMapData) {
    this->sphericalTerrainData = sphericalTerrainData;
    this->saveFileIom = saveFileIom;

    // Allocate resources
    physicsEngine = new PhysicsEngine();
    chunkManager = new ChunkManager(physicsEngine);
    chunkIo = new ChunkIOManager(saveFileIom->getSearchDirectory().getString());
    particleEngine = new ParticleEngine();
    generator = terrainGenerator;

    // Init the mapper that will handle spherical voxel mapping
    voxelPlanetMapper = new vvox::VoxelPlanetMapper((i32)sphericalTerrainData->getRadius() / CHUNK_WIDTH);

    // Set up the chunk manager
    chunkManager->initialize(gpos, voxelPlanetMapper,
                               generator,
                               startingMapData,
                               chunkIo);
}