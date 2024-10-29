#include "pch.h"
#include "Mesh.hpp"

Mesh::Mesh(int material) : VAO(0), VBO(0), EBO(0), positionVBO(0), texCoordVBO(0), normalVBO(0), isBuilt(false), material(material)
{
    activeAttributes = POSITION | TEXCOORD | NORMAL | FACES;
}

Mesh::~Mesh()
{
    Release();
}

void Mesh::Build()
{
    if (positions.size() == 0 || indices.size() == 0)
    {
        return;
    }
    if (isBuilt)
    {
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &positionVBO);
    glGenBuffers(1, &texCoordVBO);
    glGenBuffers(1, &normalVBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    if (texCoords.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0);
        glEnableVertexAttribArray(1);
    }
    if (normals.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    activeAttributes = 0;

    isBuilt = true;
}

void Mesh::Update()
{
    if (positions.size() == 0 || indices.size() == 0)
    {
        return;
    }
    if (!isBuilt)
    {
        Build();
    }
    glBindVertexArray(VAO);

    if (activeAttributes & POSITION)
    {
        glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3), &positions[0]);
    }
    if (texCoords.size() > 0)
    {
        if (activeAttributes & TEXCOORD)
        {
            glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, texCoords.size() * sizeof(glm::vec2), &texCoords[0]);
        }
    }

    if (normals.size() > 0)
    {
        if (activeAttributes & NORMAL)
        {
            glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(glm::vec3), &normals[0]);
        }
    }

    if (activeAttributes & FACES)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), &indices[0]);
    }

    activeAttributes = 0;
    glBindVertexArray(0);
}

void Mesh::Render()
{
    if (positions.size() == 0 || indices.size() == 0)
    {
        return;
    }
    if (!isBuilt)
    {
        Build();
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Release()
{
    if (!isBuilt)
        return;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &positionVBO);
    glDeleteBuffers(1, &texCoordVBO);
    glDeleteBuffers(1, &normalVBO);
    glDeleteBuffers(1, &EBO);

    VAO = VBO = EBO = positionVBO = texCoordVBO = normalVBO = 0;
    isBuilt = false;
}

// std::vector<Edge> Mesh::GetEdges() const
// {
//     std::vector<Edge> edges;

//     for (size_t i = 0; i < indices.size(); i += 3)
//     {
//         // Cada face é composta por três vértices
//         unsigned int v0 = indices[i];
//         unsigned int v1 = indices[i + 1];
//         unsigned int v2 = indices[i + 2];

//         // Cria as três arestas da face
//         Edge edge1(positions[v0], positions[v1]);
//         Edge edge2(positions[v1], positions[v2]);
//         Edge edge3(positions[v2], positions[v0]);

//         // Adiciona cada aresta ao vetor de arestas, evitando duplicação
//         auto AddUniqueEdge = [&](const Edge &edge)
//         {
//             auto it = std::find_if(edges.begin(), edges.end(), [&](const Edge &e)
//                                    { return e.Equals(edge); });
//             if (it == edges.end())
//             {
//                 edges.push_back(edge);
//             }
//         };

//         AddUniqueEdge(edge1);
//         AddUniqueEdge(edge2);
//         AddUniqueEdge(edge3);
//     }

//     return edges;
// }

// glm::vec3 Mesh::GetFaceNormal(const Edge &edge) const
// {

//     for (size_t i = 0; i < indices.size(); i += 3)
//     {
//         unsigned int v0 = indices[i];
//         unsigned int v1 = indices[i + 1];
//         unsigned int v2 = indices[i + 2];

//         glm::vec3 vert0 = positions[v0];
//         glm::vec3 vert1 = positions[v1];
//         glm::vec3 vert2 = positions[v2];

//         if ((edge.start == vert0 && edge.end == vert1) ||
//             (edge.start == vert1 && edge.end == vert2) ||
//             (edge.start == vert2 && edge.end == vert0))
//         {

//             glm::vec3 edge1 = vert1 - vert0;
//             glm::vec3 edge2 = vert2 - vert0;
//             return glm::normalize(glm::cross(edge1, edge2));
//         }
//     }

//     return glm::vec3(0.0f);
// }
size_t Mesh::AddVertex(const glm::vec3 &position)
{
    positions.push_back(position);
    activeAttributes |= POSITION;
    return positions.size();
}

size_t Mesh::AddVertex(float x, float y, float z)
{
    positions.emplace_back(x, y, z);
    activeAttributes |= POSITION;
    return positions.size();
}

size_t Mesh::AddVertex(const glm::vec3 &position, const glm::vec2 &texCoord)
{
    positions.push_back(position);
    texCoords.push_back(texCoord);
    activeAttributes |= (POSITION | TEXCOORD);
    return positions.size();
}

size_t Mesh::AddVertex(float x, float y, float z, float u, float v)
{
    positions.emplace_back(x, y, z);
    texCoords.emplace_back(u, v);
    activeAttributes |= (POSITION | TEXCOORD);
    return positions.size();
}

size_t Mesh::AddVertex(const glm::vec3 &position, const glm::vec2 &texCoord, const glm::vec3 &normal)
{
    positions.push_back(position);
    texCoords.push_back(texCoord);
    normals.push_back(normal);
    activeAttributes |= (POSITION | TEXCOORD | NORMAL);
    return positions.size();
}

size_t Mesh::AddVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
    positions.emplace_back(x, y, z);
    texCoords.emplace_back(u, v);
    normals.emplace_back(nx, ny, nz);
    activeAttributes |= (POSITION | TEXCOORD | NORMAL);
    return positions.size();
}

