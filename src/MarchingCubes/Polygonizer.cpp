#include "MarchingCubes/Polygonizer.h"

#include <glm/geometric.hpp>

#include "MarchingCubes/MarchingCubesTable.h"

// Linear interpolation on edge between p1,p2 with SDF values v1,v2
static glm::vec3 vertexInterp(const glm::vec3& p1, const glm::vec3& p2, float v1, float v2) {
    float t = v1 / (v1 - v2);
    return p1 + t * (p2 - p1);
}

void Polygonizer::PolygonizeSurface(const std::function<float(const glm::vec3&)>& sdf, const glm::vec3& minCorner,
                                    const glm::vec3& maxCorner, int resolution, std::vector<float>& outVerts, std::vector<unsigned int>& outIdx)
{
    glm::vec3 size = maxCorner - minCorner;
    float dx = size.x / float(resolution);
    float dy = size.y / float(resolution);
    float dz = size.z / float(resolution);

    // 1) Sample SDF on (resolution+1)^3 grid
    int stride = resolution + 1;
    std::vector<float> grid(stride*stride*stride);
    auto idx3 = [&](int i, int j, int k){ return i + stride*(j + stride*k); };

    for(int k = 0; k <= resolution; ++k) {
        for(int j = 0; j <= resolution; ++j) {
            for(int i = 0; i <= resolution; ++i) {
                glm::vec3 p = minCorner + glm::vec3(i*dx, j*dy, k*dz);
                grid[idx3(i,j,k)] = sdf(p);
            }
        }
    }

    // 2) Marching Cubes over each cell
    for(int k = 0; k < resolution; ++k) {
        for(int j = 0; j < resolution; ++j) {
            for(int i = 0; i < resolution; ++i) {
                // corner indices & positions & values
                int cornerIdx[8] = {
                    idx3(i  ,j  ,k),
                    idx3(i+1,j  ,k),
                    idx3(i+1,j+1,k),
                    idx3(i  ,j+1,k),
                    idx3(i  ,j  ,k+1),
                    idx3(i+1,j  ,k+1),
                    idx3(i+1,j+1,k+1),
                    idx3(i  ,j+1,k+1)
                };
                glm::vec3 pos[8];
                float val[8];
                for(int c = 0; c < 8; ++c) {
                    val[c] = grid[cornerIdx[c]];
                    int xi = i + ((c==1||c==2||c==5||c==6)?1:0);
                    int yj = j + ((c>=2&&c<=3)||(c>=6&&c<=7)?1:0);
                    int zk = k + (c>=4?1:0);
                    pos[c] = minCorner + glm::vec3(xi*dx, yj*dy, zk*dz);
                }

                // build cubeIndex
                int cubeIndex = 0;
                for(int c = 0; c < 8; ++c)
                    if(val[c] < 0.0f) cubeIndex |= (1<<c);
                int edges = MarchingCubes::EDGE_TABLE[cubeIndex];
                if(edges == 0) continue;

                // interpolate vertices on each intersected edge
                glm::vec3 vertList[12];
                if(edges & 1)   vertList[0]  = vertexInterp(pos[0], pos[1], val[0], val[1]);
                if(edges & 2)   vertList[1]  = vertexInterp(pos[1], pos[2], val[1], val[2]);
                if(edges & 4)   vertList[2]  = vertexInterp(pos[2], pos[3], val[2], val[3]);
                if(edges & 8)   vertList[3]  = vertexInterp(pos[3], pos[0], val[3], val[0]);
                if(edges & 16)  vertList[4]  = vertexInterp(pos[4], pos[5], val[4], val[5]);
                if(edges & 32)  vertList[5]  = vertexInterp(pos[5], pos[6], val[5], val[6]);
                if(edges & 64)  vertList[6]  = vertexInterp(pos[6], pos[7], val[6], val[7]);
                if(edges & 128) vertList[7]  = vertexInterp(pos[7], pos[4], val[7], val[4]);
                if(edges & 256) vertList[8]  = vertexInterp(pos[0], pos[4], val[0], val[4]);
                if(edges & 512) vertList[9]  = vertexInterp(pos[1], pos[5], val[1], val[5]);
                if(edges & 1024)vertList[10] = vertexInterp(pos[2], pos[6], val[2], val[6]);
                if(edges & 2048)vertList[11] = vertexInterp(pos[3], pos[7], val[3], val[7]);

                // generate triangles
                for(int t = 0; MarchingCubes::TRI_TABLE[cubeIndex][t] != -1; t += 3) {
                    unsigned int base = outVerts.size() / 8;
                    for(int m = 0; m < 3; ++m) {
                        glm::vec3 v = vertList[ MarchingCubes::TRI_TABLE[cubeIndex][t + m] ];
                        // approximate normal via central difference
                        const float eps = 1e-3f;
                        glm::vec3 n = glm::normalize(glm::vec3(
                            sdf(v + glm::vec3(eps,0,0)) - sdf(v - glm::vec3(eps,0,0)),
                            sdf(v + glm::vec3(0,eps,0)) - sdf(v - glm::vec3(0,eps,0)),
                            sdf(v + glm::vec3(0,0,eps)) - sdf(v - glm::vec3(0,0,eps))
                        ));
                        // push: pos(xyz), normal(xyz), UV(0,0)
                        outVerts.insert(outVerts.end(), {
                            v.x, v.y, v.z,
                            n.x, n.y, n.z,
                            0.0f, 0.0f
                        });
                    }
                    outIdx.insert(outIdx.end(), { base, base+1, base+2 });
                }
            }
        }
    }
}
