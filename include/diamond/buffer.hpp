#include "opengl.hpp"
#include <map>
#include <vector>

namespace NSM {

    class buffer;

    class buffer_builder {
    public:
        static std::map<GLuint, size_t> counters;

        static void account(buffer& key){
            if (!counters[key]) counters[key] = 0;
            counters[key]++;
        }

        static void discount(buffer& key){
            if (counters[key]) counters[key]--;
        }

        static void create(GLuint &obj){
            glCreateBuffers(1, &obj);
        }

        static void release(GLuint &obj){
            counters.erase(obj);
            glDeleteBuffers(1, &obj);
        }

        static bool single_of(buffer& key){
            return !counters[key] || counters[key] <= 1;
        }
    };

    class buffer : public gl_object<buffer_builder> {
    protected:
        using base = gl_object<buffer_builder>;
        
    public:

        // constructor (variadic)
        buffer() { base::create_alloc(); }
        buffer(buffer& another) { base::move(another); } // copy (it refs)
        buffer(buffer&& another) { base::move(std::forward<buffer>(another)); } // move
        buffer(GLuint * another) { base::move(another); } // heap by ptr

        void get_subdata(GLintptr offset, GLsizei size, void *data) const {
            glGetNamedBufferSubData(thisref, offset, size, data);
        }

        void data(GLsizei size, const void *data, GLenum usage = GL_STATIC_DRAW){
            glNamedBufferData(thisref, size, data, usage);
        }

        void subdata(GLintptr offset, GLsizei size, const void *data){
            glNamedBufferSubData(thisref, offset, size, data);
        }

        void storage(GLsizei size, const void *data = nullptr, GLenum flags = GL_DYNAMIC_STORAGE_BIT) {
            glNamedBufferStorage(thisref, size, data, flags);
        }

        void copydata(buffer& dest, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size){
            glCopyNamedBufferSubData(thisref, dest, readOffset, writeOffset, size);
        }




        // data from vector
        template<class T>
        void data(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
            buffer::data(data.size() * sizeof(T), data.data(), usage);
        }

        // fill subdata by vector
        template<class T>
        void subdata(GLintptr offset, const std::vector<T>& data) {
            buffer::subdata(offset, data.size() * sizeof(T), data.data());
        }

        // get subdata by range
        template<class T>
        std::vector<T> get_subdata(GLintptr offset, GLsizei size) const {
            std::vector<T> vctr(size);
            buffer::get_subdata(offset, vctr.size() * sizeof(T), vctr.data());
            return vctr;
        }

        // get subdata by full vector
        template<class T>
        std::vector<T>& get_subdata(GLintptr offset, std::vector<T>&vctr) const {
            buffer::get_subdata(offset, vctr.size() * sizeof(T), vctr.data());
            return vctr;
        }

    };

};