#include "GL/glew.h"
#include "glm/glm.hpp"

#define thisref (*this)
#define NSM dmd

using vpair = std::pair<GLuint, GLuint>;

template<class GL_OBJ>
class gl_object {
protected:
    //GLuint globj = -1;
    template<class GL_OBJ>
    friend class gl_object;

    // shared GL object (don't delete without needless)
    GLuint globj = -1;

    // create object by pointer
    template<class ...ARG>
    void create_alloc(ARG&&... args){
        GL_OBJ::create(globj, std::forward<ARG>(args)...);
    }

    // create from program, shader, other not allocatable
    template<class ...ARG>
    void create_heap(ARG&&... args) {
        globj = GL_OBJ::create(std::forward<ARG>(args)...);
    }

    template<class ...ARG>
    void release(ARG&&... args) {
        GL_OBJ::release(globj, std::forward<ARG>(args)...);
    }

public:

    // move GL object
    template<class ANOTHER>
    void move(ANOTHER&& obj) { globj = obj; }

    // reference by GL object
    template<class ANOTHER>
    void move(ANOTHER& obj) { globj = obj; GL_OBJ::account(obj); }

    void move(GLuint& obj) { globj = obj; }
    void move(GLuint&& obj) { globj = obj; }
    void move(GLuint * obj) { globj = *obj; }

    // default destructor (not working with multi-argumented)
    ~gl_object() {
        if (GL_OBJ::single_of(thisref)) this->release();
        GL_OBJ::discount(thisref);
    }

    operator const GLuint&() const { return globj; }
    operator GLuint&() { return globj; }
};

// some hack
template <class... T>
struct restor {};

// getting strides of buffer types
template <class Tm>
constexpr decltype(auto) get_stride_impl(GLsizei * ptr, restor<Tm>) {
    *ptr = (GLsizei)sizeof(Tm);
}

template <class Tm, class... T>
constexpr decltype(auto) get_stride_impl(GLsizei * ptr, restor<Tm, T...> rst) {
    *ptr = (GLsizei)sizeof(Tm);
    get_stride_impl<T...>(ptr + 1, restor<T...>{});
}

template <class... T>
constexpr decltype(auto) get_stride(GLsizei * ptr) {
    return get_stride_impl<T...>(ptr, restor<T...>{});
}

template <class... T>
constexpr GLsizei * get_stride_wrap() {
    GLsizei * ptr = new GLsizei[sizeof...(T)];
    get_stride<T...>(ptr);
    return ptr;
}