#pragma once

#ifdef GLBINDING_SUPPORT

#else
#include "GL/glew.h"
#endif

#include "glm/glm.hpp"
#include <memory>

#define thisref (*this)
#define NS_NAME dgl

namespace NS_NAME {


    template<class GL_OBJ>
    class gl_object {
    protected:
        //GLuint globj = -1;
        template<class GL_OBJ>
        friend class gl_object;

        // shared GL object (don't delete without needless)
        std::shared_ptr<GLuint> globj;

        // create object by pointer
        template<class ...ARG>
        void create_alloc(ARG&&... args){
            //GL_OBJ::create(&globj, std::forward<ARG>(args)...);
            globj = std::make_shared<GLuint>(-1);
            GL_OBJ::create(globj.get(), std::forward<ARG>(args)...);
        }

        // create from program, shader, other not allocatable
        template<class ...ARG>
        void create_heap(ARG&&... args) {
            //globj = GL_OBJ::create(std::forward<ARG>(args)...);
            globj = std::make_shared<GLuint>(GL_OBJ::create(std::forward<ARG>(args)...));
        }

    public:

        // move GL object
        template<class ANOTHER>
        void move(ANOTHER&& obj) {
            globj = std::move(obj.globj);
        }

        // reference by GL object
        template<class ANOTHER>
        void move(ANOTHER& obj) {
            globj = obj.globj;
        }

        // construct from heap pointer
        void move(GLuint* obj) {
            globj = std::make_shared<GLuint>(*obj); // copy allocated pointer, because can be deleted by another GL object
        }


        // destructor
        ~gl_object(){
            if (globj && globj.use_count() <= 1) GL_OBJ::release(globj.get());
        }

        operator const GLuint&() const { return (*globj); }
    };


    /*
    class base {
    protected:
        std::shared_ptr<GLuint> globj;
        void set_object(std::shared_ptr<GLuint>& obj) { globj = obj; } // share pointer
        void set_object(std::shared_ptr<GLuint>&& obj) { globj = obj; } // share pointer
        void set_object(GLuint&& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLuint& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLuint* obj) { globj = std::shared_ptr<GLuint>(obj); }
        void set_object(GLint&& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLint& obj) { globj = std::make_shared<GLuint>(obj); }
        void set_object(GLint* obj) { globj = std::shared_ptr<GLuint>((GLuint *)obj); }
        operator GLuint*() { return globj.get(); }

        std::shared_ptr<GLuint>& _get_shared() {
            return globj;
        }
    public:
        base() { }
        ~base() { }
        operator const GLuint&() const { return (*globj); }
        void make_ptr() {
            globj = std::make_shared<GLuint>(-1);
        }
        bool ready_free() {
            return globj && globj.use_count() <= 1; // count references before remove
        }
    };
*/

    // some hack
	template<class C, std::size_t N, class... Args>
	struct restor;

	// getting strides of buffer types

	template<class C, std::size_t N, class T>
	struct restor<C, N, T>	{
		constexpr restor() { type_size_buffer[N - 1] = sizeof(T); }
		C type_size_buffer[N];
	};

	template<class C, std::size_t N, class T, class ... Args>
	struct restor<C, N, T, Args...> : restor<C, N, Args...>	{
		constexpr restor() { type_size_buffer[N - sizeof...(Args)-1] = sizeof(T); }
	};

	template<class... Args>
	constexpr auto get_stride_wrap(){
		constexpr auto N = sizeof...(Args);
		auto type_size_buffer = restor<GLsizei, N, Args...>{}.type_size_buffer;
		return std::vector<GLsizei>{type_size_buffer, type_size_buffer + N};
	}

};
