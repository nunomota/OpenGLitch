#pragma once

class CustomObject: public PhysicalObject {
    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        std::vector<GLuint> indices;

    protected:
        void InitialCalculations() {
            setDrawMode(0);
        }

        void LoadShaders() {
            // compile the shaders.
            program_id_ = icg_helper::LoadShaders("cube_vshader.glsl",
                                                  "cube_fshader.glsl");
        }

        void SetupVertices() {
            glGenBuffers(1, &vertex_buffer_object_position_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_position_);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

            // setup uvs as well
            glGenBuffers(1, &vertex_buffer_object_uv_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object_uv_);
            glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        }

        void SetupIndexBuffer() {
            num_indices_ = indices.size();

            glGenBuffers(1, &vertex_buffer_object_index_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffer_object_index_);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                         &indices[0], GL_STATIC_DRAW);
        }

        void SetupUniforms() {
            // TODO setup normals uniform
            // TODO setup uvs uniform
        }

    public:
        CustomObject(const char* path) {
            if (Loader::loadObj(path, vertices, uvs, normals)) {
                GlmStrings glmStrings;
                for (int i = 0; i < vertices.size(); i++) {
                    indices.push_back(i);
                }
            } else {
                Reporter::println("Could not load OBJ", "CustomObj");
                //renderer.setState(false);
            }
        }
};