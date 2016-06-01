#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"
#include "../EmptyObject/Light/DirectionalLight/DirectionalLight.h"

class PhysicalObject: public Object3D {
    private:
        GLenum draw_mode_ = GL_TRIANGLE_STRIP;    // used after as the 1st parameter of glDrawElements
        GLuint pass_id_;
        GLuint depthMVP_id_;
        GLuint depthBiasMVP_id_;
        GLuint tex_id_;
        GLuint shadow_tex_id_;

        // matrix to translate from depth to texture coords 
        glm::mat4 biasMatrix = glm::mat4(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );

        int count = 0; // TODO REMOVE!!

    protected:
        virtual void InitialCalculations() {};    // Called once, before any OpenGL operations take place
        virtual void LoadShaders() {};            // Called once, to compile the object's shaders
        virtual void SetupVertices() {};          // Called once, to setup the mesh's vertices
        virtual void SetupIndexBuffer() {};       // Called once, to setup the vertices' indices
        virtual void SetupUniforms() {};          // Called once, to setup new uniforms for the shader
        virtual void UpdateUniforms() {};         // Called every Draw call, to update the uniforms' values
        virtual void FinalOperations() {};        // Called once, when the object is being cleaned up and destroyed

        virtual void DebugMVP(/*glm::mat4 MVP, glm::vec3 position, */int PASS){}; // TODO REMOVE

        /** Called to set the drawing mode to either:
          * 0     - GL_TRIANGLES
          * 1     - GL_TRIANGLE_STRIP
          * Other - GL_TRIANGLE_STRIP
          */
        void setDrawMode(int new_mode) {
            if (new_mode == 0) {
                draw_mode_ = GL_TRIANGLES;
            } else if (new_mode == 1) {
                draw_mode_ = GL_TRIANGLE_STRIP;
            } else {
                Reporter::println("Invalid draw mode provided. Reverting to default (GL_TRIANGLE_STRIP)");
                draw_mode_ = GL_TRIANGLE_STRIP;
            }
        }

        /** This method can be called from any of
          * the virtual methods defined above except 
          * 'InitialCalculations'. 
          */
        void addTexture(GLuint texture_id) {
            std::ostringstream tex_name;
            if(texture_id == 4){
                printf("size: %lu\n", texture_ids_.size());   
            }
            tex_name << "tex" << texture_ids_.size();

            tex_id_ = glGetUniformLocation(program_id_, tex_name.str().c_str());

            glUniform1i(tex_id_, texture_ids_.size() + 1);

            texture_ids_.push_back(texture_id);
        }


    public:
        void Init(GLuint shadow_texture_id = 0) {
            InitialCalculations();

            // call to the sub-class' method to get the shaders' name
            LoadShaders();

            if(!program_id_) {
                exit(EXIT_FAILURE);
            }

            glUseProgram(program_id_);

            // vertex one vertex array
            glGenVertexArrays(1, &vertex_array_id_);
            glBindVertexArray(vertex_array_id_);

            // call to sub-class' method to setup the vertices correctly
            SetupVertices();

            // position shader attribute
            // fetch attribute ID for vertex positions
            GLuint loc_position = glGetAttribLocation(program_id_, "position");
            glEnableVertexAttribArray(loc_position); // Enable it
            glVertexAttribPointer(loc_position, 3, GL_FLOAT, DONT_NORMALIZE,
                                  ZERO_STRIDE, ZERO_BUFFER_OFFSET);

            // call to sub-class' method to setup the index buffer correctly
            SetupIndexBuffer();

            // other uniforms
            MVP_id_ = glGetUniformLocation(program_id_, "MVP");
            renderer.getMaterial()->setUniforms(program_id_);

            pass_id_ = glGetUniformLocation(program_id_, "PASS");
            depthMVP_id_ = glGetUniformLocation(program_id_, "depthMVP");
            depthBiasMVP_id_ = glGetUniformLocation(program_id_, "depthBiasMVP");

            SetupUniforms();

            printf("Physical object tex id: %d\n", shadow_texture_id);
            addTexture(shadow_texture_id);
            DebugMVP(shadow_texture_id);

            // to avoid the current object being polluted
            glBindVertexArray(0);
            glUseProgram(0);

            is_initialized_ = true;
        }

        void Draw(const glm::mat4 &view = IDENTITY_MATRIX,
                  const glm::mat4 &projection = IDENTITY_MATRIX, const glm::mat4 &depthMVP = IDENTITY_MATRIX, int pass = 0, GLuint shadow_texture_id = 0) {
            if (is_initialized_) {
                glUseProgram(program_id_);
                glBindVertexArray(vertex_array_id_);

                // bind textures
                int counter = 1;
                for(std::vector<GLuint>::iterator it = texture_ids_.begin(); it != texture_ids_.end(); ++it) {
                    GLuint cur_id = *it;
                    glActiveTexture(GL_TEXTURE0 + counter);
                    glBindTexture(GL_TEXTURE_2D, cur_id);
                    counter++;
                }

                // setup MVP
                glm::mat4 model = transform.getModelMatrix();
                glm::mat4 MVP = projection*view*model;
                glUniformMatrix4fv(MVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(MVP));

                //glm::mat4 depthMVP = directional_light->getDepthMVP();

                glm::mat4 newDepthMVP = IDENTITY_MATRIX;             

                newDepthMVP = glm::translate(depthMVP,glm::vec3(0.02f,0.0f,0.0f));

                glUniformMatrix4fv(depthMVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(depthMVP));

                //GlmStrings astring;
                //cout << astring.create(depthMVP) << "\n" << endl;

                glm::mat4 depthBiasMVP = biasMatrix*depthMVP;
                glUniformMatrix4fv(depthBiasMVP_id_, ONE, DONT_TRANSPOSE, glm::value_ptr(depthBiasMVP));

                //GlmStrings astring;
                //cout << astring.create(depthBiasMVP) << "\n" << endl;

                glm::vec3 pos = getTransform()->getPosition();
                glm::vec4 new_pos = glm::vec4(pos.x,pos.y,pos.z,1.0f);

                glm::vec4 result = MVP * new_pos;
                result = depthBiasMVP * result;

                //GlmStrings astring;
                //cout << astring.create(glm::vec3(result.x,result.y,1.0)) << "\n" << endl;
                //cout << "pos: " << astring.create(glm::vec3(new_pos.x,new_pos.y,1.0)) << "\n" << endl;
                //DebugMVP(depthMVP, pos, );

                //printf("PASS: %d\n", pass);
                DebugMVP(pass);
                glUniform1i(pass_id_, pass);

                UpdateUniforms();

                // draw
                glDrawElements(draw_mode_, num_indices_, GL_UNSIGNED_INT, 0);

                glBindVertexArray(0);
                glUseProgram(0);
            } else {
                reporter_.println("Cannot draw uninitialized Object3D!", "Warning");
            }
        }

        void Cleanup() {
            glBindVertexArray(0);
            glUseProgram(0);
            glDeleteBuffers(1, &vertex_buffer_object_position_);
            glDeleteBuffers(1, &vertex_buffer_object_index_);
            glDeleteProgram(program_id_);
            glDeleteVertexArrays(1, &vertex_array_id_);
            // TODO turn boolean value into boolean vector
            for (std::vector<GLuint>::iterator it = texture_ids_.begin(); it != texture_ids_.end(); ++it) {
                GLuint cur_id = *it;
                glDeleteTextures(1, &cur_id);
            }
            FinalOperations();
        }
};