#include "ChunkMesher.h"

#include "SubChunk.h"
#include <sstream>
#include <memory>

namespace Engine {
    
    const glm::vec3 ChunkMesher::CUBE_VERTICES[] =
    {
        glm::vec3(0.0f, 0.0f, 0.0f), // 0
        glm::vec3(1.0f, 0.0f, 0.0f), // 1
        glm::vec3(1.0f, 0.0f, 1.0f), // 2
        glm::vec3(0.0f, 0.0f, 1.0f), // 3
        glm::vec3(0.0f, 1.0f, 0.0f), // 4
        glm::vec3(1.0f, 1.0f, 0.0f), // 5
        glm::vec3(1.0f, 1.0f, 1.0f), // 6
        glm::vec3(0.0f, 1.0f, 1.0f)  // 7
    };

    const glm::vec3 CUBE_NORMALS[] =
    {
        glm::vec3( 0,  1,  0), 
        glm::vec3( 0, -1,  0),
        glm::vec3( 1,  0,  0), 
        glm::vec3(-1,  0,  0),
        glm::vec3( 0,  0,  1), 
        glm::vec3( 0,  0, -1)
    };

    bool ChunkMesher::Left = false;
    bool ChunkMesher::Right = false;
    bool ChunkMesher::Front = false;
    bool ChunkMesher::Back = false;
    bool ChunkMesher::Top = false;
    bool ChunkMesher::Bottom = false;

    glm::vec3 ChunkMesher::Position = glm::vec3(0, 0, 0);

    std::unique_ptr<std::vector<QuadVertex>> CurrentArray = std::unique_ptr<std::vector<QuadVertex>>(new std::vector<QuadVertex>());

    void ChunkMesher::Init() {
        CurrentArray->reserve(100000);
    }

    std::unique_ptr<std::vector<QuadVertex>>* ChunkMesher::MeshSubChunk(SubChunk* subchunk) 
	{
        //if (SubChunk->GetCount() == 0)
        //    return std::vector<QuadVertex>();
        CurrentArray->clear();
        //CurrentArray->shrink_to_fit();
        int type;
        //for (int x = 0; x < SubChunk::SIZE; x++) {
        //    for (int y = 0; y < SubChunk::SIZE; y++) {
        //        for (int z = 0; z < SubChunk::SIZE; z++) {
        //            type = subchunk->GetBlock(x, y, z);
        //
        //            if (type == 0)// air
        //                continue;
        //            try {
        //                CreateBlock(x, y, z, type, subchunk);
        //            }
        //            catch (const std::bad_alloc&) {
        //                printf(("bad alloc size:" + std::to_string(CurrentArray->size()) + "\n").c_str());
        //            }
        //        }
        //    }
        //}
        CreateBlock(0, 0, 0, 1, subchunk);
        printf(( "mesh size: " + std::to_string(CurrentArray->size()) + "\n").c_str());
        return &CurrentArray;
	}
    void ChunkMesher::CreateBlock(int x, int y, int z, int type, SubChunk* chunk)
    {
        
        int chunkIdx = chunk->GetIndex();
        auto parent = chunk->GetParent();
        glm::vec2 parentPos = parent->GetPosition();
        int gx = parentPos.x * SubChunk::SIZE + x;
        int gy = (chunk->GetIndex() * SubChunk::SIZE) + y;
        int gz = parentPos.y * SubChunk::SIZE + z;

        Top    = y != SubChunk::SIZE - 1 ? chunk->GetBlock(x, y + 1, z) == 0 : false;
        Bottom = y != 0                  ? chunk->GetBlock(x, y - 1, z) == 0 : false;
        Right   = x != SubChunk::SIZE - 1 ? chunk->GetBlock(x + 1, y, z) == 0 : false; // Todo chunk borders.
        Left  = x != 0                  ? chunk->GetBlock(x - 1, y, z) == 0 : false;
        Front  = z != SubChunk::SIZE - 1 ? chunk->GetBlock(x, y, z + 1) == 0 : false;
        Back   = z != 0                  ? chunk->GetBlock(x, y, z - 1) == 0 : false;

        // Block is surrounded.
        if (Top && Bottom && Left && Right && Front && Back)
            return;

        bool topChunk = false;
        bool bottomChunk = false;

        // In between subchunks.
        if (chunkIdx != Chunk::SUBCHUNK_COUNT - 1) {
            // TODO: Chunk count check.
            SubChunk& above = parent->GetSubChunk(chunkIdx + 1);
            topChunk = above.GetBlock(x, 0, z) == 0;
        }
        if (chunkIdx != 0) {
            // TODO: Chunk count check.
            SubChunk& under = parent->GetSubChunk(chunkIdx - 1);
            bottomChunk = under.GetBlock(x, SubChunk::SIZE - 1, z) == 0;
        }

        bool topBorder    = y == SubChunk::SIZE - 1 ? topChunk :    Top;
        bool bottomBorder = y == 0                  ? bottomChunk : Bottom;

        // Todo Block texture and type.
        if (topBorder)
            PushQuad(0, gx, gy, gz, 4, 5, 6, 7);
        if (bottomBorder)
            PushQuad(1, gx, gy, gz, 3, 2, 1, 0);
        if (Left)
            PushQuad(2, gx, gy, gz, 0, 4, 7, 3);
        if (Right)
            PushQuad(3, gx, gy, gz, 1, 2, 6, 5);
        if (Front)
            PushQuad(4, gx, gy, gz, 2, 3, 7, 6);
        if (Back)
            PushQuad(5, gx, gy, gz, 5, 4, 0, 1);

    }

