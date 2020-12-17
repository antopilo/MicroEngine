#include "ChunkMesher.h"
#include "SubChunk.h"
#include "../Rendering/Vertex.h"
#include "Chunk.h"
#include <sstream>
#include <memory>
#include "../ChunkManager.h"
#include "../Generation/BlockColorer.h"
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

    std::vector<QuadVertex>* ChunkMesher::CurrentArray; 

    void ChunkMesher::Init() {
        CurrentArray = new std::vector<QuadVertex>();
        CurrentArray->reserve(100000);
    }

    void ChunkMesher::Clear() {
        delete CurrentArray;
    }

    std::vector<QuadVertex>* ChunkMesher::MeshSubChunk(SubChunk* subchunk) 
	{
        CurrentArray = new std::vector<QuadVertex>();
        //if (SubChunk->GetCount() == 0)
        //    return std::vector<QuadVertex>();

        int type;
        for (int x = 0; x < SubChunk::SIZE; x++) {
            for (int y = 0; y < SubChunk::SIZE; y++) {
                for (int z = 0; z < SubChunk::SIZE; z++) {
                    type = subchunk->GetBlock(x, y, z);
        
                    if (type == 0)// air
                        continue;
                    try {
                        CreateBlock(x, y, z, type, subchunk);
                    }
                    catch (const std::bad_alloc&) {
                        printf(("bad alloc size:" + std::to_string(CurrentArray->size()) + "\n").c_str());
                    }
                }
            }
        }

        return CurrentArray;
	}
    void ChunkMesher::CreateBlock(int x, int y, int z, int type, SubChunk* chunk)
    {
        
        int chunkIdx = chunk->GetIndex();
        auto parent = chunk->GetParent();
        glm::vec2 parentPos = parent->GetPosition();
        int gx = parentPos.x * SubChunk::SIZE + x;
        int gy = (chunk->GetIndex() * SubChunk::SIZE) + y;
        int gz = parentPos.y * SubChunk::SIZE + z;

        Top    = y != SubChunk::SIZE - 1 ? chunk->GetBlock(x, y + 1, z) == 0 : true;
        Bottom = y != 0                  ? chunk->GetBlock(x, y - 1, z) == 0 : true;

        Right = x != SubChunk::SIZE - 1 ? chunk->GetBlock(x + 1, y, z) == 0 : chunk->GetParent()->Right->GetSubChunk(chunk->GetIndex()).GetBlock(0, y, z) == 0; // Todo chunk borders.
        Left  = x != 0                  ? chunk->GetBlock(x - 1, y, z) == 0 :   chunk->GetParent()->Left->GetSubChunk(chunk->GetIndex()).GetBlock(SubChunk::SIZE - 1, y, z) == 0;
        Front  = z != SubChunk::SIZE - 1 ? chunk->GetBlock(x, y, z + 1) == 0 :  chunk->GetParent()->Front->GetSubChunk(chunk->GetIndex()).GetBlock(x, y, 0) == 0;
        Back   = z != 0                  ? chunk->GetBlock(x, y, z - 1) == 0 :  chunk->GetParent()->Back->GetSubChunk(chunk->GetIndex()).GetBlock(x, y, SubChunk::SIZE - 1) == 0;

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
        if (topBorder || gy + 1 == SubChunk::SIZE * Chunk::SUBCHUNK_COUNT)
            PushQuadAO(0, gx, gy, gz, 4, 5, 6, 7);
        if (bottomBorder)
            PushQuadAO(1, gx, gy, gz, 3, 2, 1, 0);
        if (Left)
            PushQuadAO(2, gx, gy, gz, 0, 4, 7, 3);
        if (Right)
            PushQuadAO(3, gx, gy, gz, 1, 2, 6, 5);
        if (Front)
            PushQuadAO(4, gx, gy, gz, 2, 3, 7, 6);
        if (Back)
            PushQuadAO(5, gx, gy, gz, 5, 4, 0, 1);

    }

    //void ChunkMesher::PushQuadAO(int face, int x, int y, int z, int c1, int c2, int c3, int c4)
    //{
    //    float a00 = AOLookUp(face, x, y, z, c1);
    //    float a10 = AOLookUp(face, x, y, z, c2);
    //    float a11 = AOLookUp(face, x, y, z, c3);
    //    float a01 = AOLookUp(face, x, y, z, c4);
    //
    //    if (a00 + a11 < a01 + a10) {
    //        PushTriangleAO(x, y, z, c1, c2, c3, a00, a10, a11);
    //        PushTriangleAO(x, y, z, c1, c3, c4, a00, a11, a01);
    //    }
    //    else {
    //        PushTriangleAO(x, y, z, c1, c2, c4, a00, a10, a01);
    //        PushTriangleAO(x, y, z, c1, c3, c4, a10, a11, a01);
    //    }
    //}

    float ChunkMesher::AOLookUp(int face, int x, int y, int z, int vertex)
    {
        int corner = 0, side1 = 0, side2 = 0;
        if (face != 0 && face != 1)
        {
            if (vertex == 0)
            {
                side1 = Back ? 0 : 1;
                side2 = Left ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 1)
            {

                side1 = Right ? 0 : 1;
                side2 = Back ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 2)
            {

                side1 = Front ? 0 : 1;
                side2 = Right ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y, z + 1) == 0 ? 0 : 1;
            }
            if (vertex == 3)
            {

                side1 = Left ? 0 : 1;
                side2 = Front ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y, z + 1) == 0 ? 0 : 1;
            }
            if (vertex == 4)
            {

                side1 = Back ? 0 : 1;
                side2 = Left ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 5)
            {

                side1 = Right ? 0 : 1;
                side2 = Back ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 6)
            {

                side1 = Front ? 0 : 1;
                side2 = Right ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y, z + 1) == 0 ? 0 : 1;
            }
            if (vertex == 7)
            {

                side1 = Left ? 0 : 1;
                side2 = Front ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y, z + 1) == 0 ? 0 : 1;
            }

        }
        else
        {
            if (vertex == 4)
            {
                side1 = ChunkManager::GetBlock(x, y + 1, z - 1) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x - 1, y + 1, z) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y + 1, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 5)
            {

                side1 = ChunkManager::GetBlock(x + 1, y + 1, z) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x, y + 1, z - 1) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y + 1, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 6)
            {

                side1 = ChunkManager::GetBlock(x, y + 1, z + 1) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x + 1, y + 1, z) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y + 1, z + 1) == 0 ? 0 : 1;
            }
            if (vertex == 7)
            {

                side1 = ChunkManager::GetBlock(x - 1, y + 1, z) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x, y + 1, z + 1) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y + 1, z + 1) == 0 ? 0 : 1;
            }
            if (vertex == 0)
            {

                side1 = ChunkManager::GetBlock(x, y - 1, z - 1) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x - 1, y - 1, z) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y - 1, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 1)
            {

                side1 = ChunkManager::GetBlock(x + 1, y - 1, z) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x, y - 1, z - 1) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y - 1, z - 1) == 0 ? 0 : 1;
            }
            if (vertex == 2)
            {

                side1 = ChunkManager::GetBlock(x, y - 1, z + 1) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x + 1, y - 1, z) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x + 1, y - 1, z + 1) == 0 ? 0 : 1;
            }
            if (vertex == 3)
            {

                side1 = ChunkManager::GetBlock(x - 1, y - 1, z) == 0 ? 0 : 1;
                side2 = ChunkManager::GetBlock(x, y - 1, z + 1) == 0 ? 0 : 1;
                if (side1 == 0 || side2 == 0)
                    corner = ChunkManager::GetBlock(x - 1, y - 1, z + 1) == 0 ? 0 : 1;
            }
        }
        return VertexAO(side1, side2, corner);
    }

    int ChunkMesher::VertexAO(int side1, int side2, int corner) {
        if (side1 == 1 && side2 == 1)
        {
            return 3;
        }
        return (side1 + side2 + corner);
    }

    float ChunkMesher::PushTriangleAO(int x, int y, int z, int v1, int v2, int v3, float a1, float a2, float a3)
    {
        return 0.0f;
    }

    void ChunkMesher::PushQuadAO(int face, int x, int y, int z, int c1, int c2, int c3, int c4) {
        float a00 =  1.0f - (AOLookUp(face, x, y, z, c1) * .2f);
        float a10 =  1.0f - (AOLookUp(face, x, y, z, c2) * .2f);
        float a11 =  1.0f - (AOLookUp(face, x, y, z, c3) * .2f);
        float a01 =  1.0f - (AOLookUp(face, x, y, z, c4) * .2f);
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
            light = 0.9f;
        if (face == 5)
            light = 0.75f;

        glm::vec4 color = BlockColorer::Darken(BlockColorer::GetBlockColor(1), light);
       
        if (a00 + a11 < a01 + a10)
        {
            CurrentArray->push_back(QuadVertex{
              glm::vec3(float(x + CUBE_VERTICES[c1].x), float(y + CUBE_VERTICES[c1].y), float(z + CUBE_VERTICES[c1].z)),
              glm::vec3(0.0f, 1.0f, 0.0f),
              BlockColorer::Darken(color, a00),
              glm::vec2(0.0f, 0.0f),
              1.0f,
              1.0f
            });

            CurrentArray->push_back(QuadVertex{
               glm::vec3(float(x + CUBE_VERTICES[c2].x), float(y + CUBE_VERTICES[c2].y), float(z + CUBE_VERTICES[c2].z)),
               glm::vec3(0.0f, 1.0f, 1.0f),
               BlockColorer::Darken(color, a10),
               glm::vec2(1.0f, 0.0f),
               1.0f,
               1.0f
            });

            CurrentArray->push_back(QuadVertex{
                glm::vec3(float(x + CUBE_VERTICES[c3].x), float(y + CUBE_VERTICES[c3].y), float(z + CUBE_VERTICES[c3].z)),
                glm::vec3(0.0f, 1.0f, 0.0f),
                BlockColorer::Darken(color, a11),
                glm::vec2(1.0f, 1.0f),
                1.0f,
                1.0f
            });

            CurrentArray->push_back(QuadVertex{
                glm::vec3(float(x + CUBE_VERTICES[c4].x), float(y + CUBE_VERTICES[c4].y), float(z + CUBE_VERTICES[c4].z)),
                glm::vec3(0.0f, 0.0f, 0.0f),
                BlockColorer::Darken(color, a01),
                glm::vec2(0.0f, 1.0f),
                1.0f,
                1.0f
            });
            //PushTriangleAO(x, y, z, c1, c2, c3, color, a00, a10, a11, CurrentArray);
            //PushTriangleAO(x, y, z, c1, c3, c4, color, a00, a11, a01, CurrentArray);
        }
        else
        {
            //PushTriangleAO(x, y, z, c1, c2, c4, color, a00, a10, a01, CurrentArray);
            //PushTriangleAO(x, y, z, c2, c3, c4, color, a10, a11, a01, CurrentArray);

            CurrentArray->push_back(QuadVertex{
               glm::vec3(float(x + CUBE_VERTICES[c1].x), float(y + CUBE_VERTICES[c1].y), float(z + CUBE_VERTICES[c1].z)),
               glm::vec3(0.0f, 1.0f, 0.0f),
               BlockColorer::Darken(color, a00),
               glm::vec2(0.0f, 0.0f),
               1.0f,
               1.0f
            });

            CurrentArray->push_back(QuadVertex{
               glm::vec3(float(x + CUBE_VERTICES[c2].x), float(y + CUBE_VERTICES[c2].y), float(z + CUBE_VERTICES[c2].z)),
               glm::vec3(0.0f, 1.0f, 1.0f),
               BlockColorer::Darken(color, a10),
               glm::vec2(1.0f, 0.0f),
               1.0f,
               1.0f
            });

            CurrentArray->push_back(QuadVertex{
                glm::vec3(float(x + CUBE_VERTICES[c3].x), float(y + CUBE_VERTICES[c3].y), float(z + CUBE_VERTICES[c3].z)),
                glm::vec3(0.0f, 1.0f, 0.0f),
                BlockColorer::Darken(color, a11),
                glm::vec2(1.0f, 1.0f),
                1.0f,
                1.0f
            });

            CurrentArray->push_back(QuadVertex{
               glm::vec3(float(x + CUBE_VERTICES[c4].x), float(y + CUBE_VERTICES[c4].y), float(z + CUBE_VERTICES[c4].z)),
               glm::vec3(0.0f, 0.0f, 0.0f),
               BlockColorer::Darken(color, a01),
               glm::vec2(0.0f, 1.0f),
               1.0f,
               1.0f
            });

        }

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
        CurrentArray->push_back(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c1].x), float(y + CUBE_VERTICES[c1].y), float(z + CUBE_VERTICES[c1].z)),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(0.0f, 0.0f),
            1.0f,
            1.0f
            });

        CurrentArray->push_back(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c2].x), float(y + CUBE_VERTICES[c2].y), float(z + CUBE_VERTICES[c2].z)),
            glm::vec3(0.0f, 1.0f, 1.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(1.0f, 0.0f),
            1.0f,
            1.0f
            });


        CurrentArray->push_back(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c3].x), float(y + CUBE_VERTICES[c3].y), float(z + CUBE_VERTICES[c3].z)),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(1.0f, 1.0f),
            1.0f,
            1.0f
            });

        CurrentArray->push_back(QuadVertex{
            glm::vec3(float(x + CUBE_VERTICES[c4].x), float(y + CUBE_VERTICES[c4].y), float(z + CUBE_VERTICES[c4].z)),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec4(1.0f * light, 1.0f * light, 1.0f * light, 1.0f),
            glm::vec2(0.0f, 1.0f),
            1.0f,
            1.0f
            });
    }
}