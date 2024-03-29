#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class Texture
{
    public:
        static GLuint loadTexture(std::string img_path);
        static GLuint loadNRNTexture(std::string img_path);
        static GLuint loadFromMemory(int width, int height, unsigned char* data);
        static GLuint loadFromMemory(unsigned int texture_handle, int width, int height, unsigned char* data);
        static void deleteTexture(GLuint texture_handle);
        
    //private:
        Texture();
};
