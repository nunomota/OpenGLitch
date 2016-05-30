#pragma once

class CustomObject: public PhysicalObject {
    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        std::vector<GLuint> indices;

        GLuint texture_id;

    protected:
        void InitialCalculations() {
            setDrawMode(0);
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("custom_object_vshader.glsl",
                                                  "custom_object_fshader.glsl");
        }

        void SetupVertices() {
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        }

        void SetupUVs() {
            glGenBuffers(1, &vertex_buffer_object_uv_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_uv_);
            glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        }

        void setupNormals() {
            glGenBuffers(1, &vertex_buffer_object_normal_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_normal_);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
        }

        void SetupIndexBuffer() {
            num_indices_ = indices.size();

            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                         &indices[0], GL_STATIC_DRAW);
        }

        void SetupUniforms() {
            addTexture(texture_id);
        }

    public:
        CustomObject(const char* path, string texture) {
            if (Loader::loadObj(path, vertices, uvs, normals)) {
                GlmStrings glmStrings;
                for (int i = 0; i < vertices.size(); i++) {
                    indices.push_back(i);
                }
            } else {
                Reporter::println("Could not load OBJ", "CustomObj");
                //renderer.setState(false);
            }
            texture_id = Loader::loadTexture(texture);
        }
};