#ifndef Magnum_Mesh_h
#define Magnum_Mesh_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Enum @ref Magnum::MeshPrimitive, @ref Magnum::MeshIndexType, @ref Magnum::VertexFormat, function @ref Magnum::meshIndexTypeSize(), @ref Magnum::vertexFormatSize(), @ref Magnum::vertexFormatComponentCount(), @ref Magnum::vertexFormatComponentFormat(), @ref Magnum::isVertexFormatNormalized()
 */

#include <Corrade/Utility/StlForwardString.h>

#include "Magnum/Magnum.h"
#include "Magnum/visibility.h"

namespace Magnum {

/**
@brief Mesh primitive type

In case of OpenGL, corresponds to @ref GL::MeshPrimitive and is convertible to
it using @ref GL::meshPrimitive(). See documentation of each value for more
information about the mapping.

In case of Vulkan, corresponds to @type_vk_keyword{PrimitiveTopology} and is
convertible to it using @ref Vk::vkPrimitiveTopology(). See documentation of
each value for more information about the mapping. Note that not every mode is available there, use @ref Vk::hasVkPrimitiveTopology() to check for its
presence.

For D3D, corresponds to @m_class{m-doc-external} [D3D_PRIMITIVE_TOPOLOGY](https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology);
for Metal, corresponds to @m_class{m-doc-external} [MTLPrimitiveType](https://developer.apple.com/documentation/metal/mtlprimitivetype?language=objc).
See documentation of each value for more information about the mapping.
*/
enum class MeshPrimitive: UnsignedByte {
    /* Zero reserved for an invalid type (but not being a named value) */

    /**
     * Single points.
     *
     * Corresponds to @ref GL::MeshPrimitive::Points;
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_POINT_LIST,PrimitiveTopology};
     * @m_class{m-doc-external} [D3D_PRIMITIVE_TOPOLOGY_POINTLIST](https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology)
     * or @m_class{m-doc-external} [MTLPrimitiveTypePoint](https://developer.apple.com/documentation/metal/mtlprimitivetype/mtlprimitivetypepoint?language=objc).
     * @m_keywords{D3D_PRIMITIVE_TOPOLOGY_POINTLIST MTLPrimitiveTypePoint}
     */
    Points = 1,

    /**
     * Each pair of vertices defines a single line, lines aren't
     * connected together.
     *
     * Corresponds to @ref GL::MeshPrimitive::Lines /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_LINE_LIST,PrimitiveTopology};
     * @m_class{m-doc-external} [D3D_PRIMITIVE_TOPOLOGY_LINELIST](https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology)
     * or @m_class{m-doc-external} [MTLPrimitiveTypeLine](https://developer.apple.com/documentation/metal/mtlprimitivetype/mtlprimitivetypeline?language=objc).
     * @m_keywords{D3D_PRIMITIVE_TOPOLOGY_LINELIST MTLPrimitiveTypeLine}
     */
    Lines,

    /**
     * Line strip, last and first vertex are connected together.
     *
     * Corresponds to @ref GL::MeshPrimitive::LineLoop. Not supported on
     * Vulkan, D3D or Metal.
     */
    LineLoop,

    /**
     * First two vertices define first line segment, each following
     * vertex defines another segment.
     *
     * Corresponds to @ref GL::MeshPrimitive::LineStrip /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_LINE_STRIP,PrimitiveTopology};
     * @m_class{m-doc-external} [D3D_PRIMITIVE_TOPOLOGY_LINESTRIP](https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology)
     * or @m_class{m-doc-external} [MTLPrimitiveTypeLineStrip](https://developer.apple.com/documentation/metal/mtlprimitivetype/mtlprimitivetypelinestrip?language=objc).
     * @m_keywords{D3D_PRIMITIVE_TOPOLOGY_LINESTRIP MTLPrimitiveTypeLineStrip}
     */
    LineStrip,

    /**
     * Each three vertices define one triangle.
     *
     * Corresponds to @ref GL::MeshPrimitive::Triangles /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,PrimitiveTopology};
     * @m_class{m-doc-external} [D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST](https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology)
     * or @m_class{m-doc-external} [MTLPrimitiveTypeTriangle](https://developer.apple.com/documentation/metal/mtlprimitivetype/mtlprimitivetypetriangle?language=objc).
     * @m_keywords{D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST MTLPrimitiveTypeTriangle}
     */
    Triangles,

