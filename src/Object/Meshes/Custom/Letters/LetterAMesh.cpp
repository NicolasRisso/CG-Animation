#include "Object/Meshes/Custom/Letters/LetterAMesh.h"

void LetterAMesh::setupMesh(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    vertices.clear();
    indices.clear();

    float m_Depth = 0.2f;
    
    // Parâmetros
    const float w    = m_Width;
    const float h    = m_Height;
    const float t    = m_BarThickness;
    const float barY = h * m_BarHeightRatio;
    const float halfD= m_Depth * 0.5f;

    // Helper: cria um vértice (x,y,z) + normal (nx,ny,nz) + uv (u,v)
    auto pushVertex = [&](const glm::vec3& P,
                          const glm::vec3& N,
                          const glm::vec2& UV)
    {
        vertices.insert(vertices.end(), { P.x, P.y, P.z,
                                    N.x, N.y, N.z,
                                    UV.x, UV.y });
    };

    // --- 1) Triangular prism para perna esquerda ---
    glm::vec2 p0_L(0.0f, 0.0f);
    glm::vec2 p1_L(t,   0.0f);
    glm::vec2 p2_L(w*0.5f, h);
    {
        // 6 vértices (triangular prism: 3 front, 3 back)
        unsigned int base = vertices.size()/8;
        glm::vec3 f0{p0_L.x, p0_L.y, +halfD};
        glm::vec3 f1{p1_L.x, p1_L.y, +halfD};
        glm::vec3 f2{p2_L.x, p2_L.y, +halfD};
        glm::vec3 b0{p0_L.x, p0_L.y, -halfD};
        glm::vec3 b1{p1_L.x, p1_L.y, -halfD};
        glm::vec3 b2{p2_L.x, p2_L.y, -halfD};

        // Normais front/back
        glm::vec3 nf{0,0,1}, nb{0,0,-1};
        // UVs front/back via projeção em XY
        auto uv0 = glm::vec2(p0_L.x/w, p0_L.y/h);
        auto uv1 = glm::vec2(p1_L.x/w, p1_L.y/h);
        auto uv2 = glm::vec2(p2_L.x/w, p2_L.y/h);

        // front
        pushVertex(f0,nf,uv0); //0
        pushVertex(f1,nf,uv1); //1
        pushVertex(f2,nf,uv2); //2
        // back
        pushVertex(b0,nb,uv0); //3
        pushVertex(b1,nb,uv1); //4
        pushVertex(b2,nb,uv2); //5

        // front face
        indices.insert(indices.end(), { base+0, base+1, base+2 });
        // back face (invert winding)
        indices.insert(indices.end(), { base+5, base+4, base+3 });

        // lados do prisma (3 quads, 2 tri cada)
        glm::vec2 ps[3] = { p0_L, p1_L, p2_L };
        for (int e = 0; e < 3; ++e) {
            int i0 = e, i1 = (e+1)%3;
            // calcular normal do lado
            glm::vec3 A{ps[i1].x, ps[i1].y, +halfD};
            glm::vec3 B{ps[i0].x, ps[i0].y, +halfD};
            glm::vec3 ex = glm::vec3(0,0,-m_Depth);
            glm::vec3 sideN = glm::normalize(glm::cross(ex, A - B));

            // vertices do quad: front i0, front i1, back i1, back i0
            unsigned int v0 = base + i0;
            unsigned int v1 = base + i1;
            unsigned int v2 = base + 3 + i1;
            unsigned int v3 = base + 3 + i0;

            // sobrepor normal em vértices existentes:
            // (para simplicidade, re-push vertices com esse normal)
            // mas aqui, vamos só gerar faces com índice e confiar nos normals front/back

            // tri1
            indices.insert(indices.end(), { v0, v1, v2 });
            // tri2
            indices.insert(indices.end(), { v2, v3, v0 });
        }
    }

    // --- 2) Triangular prism para perna direita ---
    glm::vec2 p0_R(w-t, 0.0f);
    glm::vec2 p1_R(w,   0.0f);
    glm::vec2 p2_R(w*0.5f, h);
    {
        unsigned int base = vertices.size()/8;
        glm::vec3 f0{p0_R.x, p0_R.y, +halfD};
        glm::vec3 f1{p1_R.x, p1_R.y, +halfD};
        glm::vec3 f2{p2_R.x, p2_R.y, +halfD};
        glm::vec3 b0{p0_R.x, p0_R.y, -halfD};
        glm::vec3 b1{p1_R.x, p1_R.y, -halfD};
        glm::vec3 b2{p2_R.x, p2_R.y, -halfD};
        glm::vec3 nf{0,0,1}, nb{0,0,-1};
        auto uv0 = glm::vec2(p0_R.x/w, p0_R.y/h);
        auto uv1 = glm::vec2(p1_R.x/w, p1_R.y/h);
        auto uv2 = glm::vec2(p2_R.x/w, p2_R.y/h);
        pushVertex(f0,nf,uv0); pushVertex(f1,nf,uv1); pushVertex(f2,nf,uv2);
        pushVertex(b0,nb,uv0); pushVertex(b1,nb,uv1); pushVertex(b2,nb,uv2);
        indices.insert(indices.end(), { base+0, base+1, base+2 });
        indices.insert(indices.end(), { base+5, base+4, base+3 });
        glm::vec2 ps[3] = { p0_R, p1_R, p2_R };
        for (int e = 0; e < 3; ++e) {
            int i0 = e, i1 = (e+1)%3;
            unsigned int v0 = base + i0;
            unsigned int v1 = base + i1;
            unsigned int v2 = base + 3 + i1;
            unsigned int v3 = base + 3 + i0;
            indices.insert(indices.end(), { v0, v1, v2 });
            indices.insert(indices.end(), { v2, v3, v0 });
        }
    }

    // --- 3) Prism retangular para a barra central ---
    glm::vec2 b0(  t,       barY - t*0.5f);
    glm::vec2 b1( w - t,    barY - t*0.5f);
    glm::vec2 b2( w - t,    barY + t*0.5f);
    glm::vec2 b3(  t,       barY + t*0.5f);
    {
        unsigned int base = vertices.size()/8;
        // front
        pushVertex({b0.x, b0.y, +halfD}, {0,0,1}, {b0.x/w, b0.y/h}); //0
        pushVertex({b1.x, b1.y, +halfD}, {0,0,1}, {b1.x/w, b1.y/h}); //1
        pushVertex({b2.x, b2.y, +halfD}, {0,0,1}, {b2.x/w, b2.y/h}); //2
        pushVertex({b3.x, b3.y, +halfD}, {0,0,1}, {b3.x/w, b3.y/h}); //3
        // back
        pushVertex({b0.x, b0.y, -halfD}, {0,0,-1},{b0.x/w, b0.y/h}); //4
        pushVertex({b1.x, b1.y, -halfD}, {0,0,-1},{b1.x/w, b1.y/h}); //5
        pushVertex({b2.x, b2.y, -halfD}, {0,0,-1},{b2.x/w, b2.y/h}); //6
        pushVertex({b3.x, b3.y, -halfD}, {0,0,-1},{b3.x/w, b3.y/h}); //7

        // faces front/back
        indices.insert(indices.end(), { base+0, base+1, base+2,  base+2, base+3, base+0 });
        indices.insert(indices.end(), { base+7, base+6, base+5,  base+5, base+4, base+7 });

        // 4 lados do retângulo extrudado
        unsigned int quadv[4][2] = {
            {0,1}, {1,2}, {2,3}, {3,0}
        };
        for (int e = 0; e < 4; ++e) {
            unsigned int i0 = quadv[e][0];
            unsigned int i1 = quadv[e][1];
            unsigned int v0 = base + i0;
            unsigned int v1 = base + i1;
            unsigned int v2 = base + 4 + i1;
            unsigned int v3 = base + 4 + i0;
            indices.insert(indices.end(), { v0, v1, v2, v2, v3, v0 });
        }
    }
}
