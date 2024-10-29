#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>
#include "Config.hpp"
#include "glad/glad.h"
#include "Utils.hpp"



class Mesh
{
public:
    enum AttributeFlags
    {
        POSITION = 1 << 0,
        TEXCOORD = 1 << 1,
        NORMAL = 1 << 2,
        TANGENT = 1 << 3,
        BINORMAL = 1 << 4,
        FACES = 1 << 5
    };

    Mesh(int material = 0);
    ~Mesh();

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    int material;

    size_t AddVertex(const glm::vec3 &position);
    size_t AddVertex(float x, float y, float z);
    size_t AddVertex(const glm::vec3 &position, const glm::vec2 &texCoord);
    size_t AddVertex(float x, float y, float z, float u, float v);
    size_t AddVertex(const glm::vec3 &position, const glm::vec2 &texCoord, const glm::vec3 &normal);
    size_t AddVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz);

    void AddIndex(unsigned int index);
    void AddFace(int v1, int v2, int v3);

    void Clear();

    int TriangleVertex(int face, int vertex);
    float VertexX(int vertex);
    float VertexY(int vertex);
    float VertexZ(int vertex);

    int GetVertexCount() const { return (int)positions.size(); }
    int GetIndexCount() const { return (int)indices.size(); }
    int GetFacesCount() const { return GetIndexCount() / 3; }



    void Build();
    void Update();
    void Render();
    void Release();

    static Mesh CreatePlane(float width, float depth);
    static Mesh CreateCube();
    static Mesh CreateQuad(float width, float height);

private:
    GLuint VAO, VBO, EBO;
    GLuint positionVBO, texCoordVBO, normalVBO;

    bool isBuilt = false;
    int activeAttributes = 0;
};



class VolumeMesh
{
public:
    VolumeMesh();
    ~VolumeMesh();


    void GenerateVolume(const glm::vec3 &lightPos, bool isDirectional);
    void UpdateVolume(const glm::vec3 &lightPos, bool isDirectional, const glm::mat4 &model,  Mesh *baseMesh);

    void Render();


private:
   
    GLuint VAO, VBO;


    bool isBuilt = false;
    bool isGenerated = false;
    std::vector<glm::vec3> positions;
    std::vector<unsigned int> indices;


    	typedef std::vector<glm::vec3> SShadowVolume;


		u32 createEdgesAndCaps(const glm::vec3& light, SShadowVolume* svp);


		void calculateAdjacency();


		// a shadow volume for every light
		std::vector<SShadowVolume> ShadowVolumes;



		std::vector<u16> Adjacency;
		std::vector<u16> Edges;
		std::vector<bool> FaceData;

		

		u32 IndexCount=0;
		u32 VertexCount=0;
		u32 ShadowVolumesUsed=0;

		f32 Infinity=10.0f;

    
};