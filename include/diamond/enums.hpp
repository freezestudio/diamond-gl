#pragma once

#include "opengl.hpp"

namespace NS_NAME {

    class _internal_format {
        GLenum _internal;
        GLenum _format;
        GLenum _type;

    public:
        // for fast creation
        _internal_format(GLenum internalFormat) : _internal(internalFormat)
        {
        }

        // for choicable selection
        _internal_format(GLenum internalFormat, GLenum generalFormat, GLenum generalType): _internal(internalFormat), _format(generalFormat), _type(generalType)
        {
        }

        operator GLenum() {
            return _internal;
        }

        GLenum internal() const {
            return _internal;
        }

        GLenum format() const {
            return _format;
        }

        GLenum type() const {
            return _type;
        }
    };


    namespace internal_format {
        // RGBA
        _internal_format rgba32f(GL_RGBA32F, GL_RGBA, GL_FLOAT);
        _internal_format rgba16f(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);
        _internal_format rgba8_unorm(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        _internal_format rgba8_snorm(GL_RGBA8_SNORM, GL_RGBA_SNORM, GL_BYTE);
        _internal_format rgba8ui(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format rgba8i(GL_RGBA8I, GL_RGBA_INTEGER, GL_BYTE);
        _internal_format rgba16_unorm(GL_RGBA16, GL_RGBA, GL_UNSIGNED_SHORT);
        _internal_format rgba16_snorm(GL_RGBA16_SNORM, GL_RGBA_SNORM, GL_SHORT);
        _internal_format rgba16ui(GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format rgba16i(GL_RGBA16I, GL_RGBA_INTEGER, GL_SHORT);
        _internal_format rgba32ui(GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT);
        _internal_format rgba32i(GL_RGBA16I, GL_RGBA_INTEGER, GL_INT);

        // RGB
        _internal_format rgb32f(GL_RGB32F, GL_RGB, GL_FLOAT);
        _internal_format rgb16f(GL_RGB16F, GL_RGB, GL_HALF_FLOAT);
        _internal_format rgb8_unorm(GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE);
        _internal_format rgb8_snorm(GL_RGB8_SNORM, GL_RGB_SNORM, GL_BYTE);
        _internal_format rgb8ui(GL_RGB8UI, GL_RGB_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format rgb8i(GL_RGB8I, GL_RGB_INTEGER, GL_BYTE);
        _internal_format rgb16_unorm(GL_RGB16, GL_RGB, GL_UNSIGNED_SHORT);
        _internal_format rgb16_snorm(GL_RGB16_SNORM, GL_RGB_SNORM, GL_SHORT);
        _internal_format rgb16ui(GL_RGB16UI, GL_RGB_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format rgb16i(GL_RGB16I, GL_RGB_INTEGER, GL_SHORT);
        _internal_format rgb32ui(GL_RGB16UI, GL_RGB_INTEGER, GL_UNSIGNED_INT);
        _internal_format rgb32i(GL_RGB16I, GL_RGB_INTEGER, GL_INT);

        // RG
        _internal_format rg32f(GL_RG32F, GL_RG, GL_FLOAT);
        _internal_format rg16f(GL_RG16F, GL_RG, GL_HALF_FLOAT);
        _internal_format rg8_unorm(GL_RG8, GL_RG, GL_UNSIGNED_BYTE);
        _internal_format rg8_snorm(GL_RG8_SNORM, GL_RG_SNORM, GL_BYTE);
        _internal_format rg8ui(GL_RG8UI, GL_RG_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format rg8i(GL_RG8I, GL_RG_INTEGER, GL_BYTE);
        _internal_format rg16_unorm(GL_RG16, GL_RG, GL_UNSIGNED_SHORT);
        _internal_format rg16_snorm(GL_RG16_SNORM, GL_RG_SNORM, GL_SHORT);
        _internal_format rg16ui(GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format rg16i(GL_RG16I, GL_RG_INTEGER, GL_SHORT);
        _internal_format rg32ui(GL_RG16UI, GL_RG_INTEGER, GL_UNSIGNED_INT);
        _internal_format rg32i(GL_RG16I, GL_RG_INTEGER, GL_INT);

        // Red
        _internal_format r32f(GL_R32F, GL_RED, GL_FLOAT);
        _internal_format r16f(GL_R16F, GL_RED, GL_HALF_FLOAT);
        _internal_format r8_unorm(GL_R8, GL_RED, GL_UNSIGNED_BYTE);
        _internal_format r8_snorm(GL_R8_SNORM, GL_RED_SNORM, GL_BYTE);
        _internal_format r8ui(GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE);
        _internal_format r8i(GL_R8I, GL_RED_INTEGER, GL_BYTE);
        _internal_format r16_unorm(GL_R16, GL_RED, GL_UNSIGNED_SHORT);
        _internal_format r16_snorm(GL_R16_SNORM, GL_RED_SNORM, GL_SHORT);
        _internal_format r16ui(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_SHORT);
        _internal_format r16i(GL_R16I, GL_RED_INTEGER, GL_SHORT);
        _internal_format r32ui(GL_R16UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
        _internal_format r32i(GL_R16I, GL_RED_INTEGER, GL_INT);
    };

    // you can pass these bitfields
    struct buffer_storage_bits {
        union {
            struct {
                GLbitfield map_read : 1;
                GLbitfield map_write : 1;
                GLbitfield map_invalidate_range : 1;
                GLbitfield map_invalidate_buffer : 1;
                GLbitfield map_flush_explicit : 1;
                GLbitfield map_unsynchronized : 1;
                GLbitfield map_persistent : 1;
                GLbitfield map_coherent : 1;
                GLbitfield dynamic_storage : 1;
                GLbitfield client_storage : 1;
                GLbitfield sparse_storage : 1;
            };
            GLbitfield bitfield = 0;
        };

        buffer_storage_bits() {

        }

        buffer_storage_bits(GLbitfield bitfield) {
            this->bitfield = bitfield;
        }
    };



    struct attrib_bits {
        union {
            struct {
                GLbitfield current : 1;
                GLbitfield point : 1;
                GLbitfield line : 1;
                GLbitfield polygon : 1;
                GLbitfield polygon_stipple : 1;
                GLbitfield pixel_mode : 1;
                GLbitfield lighting : 1;
                GLbitfield fog : 1;
                GLbitfield depth_buffer : 1;
                GLbitfield accum_buffer : 1;
                GLbitfield stencil_buffer : 1;
                GLbitfield viewport : 1;
                GLbitfield transform : 1;
                GLbitfield enable : 1;
                GLbitfield color_buffer : 1;
                GLbitfield hint : 1;
                GLbitfield eval : 1;
                GLbitfield list : 1;
                GLbitfield texture : 1;
                GLbitfield scissor : 1;
                GLbitfield multisample : 1;
            };
            GLbitfield bitfield = 0;
        };

        attrib_bits() {

        }

        attrib_bits(GLbitfield bitfield) {
            this->bitfield = bitfield;
        }
    };


    struct program_stage_bits {
        union {
            struct {
                GLbitfield vertex_shader : 1;
                GLbitfield fragment_shader : 1;
                GLbitfield geometry_shader : 1;
                GLbitfield tess_control_shader : 1;
                GLbitfield tess_evalution_shader : 1;
                GLbitfield computer_shader : 1;
            };
            GLbitfield bitfield = 0;
        };

        program_stage_bits() {

        }

        program_stage_bits(GLbitfield bitfield) {
            this->bitfield = bitfield;
        }
    };



};