void Mesh::AddIndex(unsigned int index)
{
    indices.push_back(index);
    activeAttributes |= FACES;
}

void Mesh::AddFace(int v1, int v2, int v3)
{
    indices.push_back(v1);
    indices.push_back(v2);
    indices.push_back(v3);
    activeAttributes |= FACES;
}

void Mesh::Clear()
{
    positions.clear();
    texCoords.clear();
    normals.clear();
    indices.clear();
    activeAttributes = 0;
}

int Mesh::TriangleVertex(int tri_no, int corner)
{
    int vid[3];

    tri_no = (tri_no + 1) * 3;
    vid[0] = indices[tri_no - 1];
    vid[1] = indices[tri_no - 2];
    vid[2] = indices[tri_no - 3];

    return vid[corner];
}

float Mesh::VertexX(int vertex)
{
    if (vertex < 0 || vertex >= (int)positions.size())
    {
        return 0.0f;
    }
    return positions[vertex].x;
}

float Mesh::VertexY(int vertex)
{
    if (vertex < 0 || vertex >= (int)positions.size())
    {
        return 0.0f;
    }
    return positions[vertex].y;
}

float Mesh::VertexZ(int vertex)
{
    if (vertex < 0 || vertex >= (int)positions.size())
    {
        return 0.0f;
    }
    return positions[vertex].z;
}