    void ChunkMesher::PushQuadAO(int face, int x, int y, int z, int c1, int c2, int c3, int c4)
    {
        float a00 = AOLookUp(face, x, y, z, c1);
        float a10 = AOLookUp(face, x, y, z, c2);
        float a11 = AOLookUp(face, x, y, z, c3);
        float a01 = AOLookUp(face, x, y, z, c4);

        if (a00 + a11 < a01 + a10) {
            PushTriangleAO(x, y, z, c1, c2, c3, a00, a10, a11);
            PushTriangleAO(x, y, z, c1, c3, c4, a00, a11, a01);
        }
        else {
            PushTriangleAO(x, y, z, c1, c2, c4, a00, a10, a01);
            PushTriangleAO(x, y, z, c1, c3, c4, a10, a11, a01);
        }
    }

    float ChunkMesher::AOLookUp(int face, int x, int y, int z, int vertex)
    {
        //int corner = 0, side1 = 0, side2 = 0;
        //if (face != 0 && face != 1)
        //{
        //    if (vertex == 0)
        //    {
        //        side1 = Back ? 0 : 1;
        //        side2 = Left ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x - 1, y, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 1)
        //    {

        //        side1 = Right ? 0 : 1;
        //        side2 = Back ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x + 1, y, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 2)
        //    {

        //        side1 = Front ? 0 : 1;
        //        side2 = Right ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x + 1, y, z + 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 3)
        //    {

        //        side1 = Left ? 0 : 1;
        //        side2 = Front ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x - 1, y, z + 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 4)
        //    {

        //        side1 = Back ? 0 : 1;
        //        side2 = Left ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x - 1, y, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 5)
        //    {

        //        side1 = Right ? 0 : 1;
        //        side2 = Back ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x + 1, y, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 6)
        //    {

        //        side1 = Front ? 0 : 1;
        //        side2 = Right ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x + 1, y, z + 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 7)
        //    {

        //        side1 = Left ? 0 : 1;
        //        side2 = Front ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x - 1, y, z + 1) == 0 ? 0 : 1;
        //    }

        //}
        //else
        //{
        //    if (vertex == 4)
        //    {
        //        side1 = ChunkManager::GetBlock(x, y + 1, z - 1) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x - 1, y + 1, z) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager.GetBlock(x - 1, y + 1, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 5)
        //    {

        //        side1 = ChunkManager::GetBlock(x + 1, y + 1, z) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x, y + 1, z - 1) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager.GetBlock(x + 1, y + 1, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 6)
        //    {

        //        side1 = ChunkManager::GetBlock(x, y + 1, z + 1) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x + 1, y + 1, z) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager.GetBlock(x + 1, y + 1, z + 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 7)
        //    {

        //        side1 = ChunkManager::GetBlock(x - 1, y + 1, z) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x, y + 1, z + 1) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager.GetBlock(x - 1, y + 1, z + 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 0)
        //    {

        //        side1 = ChunkManager::GetBlock(x, y - 1, z - 1) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x - 1, y - 1, z) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager.GetBlock(x - 1, y - 1, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 1)
        //    {

        //        side1 = ChunkManager::GetBlock(x + 1, y - 1, z) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x, y - 1, z - 1) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x + 1, y - 1, z - 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 2)
        //    {

        //        side1 = ChunkManager::GetBlock(x, y - 1, z + 1) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x + 1, y - 1, z) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager::GetBlock(x + 1, y - 1, z + 1) == 0 ? 0 : 1;
        //    }
        //    if (vertex == 3)
        //    {

        //        side1 = ChunkManager::GetBlock(x - 1, y - 1, z) == 0 ? 0 : 1;
        //        side2 = ChunkManager::GetBlock(x, y - 1, z + 1) == 0 ? 0 : 1;
        //        if (side1 == 0 || side2 == 0)
        //            corner = ChunkManager.GetBlock(x - 1, y - 1, z + 1) == 0 ? 0 : 1;
        //    }
        //}
        return 0.0f;
    }

    float ChunkMesher::PushTriangleAO(int x, int y, int z, int v1, int v2, int v3, float a1, float a2, float a3)
    {
        return 0.0f;
    }

    void ChunkMesher::PushQuad(int face, int x, int y, int z, int c1, int c2, int c3, int c4)
    {
        float light;
        if (face == 0)
            light = 1.0f;
        if (face == 1)
            light = 0.5f;
        if (face == 2)
            light = 0.7f;
        if (face == 3)
            light = 0.8f;
        if (face == 4)
            light =0.9f;
        if (face == 5)
            light = 0.75f;
        CurrentArray->push_back(std::move(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c1].x), float(y + CUBE_VERTICES[c1].y), float(z + CUBE_VERTICES[c1].z)),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(0.0f, 0.0f),
            1.0f,
            1.0f
            }));

        CurrentArray->push_back(std::move(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c2].x), float(y + CUBE_VERTICES[c2].y), float(z + CUBE_VERTICES[c2].z)),
            glm::vec3(0.0f, 1.0f, 1.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(1.0f, 0.0f),
            1.0f,
            1.0f
            }));


        CurrentArray->push_back(std::move(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c3].x), float(y + CUBE_VERTICES[c3].y), float(z + CUBE_VERTICES[c3].z)),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(1.0f, 1.0f),
            1.0f,
            1.0f
            }));

        CurrentArray->push_back(std::move(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c4].x), float(y + CUBE_VERTICES[c4].y), float(z + CUBE_VERTICES[c4].z)),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(0.0f, 1.0f),
            1.0f,
            1.0f
            }));
    }
}