    /**
     * First three vertices define first triangle, each following
     * vertex defines another triangle.
     *
     * Corresponds to @ref GL::MeshPrimitive::TriangleStrip /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,PrimitiveTopology} or
     * @m_class{m-doc-external} [D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP](https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology)
     * or @m_class{m-doc-external} [MTLPrimitiveTypeTriangleStrip](https://developer.apple.com/documentation/metal/mtlprimitivetype/mtlprimitivetypetrianglestrip?language=objc).
     * @m_keywords{D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP MTLPrimitiveTypeTriangleStrip}
     */
    TriangleStrip,

    /**
     * First vertex is center, each following vertex is connected to
     * previous and center vertex.
     *
     * Corresponds to @ref GL::MeshPrimitive::TriangleFan /
     * @def_vk_keyword{PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,PrimitiveTopology}. Not
     * supported on D3D or Metal.
     */
    TriangleFan
};

/** @debugoperatorenum{MeshPrimitive} */
MAGNUM_EXPORT Debug& operator<<(Debug& debug, MeshPrimitive value);

/**
@brief Mesh index type

In case of OpenGL, corresponds to @ref GL::MeshIndexType and is convertible to
it using @ref GL::meshIndexType(). See documentation of each value for more
information about the mapping.

In case of Vulkan, corresponds to @type_vk_keyword{IndexType} and is
convertible to it using @ref Vk::vkIndexType(). See documentation of each value
for more information about the mapping. Note that not every type is available
there, use @ref Vk::hasVkIndexType() to check for its presence.
@see @ref meshIndexTypeSize()
*/
enum class MeshIndexType: UnsignedByte {
    /* Zero reserved for an invalid type (but not being a named value) */

    /**
     * Unsigned byte
     *
     * Corresponds to @ref GL::MeshIndexType::UnsignedByte. Not available on
     * Vulkan. While available on OpenGL, at least AMD GPUs are known to
     * suggest (via debug output) using 16-byte types instead for better
     * efficiency.
     */
    UnsignedByte = 1,

    /**
     * Unsigned short
     *
     * Corresponds to @ref GL::MeshIndexType::UnsignedShort /
     * @def_vk_keyword{INDEX_TYPE_UINT16,IndexType}.
     */
    UnsignedShort,

    /**
     * Unsigned int
     *
     * Corresponds to @ref GL::MeshIndexType::UnsignedInt /
     * @def_vk_keyword{INDEX_TYPE_UINT32,IndexType}.
     */
    UnsignedInt
};

/** @brief Size of given mesh index type */
MAGNUM_EXPORT UnsignedInt meshIndexTypeSize(MeshIndexType type);

/** @debugoperatorenum{MeshIndexType} */
MAGNUM_EXPORT Debug& operator<<(Debug& debug, MeshIndexType value);

/**
@brief Vertex format
@m_since_latest

Like @ref PixelFormat, but for mesh attributes --- including double-precision
types and matrices.
@see @ref Trade::MeshData, @ref Trade::MeshAttributeData,
    @ref Trade::MeshAttribute
*/
enum class VertexFormat: UnsignedInt {
    /* Zero reserved for an invalid type (but not being a named value) */

    /** @ref Float */
    Float = 1,

    /** @ref Half */
    Half,

    /** @ref Double */
    Double,

    /** @ref UnsignedByte */
    UnsignedByte,

    /**
     * @ref UnsignedByte, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$.
     */
    UnsignedByteNormalized,

    /** @ref Byte */
    Byte,

    /**
     * @ref Byte, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     */
    ByteNormalized,

    /** @ref UnsignedShort */
    UnsignedShort,

    /**
     * @ref UnsignedShort, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$.
     */
    UnsignedShortNormalized,

    /** @ref Short */
    Short,

    /**
     * @ref Short, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     */
    ShortNormalized,

    /** @ref UnsignedInt */
    UnsignedInt,

    /** @ref Int */
    Int,

    /**
     * @ref Vector2. Usually used for 2D positions and 2D texture coordinates.
     */
    Vector2,

    /**
     * @ref Vector2h. Can be used instead of @ref VertexFormat::Vector2 for 2D
     * positions and 2D texture coordinates.
     */
    Vector2h,

