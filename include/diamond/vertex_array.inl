#include "opengl.hpp"
#include "vertex_array.hpp"
#include <map>

namespace NSM {

    void vertex_array_attribute::attrib_format(GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset) {
        glVertexArrayAttribFormat(vao(), thisref, size, type, normalized, relativeoffset);
    }

    void vertex_array_attribute::attrib_format_int(GLint size, GLenum type, GLuint relativeoffset) {
        glVertexArrayAttribIFormat(vao(), thisref, size, type, relativeoffset);
    }

    void vertex_array_attribute::attrib_format_long(GLint size, GLenum type, GLuint relativeoffset) {
        glVertexArrayAttribLFormat(vao(), thisref, size, type, relativeoffset);
    }

    void vertex_array_attribute::binding(GLuint binding) { // low level function
        glVertexArrayAttribBinding(vao(), thisref, binding);
    }



    template<class... T>
    void vertex_array_binding<T...>::vertex_buffer(buffer& buf, GLintptr offset) {
        GLsizei * strides = get_stride_wrap<T...>();
        glVertexArrayVertexBuffer(vao(), thisref, buf, offset, strides[0]);
    }

    template<class... T>
    void vertex_array_binding<T...>::vertex_buffer(std::vector<buffer>& bufs, const GLintptr * offsets) {
        constexpr size_t N = sizeof...(T);
        GLsizei * strides = get_stride_wrap<T...>();
        bool hadOffsets = !!offsets;
        if (!hadOffsets) offsets = new GLintptr[N]{ 0 };
        GLuint * bufsp = new GLuint[N];
        const size_t Nv = std::min(N, bufs.size());
        for (int i = 0; i < Nv; i++) bufsp[i] = bufs[i];
        glVertexArrayVertexBuffers(vao(), thisref, Nv, bufsp, offsets, strides);
        if (!hadOffsets) delete offsets;
        delete bufsp;
    }

    template<class... T>
    void vertex_array_binding<T...>::vertex_buffer(buffer * bufs, const GLintptr * offsets) {
        constexpr size_t N = sizeof...(T);
        GLsizei * strides = get_stride_wrap<T...>();
        bool hadOffsets = !!offsets;
        if (!hadOffsets) offsets = new GLintptr[N]{ 0 };
        glVertexArrayVertexBuffers(vao(), thisref, N, bufs, offsets, strides);
        if (!hadOffsets) delete offsets;
    }

};