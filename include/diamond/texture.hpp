#include "opengl.hpp"
#include <map>
#include "buffer.hpp"

namespace NSM {

    class texture;

    class texture_builder {
    public:
        static std::map<GLuint, size_t> counters;
        static std::map<GLuint, GLuint> dictionary; // optional dictionary

        static void account(texture& key){
            if (!counters[key]) counters[key] = 1;
            counters[key]++;
        }

        static void discount(texture& key){
            if (counters[key]) counters[key]--;
        }

        static void create(GLuint &obj, texture& tex, GLuint& target){
            glCreateTextures(target, 1, &obj);
            dictionary[tex] = target; // link
        }

        static void release(GLuint &obj, texture& tex){
            glDeleteTextures(1, &obj);
            dictionary.erase(tex);
            vpair key = {dictionary[tex], tex};
            if (counters[key]) counters.erase(key);
        }

        static bool single_of(texture& tex){
            vpair key = {dictionary[tex], tex};
            return !counters[key] || counters[key] <= 1;
        }
    };

    class texture: public gl_object<texture_builder> {
    protected:
        using base = gl_object<texture_builder>;

    public:
        texture(){}
        texture(GLuint target){
            create_alloc(thisref, target);
        }

        texture(texture& another) { base::move(another); } // copy (it refs)
        texture(texture&& another) { base::move(std::forward<texture>(another)); } // move

        ~texture(){
            if (texture_builder::single_of(thisref)) this->release(thisref);
            texture_builder::discount(thisref);
        }

        GLuint target(){
            return texture_builder::dictionary[thisref];
        }

        static std::vector<texture> create(GLuint gltarget, size_t n = 1){
            std::vector<texture> textures(n);
            glCreateTextures(gltarget, n, (GLuint *)textures.data());
            return textures;
        };


        template<class T>
        void parameter_val(GLenum pname, T param) const {
            this->parameter<T>(pname, &param);
        }

        template<class T>
        void parameter_int_val(GLenum pname, T param) const {
            this->parameter_int<T>(pname, &param);
        }

        template<class T>
        T get_parameter_val(GLenum pname, T * params = nullptr) const {
            return *(this->get_parameter(pname, params));
        }

        template<class T>
        T get_parameter_int_val(GLenum pname, T * params = nullptr) const {
            return *(this->get_parameter_int(pname, params));
        }



        template<class T>
        void parameter(GLenum pname, T * params) const {
            if constexpr (std::is_same<T, int>::value) glTextureParameteriv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, float>::value) glTextureParameterfv(thisref, pname, (float*)params);
        }

        template<class T>
        void parameter_int(GLenum pname, T * params) const {
            if constexpr (std::is_same<T, int>::value) glTextureParameterIiv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, GLuint>::value) glTextureParameterIuiv(thisref, pname, (GLuint*)params);
        }

        template<class T>
        T * get_parameter(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if constexpr (std::is_same<T, int>::value) glGetTextureParameteriv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, float>::value) glGetTextureParameterfv(thisref, pname, (float*)params);
            return params;
        }

        template<class T>
        T * get_parameter_int(GLenum pname, T * params = nullptr) const {
            if (!params) params = { 0 };
            if constexpr (std::is_same<T, int>::value) glGetTextureParameterIiv(thisref, pname, (int*)params);
            if constexpr (std::is_same<T, GLuint>::value) glGetTextureParameterIuiv(thisref, pname, (GLuint*)params);
            return params;
        }



        // texture storage (accept GLM vector)
        void storage(GLsizei levels, const GLenum& internalformat, GLsizei size) {
            glTextureStorage1D(thisref, levels, internalformat, size);
        }

        void storage(GLsizei levels, const GLenum& internalformat, glm::uvec2 size) {
            glTextureStorage2D(thisref, levels, internalformat, size.x, size.y);
        }

        void storage(GLsizei levels, const GLenum& internalformat, glm::uvec3 size) {
            glTextureStorage3D(thisref, levels, internalformat, size.x, size.y, size.z);
        }


        // subimage (accept GLM vector)
        void subimage(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage3D(thisref, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, pixels);
        }

        void subimage(GLint level, glm::ivec2 offset, glm::uvec2 size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage2D(thisref, level, offset.x, offset.y, size.x, size.y, format, type, pixels);
        }

        void subimage(GLint level, GLint offset, GLsizei size, GLenum format, GLenum type, const GLvoid * pixels) {
            glTextureSubImage1D(thisref, level, offset, size, format, type, pixels);
        }


        // simplified version of very hard function
        void copy_image_subdata(GLint srcLevel, glm::ivec3 srcOffset, texture& destination, GLint dstLevel, glm::ivec3 dstOffset, glm::uvec3 size) const;

        // texture of buffer 
        void buffer(const GLenum& internalformat, buffer& buf){
            glTextureBuffer(thisref, internalformat, buf);
        }


        // generate mipmap
        void generate_mipmap() {
            glGenerateTextureMipmap(thisref);
        }



        void get_image_subdata(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize, void *pixels) const {
            glGetTextureSubImage(thisref, level, offset.x, offset.y, offset.z, size.x, size.y, size.z, format, type, buffersize, pixels);
        }


        // get subimage to vector
        template<class T>
        void get_image_subdata(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, std::vector<T>& buffer) const {
            this->get_image_subdata(level, offset, size, format, type, buffer.size() * sizeof(T), buffer.data());
        }

        // get subimage as vector
        template<class T>
        std::vector<T> get_image_subdata(GLint level, glm::ivec3 offset, glm::uvec3 size, GLenum format, GLenum type, GLenum buffersize) const {
            std::vector<T> buffer(buffersize);
            this->get_image_subdata(level, offset, size, format, type, buffer.size() * sizeof(T), buffer.data());
            return buffer;
        }

    };


};