    /** @ref Vector2d */
    Vector2d,

    /**
     * @ref Vector2ub. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [0, 255] @f$ is interpreted as @f$ [0.0, 255.0] @f$.
     */
    Vector2ub,

    /**
     * @ref Vector2ub, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     */
    Vector2ubNormalized,

    /**
     * @ref Vector2b. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [-128, 127] @f$ is interpreted as @f$ [-128.0, 127.0] @f$.
     */
    Vector2b,

    /**
     * @ref Vector2b, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     */
    Vector2bNormalized,

    /**
     * @ref Vector2us. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [0, 65535] @f$ is interpreted as @f$ [0.0, 65535.0] @f$.
     */
    Vector2us,

    /**
     * @ref Vector2us, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     */
    Vector2usNormalized,

    /**
     * @ref Vector2s. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [-32768, 32767] @f$ is interpreted as @f$ [-32768.0, 32767.0] @f$.
     */
    Vector2s,

    /**
     * @ref Vector2s, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     */
    Vector2sNormalized,

    /** @ref Vector2ui */
    Vector2ui,

    /** @ref Vector2i */
    Vector2i,

    /**
     * @ref Vector3 or @ref Color3. Usually used for 3D positions, normals and
     * three-component colors.
     */
    Vector3,

    /**
     * @ref Vector3h. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions and three-component colors.
     */
    Vector3h,

    /** @ref Vector3d */
    Vector3d,

    /**
     * @ref Vector3ub. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions, in which case the range @f$ [0, 255] @f$ is
     * interpreted as @f$ [0.0, 255.0] @f$.
     */
    Vector3ub,

    /**
     * @ref Vector3ub, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector3
     * for packed 3D positions and three-component colors.
     */
    Vector3ubNormalized,

    /**
     * @ref Vector3b. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions, in which case the range @f$ [-128, 127] @f$ is
     * interpreted as @f$ [-128.0, 127.0] @f$.
     */
    Vector3b,

    /**
     * @ref Vector3b, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of
     * @ref VertexFormat::Vector3 for packed 3D positions and normals.
     */
    Vector3bNormalized,

    /**
     * @ref Vector3us. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 2D positions, in which case the range @f$ [0, 65535] @f$ is
     * interpreted as @f$ [0.0, 65535.0] @f$.
     */
    Vector3us,

    /**
     * @ref Vector3us, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 3D positions and three-component colors.
     */
    Vector3usNormalized,

    /**
     * @ref Vector3s. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions, in which case the range @f$ [-32768, 32767] @f$ is
     * interpreted as @f$ [-32768.0, 32767.0] @f$.
     */
    Vector3s,

    /**
     * @ref Vector3s, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector3
     * for packed 3D positions and normals.
     */
    Vector3sNormalized,

    /** @ref Vector3ui */
    Vector3ui,

    /** @ref Vector3i */
    Vector3i,

    /**
     * @ref Vector4 or @ref Color4. Usually used for four-component colors.
     */
    Vector4,

    /**
     * @ref Vector4h. Can be used instead of @ref VertexFormat::Vector4 for
     * four-component colors.
     */
    Vector4h,

    /** @ref Vector4d */
    Vector4d,

    /** @ref Vector4ub */
    Vector4ub,

    /**
     * @ref Vector4ub, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector4
     * for packed linear four-component colors.
     */
    Vector4ubNormalized,

    /** @ref Vector4b */
    Vector4b,

    /**
     * @ref Vector4b, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     */
    Vector4bNormalized,

    /** @ref Vector4us */
    Vector4us,

    /**
     * @ref Vector4us, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector4
     * for packed linear four-component colors.
     */
    Vector4usNormalized,

    /** @ref Vector4s */
    Vector4s,

    /**
     * @ref Vector4s, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     */
    Vector4sNormalized,

    /** @ref Vector4ui */
    Vector4ui,

