#include "opengl.hpp"
#include <map>
#include "buffer.hpp"

namespace NSM {

    class vertex_array_attribute;
    class vertex_array;
    class vertex_array_binding_base;

    class vertex_array_attribute_builder {
    public:
        static std::map<vpair, size_t> counters;
        static std::map<vertex_array_attribute*, GLuint> dictionary;

        static void account(vertex_array_attribute& obj){
            vpair key = {dictionary[&obj], obj};
            if (!counters[key]) counters[key] = 1;
            counters[key]++;
        }

        static void discount(vertex_array_attribute& obj){
            if (dictionary[&obj]) {
                vpair key = {dictionary[&obj], obj};
                if (counters[key]) counters[key]--;
            }
        }

        static void create(GLuint &obj, vertex_array_attribute& vat, vertex_array& vao){
            glEnableVertexArrayAttrib(vao, obj);
            dictionary[&vat] = vao; // link
        }

        static void release(GLuint &obj, vertex_array_attribute& vat){
            glDisableVertexArrayAttrib(dictionary[&vat], obj);
            dictionary.erase(&vat);
            vpair key = {dictionary[&vat], vat};
            if (counters[key]) counters.erase(key);
        }

        static bool single_of(vertex_array_attribute& vat){
            vpair key = {dictionary[&vat], vat};
            return !counters[key] || counters[key] <= 1;
        }
    };

    class vertex_array_binding_builder {
    public:
        static std::map<vpair, size_t> counters;
        static std::map<vertex_array_binding_base*, GLuint> dictionary;

        static void account(vertex_array_binding_base& obj){
            vpair key = {dictionary[&obj], obj};
            if (!counters[key]) counters[key] = 1;
            counters[key]++;
        }

        static void discount(vertex_array_binding_base& obj){
            if (dictionary[&obj]) {
                vpair key = {dictionary[&obj], obj};
                if (counters[key]) counters[key]--;
            }
        }

        static void create(GLuint &obj, vertex_array_binding_base& vat, vertex_array& vao){
            dictionary[&vat] = vao; // link
        }

        static void release(GLuint &obj, vertex_array_binding_base& vat){
            dictionary.erase(&vat);
            vpair key = {dictionary[&vat], vat};
            if (counters[key]) counters.erase(key);
        }

        static bool single_of(vertex_array_binding_base& vat){
            vpair key = {dictionary[&vat], vat};
            return !counters[key] || counters[key] <= 1;
        }
    };

    class vertex_array_builder {
    public:
        static std::map<GLuint, size_t> counters;
        
        static void account(vertex_array& key){
            if (!counters[key]) counters[key] = 0;
            counters[key]++;
        }

        static void discount(vertex_array& key){
            if (counters[key]) counters[key]--;
        }

        static void create(GLuint &obj){
            glCreateVertexArrays(1, &obj);
        }

        static void release(GLuint &obj){
            counters.erase(obj);
            glDeleteVertexArrays(1, &obj);
        }

        static bool single_of(vertex_array& key){
            return !counters[key] || counters[key] <= 1;
        }
    };



    class vertex_array_attribute : public gl_object<vertex_array_attribute_builder> {
    protected:
        using base = gl_object<vertex_array_attribute_builder>;
        friend vertex_array;

    public:
        vertex_array_attribute(vertex_array& vao, GLuint obj = 0){
            globj = obj;
            create_alloc(thisref, vao);
        }

        ~vertex_array_attribute(){
            if (vertex_array_attribute_builder::single_of(thisref)) this->release(thisref);
            vertex_array_attribute_builder::discount(thisref);
        }

        GLuint& vao() {
            return vertex_array_attribute_builder::dictionary[this];
        }

        // constructor
        vertex_array_attribute(vertex_array_attribute& another) { base::move(another); } // copy (it refs)
        vertex_array_attribute(vertex_array_attribute&& another) { base::move(std::forward<vertex_array_attribute>(another)); } // move
        vertex_array_attribute(GLuint * another) { base::move(another); } // heap by ptr


        void attrib_format(GLint size, GLenum type, GLboolean normalized = false, GLuint relativeoffset = 0);
        void attrib_format_int(GLint size, GLenum type, GLuint relativeoffset = 0);
        void attrib_format_long(GLint size, GLenum type, GLuint relativeoffset = 0);
        void binding(GLuint binding);

        template<class... T>
        void binding(vertex_array_binding<T...>& bnd) {this->binding((GLuint)bnd);}
    };

    // only container
    class vertex_array_binding_base : public gl_object<vertex_array_binding_builder> {
    protected:
        using base = gl_object<vertex_array_binding_builder>;
        friend vertex_array;

    public:
        vertex_array_binding_base(vertex_array& vao, GLuint obj = 0){
            globj = obj;
            base::create_alloc(thisref, vao);
        }

        ~vertex_array_binding_base(){
            if (vertex_array_binding_builder::single_of(thisref)) this->release(thisref);
            vertex_array_binding_builder::discount(thisref);
        }

        GLuint& vao() {
            return vertex_array_binding_builder::dictionary[this];
        }
    };

    // typed derrivative
    template<class ...T>
    class vertex_array_binding : public vertex_array_binding_base {
    public:
        vertex_array_binding(vertex_array& vao, GLuint obj = 0): vertex_array_binding_base(vao, obj) {};
        void vertex_buffer(buffer& buf, GLintptr offset = 0);
        void vertex_buffer(std::vector<buffer>& buf, const GLintptr * offsets = 0);
        void vertex_buffer(buffer*bufs, const GLintptr * offsets = 0);
    };

    class vertex_array : public gl_object<vertex_array_builder> {
    protected:
        using base = gl_object<vertex_array_builder>;
    public:
        vertex_array() { base::create_alloc(); }
        vertex_array(vertex_array& another) { base::move(another); } // copy (it refs)
        vertex_array(vertex_array&& another) { base::move(std::forward<vertex_array>(another)); } // move
    };


};