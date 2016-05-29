#pragma once

class CustomObject: public PhysicalObject {
    private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

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
        }

    public:
        CustomObject(const char* path) {
            if (!Loader::loadObj(path, vertices, uvs, normals)) renderer.setState(false);
        }
};