    /** @ref Vector4i */
    Vector4i
};

/**
@brief Size of given vertex format
@m_since_latest

To get size of a single component, call this function on a result of
@ref vertexFormatComponentFormat().
*/
MAGNUM_EXPORT UnsignedInt vertexFormatSize(VertexFormat format);

/**
@brief Component format of given vertex format
@m_since_latest

The function also removes the normalization aspect from the type --- use
@ref isVertexFormatNormalized() to query that. Returns for example
@ref VertexFormat::Short for @ref VertexFormat::ShortNormalized or
@ref VertexFormat::UnsignedByte for @ref VertexFormat::Vector3ub.
Calling @ref vertexFormatComponentCount() on the return value will always
give @cpp 1 @ce; calling @ref isVertexFormatNormalized() on the return
value will always give @cpp false @ce.
@see @ref vertexFormat(VertexFormat, UnsignedInt, bool)
*/
MAGNUM_EXPORT VertexFormat vertexFormatComponentFormat(VertexFormat format);

/**
@brief Component count of given vertex format
@m_since_latest

Returns @cpp 1 @ce for scalar types and e.g. @cpp 3 @ce for
@ref VertexFormat::Vector3ub.
@see @ref vertexFormat(VertexFormat, UnsignedInt, bool)
*/
MAGNUM_EXPORT UnsignedInt vertexFormatComponentCount(VertexFormat format);

/**
@brief Component count of given vertex format
@m_since_latest

Returns @cpp true @ce for `*Normalized` types, @cpp false @ce otherwise. In
particular, floating-point types are *not* treated as normalized, even though
for example colors might commonly have values only in the @f$ [0.0, 1.0] @f$
range (or normals in the @f$ [-1.0, 1.0] @f$ range).
@see @ref vertexFormat(VertexFormat, UnsignedInt, bool)
*/
MAGNUM_EXPORT bool isVertexFormatNormalized(VertexFormat format);

/**
@brief Assemble a vertex format from parts
@m_since_latest

Converts @p format to a new format of desired component count and
normalization. Expects that @p componentCount is not larger than @cpp 4 @ce and
@p normalized is @cpp true @ce only for 8- and 16-byte integer types.
@see @ref vertexFormatComponentFormat(),
    @ref vertexFormatComponentCount(),
    @ref isVertexFormatNormalized()
*/
MAGNUM_EXPORT VertexFormat vertexFormat(VertexFormat format, UnsignedInt componentCount, bool normalized);

/**
@debugoperatorenum{VertexFormat}
@m_since_latest
*/
MAGNUM_EXPORT Debug& operator<<(Debug& debug, VertexFormat value);

}

namespace Corrade { namespace Utility {

/** @configurationvalue{Magnum::MeshPrimitive} */
template<> struct MAGNUM_EXPORT ConfigurationValue<Magnum::MeshPrimitive> {
    ConfigurationValue() = delete;

    /**
     * @brief Writes enum value as string
     *
     * If the value is invalid, returns empty string.
     */
    static std::string toString(Magnum::MeshPrimitive value, ConfigurationValueFlags);

    /**
     * @brief Reads enum value as string
     *
     * If the value is invalid, returns a zero (invalid) primitive.
     */
    static Magnum::MeshPrimitive fromString(const std::string& stringValue, ConfigurationValueFlags);
};

/** @configurationvalue{Magnum::MeshIndexType} */
template<> struct MAGNUM_EXPORT ConfigurationValue<Magnum::MeshIndexType> {
    ConfigurationValue() = delete;

    /**
     * @brief Write enum value as string
     *
     * If the value is invalid, returns empty string.
     */
    static std::string toString(Magnum::MeshIndexType value, ConfigurationValueFlags);

    /**
     * @brief Read enum value as string
     *
     * If the value is invalid, returns a zero (invalid) type.
     */
    static Magnum::MeshIndexType fromString(const std::string& stringValue, ConfigurationValueFlags);
};

/**
@configurationvalue{Magnum::VertexFormat}
@m_since_latest
*/
template<> struct MAGNUM_EXPORT ConfigurationValue<Magnum::VertexFormat> {
    ConfigurationValue() = delete;

    /**
     * @brief Write enum value as string
     *
     * If the value is invalid, returns empty string.
     */
    static std::string toString(Magnum::VertexFormat value, ConfigurationValueFlags);

    /**
     * @brief Read enum value as string
     *
     * If the value is invalid, returns a zero (invalid) type.
     */
    static Magnum::VertexFormat fromString(const std::string& stringValue, ConfigurationValueFlags);
};

}}

#endif