Mesh Mesh::CreatePlane(float width, float depth)
{
    Mesh plane;

    float halfWidth = width / 2.0f;
    float halfDepth = depth / 2.0f;

    plane.AddVertex(glm::vec3(-halfWidth, 0.0f, -halfDepth), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    plane.AddVertex(glm::vec3(-halfWidth, 0.0f, halfDepth), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    plane.AddVertex(glm::vec3(halfWidth, 0.0f, halfDepth), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    plane.AddVertex(glm::vec3(halfWidth, 0.0f, -halfDepth), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    plane.AddIndex(0);
    plane.AddIndex(1);
    plane.AddIndex(2);
    plane.AddIndex(0);
    plane.AddIndex(2);
    plane.AddIndex(3);

    plane.Build();
    return plane;
}

Mesh Mesh::CreateCube()
{
    Mesh mesh;

    // Face de trás (normal (0, 0, -1))
    mesh.AddVertex(-0.5, -0.5, -0.5, 0.0, 0.0, 0.0, 0.0, -1.0); // V0
    mesh.AddVertex(0.5, -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, -1.0);  // V1
    mesh.AddVertex(0.5, 0.5, -0.5, 1.0, 1.0, 0.0, 0.0, -1.0);   // V2
    mesh.AddVertex(-0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, -1.0);  // V3

    // Face da frente (normal (0, 0, 1););
    mesh.AddVertex(-0.5, -0.5, 0.5, 0.0, 0.0, 0.0, 0.0, 1.0); // V4
    mesh.AddVertex(0.5, -0.5, 0.5, 1.0, 0.0, 0.0, 0.0, 1.0);  // V5
    mesh.AddVertex(0.5, 0.5, 0.5, 1.0, 1.0, 0.0, 0.0, 1.0);   // V6
    mesh.AddVertex(-0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 0.0, 1.0);  // V7

    // Face da esquerda (normal (-1, 0, 0););
    mesh.AddVertex(-0.5, -0.5, 0.5, 0.0, 0.0, -1.0, 0.0, 0.0);  // V8
    mesh.AddVertex(-0.5, 0.5, 0.5, 1.0, 0.0, -1.0, 0.0, 0.0);   // V9
    mesh.AddVertex(-0.5, 0.5, -0.5, 1.0, 1.0, -1.0, 0.0, 0.0);  // V10
    mesh.AddVertex(-0.5, -0.5, -0.5, 0.0, 1.0, -1.0, 0.0, 0.0); // V11

    // Face da direita (normal (1, 0, 0););
    mesh.AddVertex(0.5, -0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0); // V12
    mesh.AddVertex(0.5, 0.5, -0.5, 1.0, 0.0, 1.0, 0.0, 0.0);  // V13
    mesh.AddVertex(0.5, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0, 0.0);   // V14
    mesh.AddVertex(0.5, -0.5, 0.5, 0.0, 1.0, 1.0, 0.0, 0.0);  // V15

    // Face de cima (normal (0, 1, 0););
    mesh.AddVertex(-0.5, 0.5, -0.5, 0.0, 0.0, 0.0, 1.0, 0.0); // V16
    mesh.AddVertex(0.5, 0.5, -0.5, 1.0, 0.0, 0.0, 1.0, 0.0);  // V17
    mesh.AddVertex(0.5, 0.5, 0.5, 1.0, 1.0, 0.0, 1.0, 0.0);   // V18
    mesh.AddVertex(-0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0);  // V19

    // Face de baixo (normal (0, -1, 0);); - Sentido anti-horário (CCW);
    mesh.AddVertex(-0.5, -0.5, -0.5, 0.0, 0.0, 0.0, -1.0, 0.0); // V20
    mesh.AddVertex(0.5, -0.5, -0.5, 1.0, 0.0, 0.0, -1.0, 0.0);  // V21
    mesh.AddVertex(0.5, -0.5, 0.5, 1.0, 1.0, 0.0, -1.0, 0.0);   // V22
    mesh.AddVertex(-0.5, -0.5, 0.5, 0.0, 1.0, 0.0, -1.0, 0.0);  // V23

    // Índices para os triângulos de cada face (organizados em sentido anti-horário)
    unsigned int indices[] = {
        2, 1, 0, 3, 2, 0,       // Face de trás
        4, 5, 6, 4, 6, 7,       // Face da frente
        8, 9, 10, 8, 10, 11,    // Face da esquerda
        12, 13, 14, 12, 14, 15, // Face da direita
        18, 17, 16, 19, 18, 16, // Face de cima
        20, 21, 22, 20, 22, 23  // Face de baixo
    };

    for (int i = 0; i < 36; i++)
    {
        mesh.AddIndex(indices[i]);
    }
    mesh.Build();
    return mesh;
}

Mesh Mesh::CreateQuad(float width, float height)
{
    Mesh quad;

    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    quad.AddVertex(glm::vec3(-halfWidth, -halfHeight, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    quad.AddVertex(glm::vec3(halfWidth, -halfHeight, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    quad.AddVertex(glm::vec3(halfWidth, halfHeight, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    quad.AddVertex(glm::vec3(-halfWidth, halfHeight, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    quad.AddIndex(0);
    quad.AddIndex(1);
    quad.AddIndex(2);
    quad.AddIndex(2);
    quad.AddIndex(3);
    quad.AddIndex(0);

    quad.Build();
    return quad;
}


void TransformVec(glm::mat4 grid, float &rx, float &ry, float &rz, int addTranslation = 0)
{
    float w = 1.0 / (grid[0][3] + grid[1][3] + grid[2][3] + grid[3][3]);
    float ix = rx;
    float iy = ry;
    float iz = rz;

    addTranslation = addTranslation & 1;

    rx = ((grid[0][0] * ix) + (grid[1][0] * iy) + (grid[2][0] * iz) + grid[3][0] * addTranslation) * w;
    ry = ((grid[0][1] * ix) + (grid[1][1] * iy) + (grid[2][1] * iz) + grid[3][1] * addTranslation) * w;
    rz = ((grid[0][2] * ix) + (grid[1][2] * iy) + (grid[2][2] * iz) + grid[3][2] * addTranslation) * w;
}








VolumeMesh::VolumeMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

VolumeMesh::~VolumeMesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}





glm::vec3 projectToInfinity(const glm::vec3 &vertex, const glm::vec3 &lightPosition, float infinity)
{
    glm::vec3 direction = glm::normalize(vertex - lightPosition);
    return vertex + direction * infinity;
}

bool areEqual(const glm::vec3 &v1, const glm::vec3 &v2, float epsilon = 1e-5f)
{
    return glm::all(glm::epsilonEqual(v1, v2, epsilon));
}

bool isFrontFacing(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& lightDirection) 
{
    // Calcula o vetor normal do triângulo
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));


    float dotProduct = glm::dot(normal, glm::normalize(lightDirection));

    return dotProduct > 0.0f;
}
void VolumeMesh::GenerateVolume(const glm::vec3 &lightPos, bool isDirectional)
{

    SShadowVolume *svp = 0;
   

	if (ShadowVolumes.size() > ShadowVolumesUsed)
	{
		// get the next unused buffer

		svp = &ShadowVolumes[ShadowVolumesUsed];
		svp->resize(0);

	
	}
	else
	{
        ShadowVolumes.push_back(SShadowVolume());
        svp = &ShadowVolumes.back();
	}

    svp->reserve(IndexCount * 5);
    ++ShadowVolumesUsed;

    // We use triangle lists
    Edges.reserve(IndexCount * 2);
    u32 numEdges = 0;

    numEdges = createEdgesAndCaps(lightPos, svp);

    // for all edges add the near->far quads
    for (u32 i = 0; i < numEdges; ++i)
    {

        const glm::vec3 &v1 = positions[Edges[2 * i + 0]];
        const glm::vec3 &v2 = positions[Edges[2 * i + 1]];
        const glm::vec3 v3 = projectToInfinity(v1, lightPos, Infinity);
        //(v1 + (v1 - light).normalize() * Infinity);
        const glm::vec3 v4 = projectToInfinity(v2, lightPos, Infinity);
        //(v2 + (v2 - light).normalize() * Infinity);

  


        

        // Add a quad (two triangles) to the vertex list

        svp->push_back(v1);
        svp->push_back(v2);
        svp->push_back(v3);

        svp->push_back(v2);
        svp->push_back(v4);
        svp->push_back(v3);
    }
  //  LogInfo("Created volume mesh with %d vertices and %d indices and %d edges", (int)positions.size(), (int)indices.size(), numEdges);
}



void VolumeMesh::calculateAdjacency()
{
    Adjacency.reserve(IndexCount);

    // go through all faces and fetch their three neighbours
    for (u32 f = 0; f < IndexCount; f += 3)
    {
        for (u32 edge = 0; edge < 3; ++edge)
        {
            const glm::vec3 &v1 = positions[indices[f + edge]];
            const glm::vec3 &v2 = positions[indices[f + ((edge + 1) % 3)]];

            // now we search an_O_ther _F_ace with these two
            // vertices, which is not the current face.
            u32 of;

            for (of = 0; of < IndexCount; of += 3)
            {
                // only other faces
                if (of != f)
                {
                    bool cnt1 = false;
                    bool cnt2 = false;

                    for (s32 e = 0; e < 3; ++e)
                    {
                        if (areEqual(v1 , (positions[indices[of + e]])))
                            cnt1 = true;

                        if (areEqual(v2 ,(positions[indices[of + e]])))
                            cnt2 = true;
                    }
                    // one match for each vertex, i.e. edge is the same
                    if (cnt1 && cnt2)
                        break;
                }
            }

            // no adjacent edges -> store face number, else store adjacent face
            if (of >= IndexCount)
                Adjacency[f + edge] = f / 3;
            else
                Adjacency[f + edge] = of / 3;
        }
    }
}



u32 VolumeMesh::createEdgesAndCaps(const glm::vec3 &light, SShadowVolume *svp)
{
  
    bool UseZFailMethod = true;
    u32 numEdges = 0;
    const u32 faceCount = IndexCount / 3;

    // Check every face if it is front or back facing the light.
    for (u32 i = 0; i < faceCount; ++i)
    {
        const glm::vec3 v0 = positions[indices[3 * i + 0]];
        const glm::vec3 v1 = positions[indices[3 * i + 1]];
        const glm::vec3 v2 = positions[indices[3 * i + 2]];

        FaceData[i] = isFrontFacing(v0, v1, v2, light);
      //  FaceData[i] = isFrontFacing(v2, v1, v0, light);

        if (UseZFailMethod && FaceData[i])
        {

            // add front cap from light-facing faces
            svp->push_back(v2);
            svp->push_back(v1);
            svp->push_back(v0);

            // add back cap
            const glm::vec3 i0 = v0 + projectToInfinity(v0, light, Infinity);
            //(v0 - light).normalize() * Infinity;
            const glm::vec3 i1 = v1 + projectToInfinity(v1, light, Infinity);
            //(v1 - light).normalize() *  Infinity;
            const glm::vec3 i2 = v2 + projectToInfinity(v2, light, Infinity);
            //(v2 - light).normalize() * Infinity;

            svp->push_back(i0);
            svp->push_back(i1);
            svp->push_back(i2);
        }
    }

    // Create edges
    for (u32 i = 0; i < faceCount; ++i)
    {
        // check all front facing faces
        if (FaceData[i] == true)
        {
            const u16 wFace0 = indices[3 * i + 0];
            const u16 wFace1 = indices[3 * i + 1];
            const u16 wFace2 = indices[3 * i + 2];

            const u16 adj0 = Adjacency[3 * i + 0];
            const u16 adj1 = Adjacency[3 * i + 1];
            const u16 adj2 = Adjacency[3 * i + 2];

            // add edges if face is adjacent to back-facing face
            // or if no adjacent face was found

            if (adj0 == i || FaceData[adj0] == false)
            {
                // add edge v0-v1
                Edges[2 * numEdges + 0] = wFace0;
                Edges[2 * numEdges + 1] = wFace1;
                ++numEdges;
            }


            if (adj1 == i || FaceData[adj1] == false)
            {
                // add edge v1-v2
                Edges[2 * numEdges + 0] = wFace1;
                Edges[2 * numEdges + 1] = wFace2;
                ++numEdges;
            }


            if (adj2 == i || FaceData[adj2] == false)
            {
                // add edge v2-v0
                Edges[2 * numEdges + 0] = wFace2;
                Edges[2 * numEdges + 1] = wFace0;
                ++numEdges;
            }
        }
    }
    return numEdges;
}
void VolumeMesh::UpdateVolume(const glm::vec3 &lightPos, bool isDirectional,const glm::mat4 &model,  Mesh *mesh )
{
   
    const u32 oldIndexCount = IndexCount;
	const u32 oldVertexCount = VertexCount;

	
	if (!mesh)
		return;


	// calculate total amount of vertices and indices

	VertexCount = 0;
	IndexCount = 0;
	ShadowVolumesUsed = 0;

	u32 i;
	u32 totalVertices = 0;
	u32 totalIndices = 0;
	
		//loop all surfaces
    totalIndices += mesh->GetIndexCount();
    totalVertices += mesh->GetVertexCount();


	// allocate memory if necessary



	positions.reserve(totalVertices);
	indices.reserve(totalIndices);
	FaceData.reserve(totalIndices / 3);


	// copy mesh

    const unsigned int* idxp = mesh->indices.data();
    const unsigned int* idxpend = idxp + mesh->GetIndexCount();
    for (; idxp!=idxpend; ++idxp)
        indices[IndexCount++] = *idxp + VertexCount;

    const u32 vtxcnt = mesh->GetVertexCount();
    for (u32 j=0; j<vtxcnt; ++j)
        positions[VertexCount++] = mesh->positions[j];


	// recalculate adjacency if necessary
	if (oldVertexCount != VertexCount || oldIndexCount != IndexCount)
		calculateAdjacency();


    glm::mat4 mat =glm::inverse(model);
    glm::vec4 light =  mat * glm::vec4(lightPos, 1.0f);
    GenerateVolume(glm::vec3(light.x, light.y, light.z), isDirectional);
    //GenerateVolume(lightPos, isDirectional);



}

void VolumeMesh::Render()
{
      	if (!ShadowVolumesUsed)
			return;
  

    for (int i=0;i<ShadowVolumesUsed;i++)
    {
            const u32 points = ShadowVolumes[i].size();
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, points * sizeof(glm::vec3), ShadowVolumes[i].data(), GL_STATIC_DRAW);
            
            
            glDrawArrays(GL_TRIANGLES, 0, points);

            glBindVertexArray(0);

    }
    

}


