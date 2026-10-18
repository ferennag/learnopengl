#include "model.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_filesystem.h>
#include <memory>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static glm::mat4 ToGlmMatrix(const aiMatrix4x4 &m) {
    return glm::mat4(m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2, m.a3, m.b3, m.c3, m.d3, m.a4, m.b4, m.c4, m.d4);
}

std::vector<std::string> Model::GetAvailable() {
    int count;
    char **entries = SDL_GlobDirectory("resources/objects", nullptr, 0, &count);
    std::vector<std::string> results;
    for (int i = 0; i < count; ++i) {
        results.push_back(std::string(entries[i]));
    }
    SDL_free(entries);
    return results;
}

std::unique_ptr<Model> Model::Load(const std::string &file) {
    std::stringstream ss;
    ss << "resources/objects/" << file;
    auto filename = ss.str();

    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(filename.c_str(),
                          aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                              aiProcess_SortByPType | aiProcess_GenSmoothNormals);
    if (!scene) {
        SDL_Log("Failed to load object: %s", filename.c_str());
        return nullptr;
    }

    SDL_Log("Processing scene \"%s\" (%s)", scene->mName.C_Str(), filename.c_str());
    std::vector<aiNode *> nodes;
    std::vector<Mesh> meshes;
    nodes.push_back(scene->mRootNode);
    while (!nodes.empty()) {
        aiNode *current = nodes.back();
        nodes.pop_back();

        for (unsigned int i = 0; i < current->mNumChildren; ++i) {
            nodes.push_back(current->mChildren[i]);
        }

        glm::mat4 transformation = ToGlmMatrix(current->mTransformation);
        aiNode *node = current;
        while (node->mParent) {
            node = node->mParent;
            transformation = ToGlmMatrix(node->mTransformation) * transformation;
        }

        for (unsigned int i = 0; i < current->mNumMeshes; ++i) {
            Mesh meshData{};
            meshData.transform = transformation;

            aiMesh *mesh = scene->mMeshes[current->mMeshes[i]];
            meshData.vertices.reserve(mesh->mNumVertices);
            meshData.indices.reserve(mesh->mNumFaces * 3); // Assume triangles
            for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
                aiVector3D position = mesh->mVertices[j];
                aiVector3D normal = mesh->mNormals[j];

                Vertex vertex;
                vertex.position = glm::vec3(position.x, position.y, position.z);
                vertex.normal = glm::vec3(normal.x, normal.y, normal.z);
                meshData.vertices.push_back(vertex);
            }

            for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
                aiFace face = mesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; ++k) {
                    meshData.indices.push_back(face.mIndices[k]);
                }
            }

            meshes.push_back(std::move(meshData));
        }
    }
    SDL_Log("Processed scene \"%s\" (%s), number of meshes loaded: %lu",
            scene->mName.C_Str(),
            filename.c_str(),
            meshes.size());

    return std::unique_ptr<Model>(new Model(file, meshes));
}

Model::Model(const std::string &name, const std::vector<Mesh> &meshes) : mName(name), mMeshes(meshes) {
    glm::vec3 minBounds(FLT_MAX);
    glm::vec3 maxBounds(-FLT_MAX);

    for (auto &mesh : mMeshes) {
        glCreateVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);
        glCreateBuffers(1, &mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glNamedBufferStorage(mesh.vbo, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), 0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        glCreateBuffers(1, &mesh.ebo);
        glNamedBufferStorage(mesh.ebo, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);

        for (const auto &vertex : mesh.vertices) {
            minBounds = glm::min(minBounds, vertex.position);
            maxBounds = glm::max(maxBounds, vertex.position);
        }
    }

    SDL_Log("Min: %s, Max: %s", glm::to_string(minBounds).c_str(), glm::to_string(maxBounds).c_str());
    mMinBounds = minBounds;
    mMaxBounds = maxBounds;

    auto center = (mMinBounds + mMaxBounds) * 0.5f;
    mTransform = glm::translate(mTransform, -center);
}

Model::~Model() {
    for (auto &mesh : mMeshes) {
        glDeleteVertexArrays(1, &mesh.vao);
        glDeleteBuffers(1, &mesh.vbo);
        glDeleteBuffers(1, &mesh.ebo);
    }
}

void Model::Draw(std::shared_ptr<Shader> shader) const {
    for (const auto &mesh : mMeshes) {
        glm::mat4 transform = mTransform * mesh.transform;
        glm::mat3 normal = glm::transpose(glm::inverse(transform));
        shader->UniformMat4("Model", transform);
        shader->UniformMat3("Normal", normal);
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}

void Model::Rotate(float angle, const glm::vec3 &axis) {
    auto center = (mMinBounds + mMaxBounds) * 0.5f;
    mTransform = glm::translate(mTransform, center);
    mTransform = glm::rotate(mTransform, angle, axis);
    mTransform = glm::translate(mTransform, -center);
}

std::string Model::GetName() const {
    return mName;
}
