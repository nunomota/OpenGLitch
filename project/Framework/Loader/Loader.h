#pragma once
#include "icg_helper.h"
#include "glm/gtc/type_ptr.hpp"

class Loader {
    public:
        static GLuint loadTexture(string texture_name) {
            int width;
            int height;
            int nb_component;
            GLuint texture_id;

            // set stb_image to have the same coordinates as OpenGL
            stbi_set_flip_vertically_on_load(1);
            unsigned char* image = stbi_load(texture_name.c_str(), &width,
                                             &height, &nb_component, 0);

            if(image == nullptr) {
                throw(string("Failed to load texture"));
            }

            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glEnable(GL_BLEND);

            if(nb_component == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                             GL_RGB, GL_UNSIGNED_BYTE, image);
            } else if(nb_component == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, image);
            }

            // cleanup
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(image);

            return texture_id;
        }

        static bool loadObj(const char * path,
                            std::vector < glm::vec3 > & out_vertices,
                            std::vector < glm::vec2 > & out_uvs,
                            std::vector < glm::vec3 > & out_normals) {
            std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
            std::vector< glm::vec3 > temp_vertices;
            std::vector< glm::vec2 > temp_uvs;
            std::vector< glm::vec3 > temp_normals;
            
            FILE * file = fopen(path, "r");
            if( file == NULL ){
                Reporter::println("[IO ERROR]", "Impossible to open the file !");
                return false;
            }
            
            while( 1 ){

                char lineHeader[128];
                // read the first word of the line
                int res = fscanf(file, "%s", lineHeader);
                if (res == EOF)
                    break; // EOF = End Of File. Quit the loop.

                // else : parse lineHeader
                if ( strcmp( lineHeader, "v" ) == 0 ){
                    glm::vec3 vertex;
                    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                    temp_vertices.push_back(vertex);
                }else if ( strcmp( lineHeader, "vt" ) == 0 ){
                    glm::vec2 uv;
                    fscanf(file, "%f %f\n", &uv.x, &uv.y );
                    temp_uvs.push_back(uv);
                }else if ( strcmp( lineHeader, "vn" ) == 0 ){
                    glm::vec3 normal;
                    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                    temp_normals.push_back(normal);
                }else if ( strcmp( lineHeader, "f" ) == 0 ){
                    std::string vertex1, vertex2, vertex3;
                    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                    int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                    if (matches != 9){
                        Reporter::println("[IO ERROR]", "File can't be read by our simple parser :( Try exporting with other options");
                        return false;
                    }
                    vertexIndices.push_back(vertexIndex[0]);
                    vertexIndices.push_back(vertexIndex[1]);
                    vertexIndices.push_back(vertexIndex[2]);
                    uvIndices    .push_back(uvIndex[0]);
                    uvIndices    .push_back(uvIndex[1]);
                    uvIndices    .push_back(uvIndex[2]);
                    normalIndices.push_back(normalIndex[0]);
                    normalIndices.push_back(normalIndex[1]);
                    normalIndices.push_back(normalIndex[2]);
                }

                for(unsigned int i = 0; i < vertexIndices.size(); i++) {
                    unsigned int vertexIndex = vertexIndices[i];
                    unsigned int uvIndex     = uvIndices[i];
                    unsigned int normalIndex = normalIndices[i];
                    
                    glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
                    glm::vec2 uv     = temp_uvs[ uvIndex-1 ];
                    glm::vec3 normal = temp_normals[ normalIndex-1 ];
                    
                    out_vertices.push_back(vertex);
                    out_uvs     .push_back(uv);
                    out_normals .push_back(normal);
                }
            }
        }
};