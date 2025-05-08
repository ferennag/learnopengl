#include "Model.h"
#include "Mesh.h"
#include <SDL3/SDL_log.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <memory>

Model::Model(Mesh *meshes, unsigned int numMeshes) : meshes(meshes), numMeshes(numMeshes) {
    transform = glm::identity<glm::mat4>();
}

Model::~Model() {}

std::shared_ptr<Model> Model::load(const std::string &path) {
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);

    if (scene == nullptr) {
        SDL_Log("Failed to load model: %s", path.c_str());
        return nullptr;
    }

    Mesh *meshes = new Mesh[scene->mNumMeshes];
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[i];
        Vertex *vertices = new Vertex[mesh->mNumVertices];
        Face *faces = new Face[mesh->mNumFaces];

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            aiVector3D vertex = mesh->mVertices[j];
            vertices[j].position[0] = vertex.x;
            vertices[j].position[1] = vertex.y;
            vertices[j].position[2] = vertex.z;

            aiVector3D normal = mesh->mNormals[j];
            vertices[j].normal[0] = normal.x;
            vertices[j].normal[1] = normal.y;
            vertices[j].normal[2] = normal.z;
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            aiFace face = mesh->mFaces[j];
            faces[j].indices[0] = face.mIndices[0];
            faces[j].indices[1] = face.mIndices[1];
            faces[j].indices[2] = face.mIndices[2];
        }

        meshes[i].initialize(vertices, mesh->mNumVertices, faces, mesh->mNumFaces);
    }

    return std::shared_ptr<Model>(new Model(meshes, scene->mNumMeshes));
}

void Model::update() { transform = glm::rotate(transform, glm::radians(0.1f), glm::vec3(0.0f, 1.0f, 0.0f)); }

void Model::render() const {
    for (unsigned int i = 0; i < numMeshes; ++i) {
        meshes[i].render();
    }
}
