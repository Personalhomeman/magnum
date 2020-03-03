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
 * @brief Enum @ref Magnum::MeshPrimitive, @ref Magnum::MeshIndexType, @ref Magnum::VertexFormat, function @ref Magnum::meshIndexTypeSize(), @ref Magnum::isVertexFormatImplementationSpecific(), @ref Magnum::vertexFormatWrap(), @ref Magnum::vertexFormatUnwrap(), @ref Magnum::vertexFormatSize(), @ref Magnum::vertexFormatComponentCount(), @ref Magnum::vertexFormatComponentFormat(), @ref Magnum::isVertexFormatNormalized()
 */

#include <Corrade/Utility/Assert.h>
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
types and matrices. Can act also as a wrapper for implementation-specific mesh
attribute type values using @ref vertexFormatWrap() and
@ref vertexFormatUnwrap(). Distinction between generic and
implementation-specific types can be done using
@ref isVertexFormatImplementationSpecific().

In case of OpenGL, corresponds to a tuple of @ref GL::DynamicAttribute::Kind,
@ref GL::DynamicAttribute::Components and @ref GL::DynamicAttribute::DataType
and is convertible to them using
@ref GL::DynamicAttribute::DynamicAttribute(Kind, UnsignedInt, VertexFormat).
See documentation of each value for more information about the mapping. Note
that not every format is available on all targets, use
@ref GL::hasVertexFormat() to check for its presence.

In case of Vulkan, corresponds to @type_vk_keyword{Format} and is convertible
to it using @ref Vk::vkFormat(Magnum::VertexFormat). See documentation of each
value for more information about the mapping. Note that not every format may be
available, use @ref Vk::hasVkFormat(Magnum::VertexFormat) to check for its
presence.

For D3D, corresponds to @m_class{m-doc-external} [DXGI_FORMAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format);
for Metal, corresponds to @m_class{m-doc-external} [MTLVertexFormat](https://developer.apple.com/documentation/metal/mtlvertexformat?language=objc).
See documentation of each value for more information about the mapping.
@see @ref Trade::MeshData, @ref Trade::MeshAttributeData,
    @ref Trade::MeshAttribute
*/
enum class VertexFormat: UnsignedInt {
    /* Zero reserved for an invalid type (but not being a named value) */

    /**
     * @ref Float.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Float;
     * @def_vk_keyword{FORMAT_R32_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatFloat](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatfloat?language=objc).
     * @m_keywords{DXGI_FORMAT_R32_FLOAT MTLVertexFormatFloat}
     */
    Float = 1,

    /**
     * @ref Half.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Half;
     * @def_vk_keyword{FORMAT_R16_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatHalf](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformathalf?language=objc).
     * @m_keywords{DXGI_FORMAT_R16_FLOAT MTLVertexFormatHalf}
     */
    Half,

    /**
     * @ref Double.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Long
     * @ref GL::DynamicAttribute::DataType::Double;
     * @def_vk_keyword{FORMAT_R64_SFLOAT,Format}. No D3D or Metal equivalent.
     */
    Double,

    /**
     * @ref UnsignedByte.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUChar](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuchar?language=objc)
     * @m_keywords{DXGI_FORMAT_R8_UINT MTLVertexFormatUChar}
     */
    UnsignedByte,

    /**
     * @ref UnsignedByte, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$.
     *
     * Corresponds to single-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUCharNormalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatucharnormalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8_UNORM MTLVertexFormatUCharNormalized}
     */
    UnsignedByteNormalized,

    /**
     * @ref Byte.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatChar](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatchar?language=objc).
     * @m_keywords{DXGI_FORMAT_R8_SINT MTLVertexFormatChar}
     */
    Byte,

    /**
     * @ref Byte, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     *
     * Corresponds to single-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatCharNormalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatcharnormalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8_SNORM MTLVertexFormatCharNormalized}
     */
    ByteNormalized,

    /**
     * @ref UnsignedShort.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedShort;
     * @def_vk_keyword{FORMAT_R16_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShort](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushort?language=objc).
     * @m_keywords{DXGI_FORMAT_R16_UINT MTLVertexFormatUShort}
     */
    UnsignedShort,

    /**
     * @ref UnsignedShort, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$.
     *
     * Corresponds to single-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R16_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShortNormalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushortnormalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16_UNORM MTLVertexFormatUShortNormalized}
     */
    UnsignedShortNormalized,

    /**
     * @ref Short.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShort](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshort?language=objc).
     * @m_keywords{DXGI_FORMAT_R16_SINT MTLVertexFormatShort}
     */
    Short,

    /**
     * @ref Short, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     *
     * Corresponds to single-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShortNormalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshortnormalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16_SNORM MTLVertexFormatShortNormalized}
     */
    ShortNormalized,

    /**
     * @ref UnsignedInt.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedInt;
     * @def_vk_keyword{FORMAT_R32_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUInt](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuint?language=objc).
     * @m_keywords{DXGI_FORMAT_R32_UINT MTLVertexFormatUInt}
     */
    UnsignedInt,

    /**
     * @ref Int.
     *
     * Corresponds to single-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Int;
     * @def_vk_keyword{FORMAT_R32_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatInt](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatint?language=objc).
     * @m_keywords{DXGI_FORMAT_R32_SINT MTLVertexFormatInt}
     */
    Int,

    /**
     * @ref Vector2. Usually used for 2D positions and 2D texture coordinates.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Float;
     * @def_vk_keyword{FORMAT_R32G32_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatFloat2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatfloat2?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32_FLOAT MTLVertexFormatFloat2}
     */
    Vector2,

    /**
     * @ref Vector2h. Can be used instead of @ref VertexFormat::Vector2 for 2D
     * positions and 2D texture coordinates.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Half;
     * @def_vk_keyword{FORMAT_R16G16_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatHalf2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformathalf2?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16_FLOAT MTLVertexFormatHalf2}
     */
    Vector2h,

    /**
     * @ref Vector2d.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Long
     * @ref GL::DynamicAttribute::DataType::Double;
     * @def_vk_keyword{FORMAT_R64G64_SFLOAT,Format}. No D3D or Metal
     * equivalent.
     */
    Vector2d,

    /**
     * @ref Vector2ub. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [0, 255] @f$ is interpreted as @f$ [0.0, 255.0] @f$.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8G8_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUChar2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuchar2?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8_UINT MTLVertexFormatUChar2}
     */
    Vector2ub,

    /**
     * @ref Vector2ub, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     *
     * Corresponds to two-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8G8_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUChar2Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuchar2normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8_UNORM MTLVertexFormatUChar2Normalized}
     */
    Vector2ubNormalized,

    /**
     * @ref Vector2b. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [-128, 127] @f$ is interpreted as @f$ [-128.0, 127.0] @f$.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8G8_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatChar2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatchar2?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8_SINT MTLVertexFormatChar2}
     */
    Vector2b,

    /**
     * @ref Vector2b, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     *
     * Corresponds to two-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8G8_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatChar2Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatchar2normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8_SNORM MTLVertexFormatChar2Normalized}
     */
    Vector2bNormalized,

    /**
     * @ref Vector2us. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [0, 65535] @f$ is interpreted as @f$ [0.0, 65535.0] @f$.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedShort;
     * @def_vk_keyword{FORMAT_R16G16_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShort2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushort2?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16_UINT MTLVertexFormatUShort2}
     */
    Vector2us,

    /**
     * @ref Vector2us, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     *
     * Corresponds to two-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R16G16_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShort2Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushort2normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16_UNORM MTLVertexFormatUShort2Normalized}
     */
    Vector2usNormalized,

    /**
     * @ref Vector2s. Can be used instead of @ref VertexFormat::Vector2 for
     * packed 2D positions and 2D texture coordinates, in which case the range
     * @f$ [-32768, 32767] @f$ is interpreted as @f$ [-32768.0, 32767.0] @f$.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16G16_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShort2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshort2?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16_SINT MTLVertexFormatShort2}
     */
    Vector2s,

    /**
     * @ref Vector2s, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 2D positions and 2D texture coordinates.
     *
     * Corresponds to two-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16G16_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShort2Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshort2normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16_SNORM MTLVertexFormatShort2Normalized}
     */
    Vector2sNormalized,

    /**
     * @ref Vector2ui.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedInt;
     * @def_vk_keyword{FORMAT_R32G32_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUInt2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuint2?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32_UINT MTLVertexFormatUInt2}
     */
    Vector2ui,

    /**
     * @ref Vector2i.
     *
     * Corresponds to two-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Int;
     * @def_vk_keyword{FORMAT_R32G32_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatInt2](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatint2?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32_SINT MTLVertexFormatInt2}
     */
    Vector2i,

    /**
     * @ref Vector3 or @ref Color3. Usually used for 3D positions, normals and
     * three-component colors.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Float;
     * @def_vk_keyword{FORMAT_R32G32B32_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32B32_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatFloat3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatfloat3?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32B32_FLOAT MTLVertexFormatFloat3}
     */
    Vector3,

    /**
     * @ref Vector3h. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions and three-component colors.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Half;
     * @def_vk_keyword{FORMAT_R16G16B16_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatHalf3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformathalf3?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16_FLOAT MTLVertexFormatHalf3}
     */
    Vector3h,

    /**
     * @ref Vector3d.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Long
     * @ref GL::DynamicAttribute::DataType::Double;
     * @def_vk_keyword{FORMAT_R64G64B64_SFLOAT,Format}. No D3D or Metal
     * equivalent.
     */
    Vector3d,

    /**
     * @ref Vector3ub. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions, in which case the range @f$ [0, 255] @f$ is
     * interpreted as @f$ [0.0, 255.0] @f$.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8G8B8_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUChar3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuchar3?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8_UINT MTLVertexFormatUChar3}
     */
    Vector3ub,

    /**
     * @ref Vector3ub, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector3
     * for packed 3D positions and three-component colors.
     *
     * Corresponds to three-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8G8B8_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUChar3Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuchar3normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8_UNORM MTLVertexFormatUChar3Normalized}
     */
    Vector3ubNormalized,

    /**
     * @ref Vector3b. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions, in which case the range @f$ [-128, 127] @f$ is
     * interpreted as @f$ [-128.0, 127.0] @f$.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8G8B8_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatChar3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatchar3?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8_SINT MTLVertexFormatChar3}
     */
    Vector3b,

    /**
     * @ref Vector3b, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of
     * @ref VertexFormat::Vector3 for packed 3D positions and normals.
     *
     * Corresponds to three-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8G8B8_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatChar3Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatchar3normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8_SNORM MTLVertexFormatChar3Normalized}
     */
    Vector3bNormalized,

    /**
     * @ref Vector3us. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 2D positions, in which case the range @f$ [0, 65535] @f$ is
     * interpreted as @f$ [0.0, 65535.0] @f$.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedShort;
     * @def_vk_keyword{FORMAT_R16G16B16_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShort3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushort3?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16_UINT MTLVertexFormatUShort3}
     */
    Vector3us,

    /**
     * @ref Vector3us, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector2
     * for packed 3D positions and three-component colors.
     *
     * Corresponds to three-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R16G16B16_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShort3Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushort3normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16_UNORM MTLVertexFormatUShort3Normalized}
     */
    Vector3usNormalized,

    /**
     * @ref Vector3s. Can be used instead of @ref VertexFormat::Vector3 for
     * packed 3D positions, in which case the range @f$ [-32768, 32767] @f$ is
     * interpreted as @f$ [-32768.0, 32767.0] @f$.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16G16B16_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShort3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshort3?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16_SINT MTLVertexFormatShort3}
     */
    Vector3s,

    /**
     * @ref Vector3s, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector3
     * for packed 3D positions and normals.
     *
     * Corresponds to three-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16G16B16_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShort3Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshort3normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16_SNORM MTLVertexFormatShort3Normalized}
     */
    Vector3sNormalized,

    /**
     * @ref Vector3ui.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedInt;
     * @def_vk_keyword{FORMAT_R32G32B32_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32B32_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUInt3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuint3?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32B32_UINT MTLVertexFormatUInt3}
     */
    Vector3ui,

    /**
     * @ref Vector3i.
     *
     * Corresponds to three-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Int;
     * @def_vk_keyword{FORMAT_R32G32B32_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32B32_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatInt3](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatint3?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32B32_SINT MTLVertexFormatInt3}
     */
    Vector3i,

    /**
     * @ref Vector4 or @ref Color4. Usually used for four-component colors.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Float;
     * @def_vk_keyword{FORMAT_R32G32B32A32_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32B32A32_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatFloat4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatfloat4?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32B32A32_FLOAT MTLVertexFormatFloat4}
     */
    Vector4,

    /**
     * @ref Vector4h. Can be used instead of @ref VertexFormat::Vector4 for
     * four-component colors.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * @ref GL::DynamicAttribute::DataType::Half;
     * @def_vk_keyword{FORMAT_R16G16B16A16_SFLOAT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16A16_FLOAT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatHalf4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformathalf4?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16A16_FLOAT MTLVertexFormatHalf4}
     */
    Vector4h,

    /**
     * @ref Vector4d.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Long
     * @ref GL::DynamicAttribute::DataType::Double;
     * @def_vk_keyword{FORMAT_R64G64B64A64_SFLOAT,Format}. No D3D or Metal
     * equivalent.
     */
    Vector4d,

    /**
     * @ref Vector4ub.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8G8B8A8_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8A8_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUChar4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuchar4?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8A8_UINT MTLVertexFormatUChar4}
     */
    Vector4ub,

    /**
     * @ref Vector4ub, with range @f$ [0, 255] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector4
     * for packed linear four-component colors.
     *
     * Corresponds to four-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R8G8B8A8_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8A8_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUChar4Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuchar4normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8A8_UNORM MTLVertexFormatUChar4Normalized}
     */
    Vector4ubNormalized,

    /**
     * @ref Vector4b.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8G8B8A8_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8A8_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatChar4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatchar4?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8A8_SINT MTLVertexFormatChar4}
     */
    Vector4b,

    /**
     * @ref Vector4b, with range @f$ [-127, 127] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     *
     * Corresponds to four-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Byte;
     * @def_vk_keyword{FORMAT_R8G8B8A8_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R8G8B8A8_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatChar4Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatchar4normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R8G8B8A8_SNORM MTLVertexFormatChar4Normalized}
     */
    Vector4bNormalized,

    /**
     * @ref Vector4us.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedShort;
     * @def_vk_keyword{FORMAT_R16G16B16A16_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16A16_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShort4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushort4?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16A16_UINT MTLVertexFormatUShort4}
     */
    Vector4us,

    /**
     * @ref Vector4us, with range @f$ [0, 65535] @f$ interpreted as
     * @f$ [0.0, 1.0] @f$. Can be used instead of @ref VertexFormat::Vector4
     * for packed linear four-component colors.
     *
     * Corresponds to four-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::UnsignedByte;
     * @def_vk_keyword{FORMAT_R16G16B16A16_UNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16A16_UNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUShort4Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatushort4normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16A16_UNORM MTLVertexFormatUShort4Normalized}
     */
    Vector4usNormalized,

    /**
     * @ref Vector4s.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16G16B16A16_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16A16_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShort4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshort4?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16A16_SINT MTLVertexFormatShort4}
     */
    Vector4s,

    /**
     * @ref Vector4s, with range @f$ [-32767, 32767] @f$ interpreted as
     * @f$ [-1.0, 1.0] @f$.
     *
     * Corresponds to four-component
     * @ref GL::DynamicAttribute::Kind::GenericNormalized
     * @ref GL::DynamicAttribute::DataType::Short;
     * @def_vk_keyword{FORMAT_R16G16B16A16_SNORM,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R16G16B16A16_SNORM](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatShort4Normalized](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatshort4normalized?language=objc).
     * @m_keywords{DXGI_FORMAT_R16G16B16A16_SNORM MTLVertexFormatShort4Normalized}
     */
    Vector4sNormalized,

    /**
     * @ref Vector4ui.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::UnsignedInt;
     * @def_vk_keyword{FORMAT_R32G32B32A16_UINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32B32A32_UINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatUInt4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatuint4?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32B32A32_UINT MTLVertexFormatUInt4}
     */
    Vector4ui,

    /**
     * @ref Vector4i.
     *
     * Corresponds to four-component @ref GL::DynamicAttribute::Kind::Generic
     * or @ref GL::DynamicAttribute::Kind::Integral
     * @ref GL::DynamicAttribute::DataType::Int;
     * @def_vk_keyword{FORMAT_R32G32B32A32_SINT,Format};
     * @m_class{m-doc-external} [DXGI_FORMAT_R32G32B32A32_SINT](https://docs.microsoft.com/en-us/windows/win32/api/dxgiformat/ne-dxgiformat-dxgi_format)
     * or @m_class{m-doc-external} [MTLVertexFormatInt4](https://developer.apple.com/documentation/metal/mtlvertexformat/mtlvertexformatint4?language=objc).
     * @m_keywords{DXGI_FORMAT_R32G32B32A32_SINT MTLVertexFormatInt4}
     */
    Vector4i
};

/**
@debugoperatorenum{VertexFormat}
@m_since_latest
*/
MAGNUM_EXPORT Debug& operator<<(Debug& debug, VertexFormat value);

/**
@brief Whether a @ref VertexFormat value wraps an implementation-specific identifier
@m_since_latest

Returns @cpp true @ce if value of @p format has its highest bit set,
@cpp false @ce otherwise. Use @ref vertexFormatWrap() and @ref vertexFormatUnwrap()
to wrap/unwrap an implementation-specific indentifier to/from
@ref VertexFormat.
*/
constexpr bool isVertexFormatImplementationSpecific(VertexFormat format) {
    return UnsignedInt(format) & (1u << 31);
}

/**
@brief Wrap an implementation-specific vertex format identifier in @ref VertexFormat
@m_since_latest

Sets the highest bit on @p type to mark it as implementation-specific. Expects
that @p type fits into the remaining bits. Use @ref vertexFormatUnwrap()
for the inverse operation.
@see @ref isVertexFormatImplementationSpecific()
*/
template<class T> constexpr VertexFormat vertexFormatWrap(T implementationSpecific) {
    static_assert(sizeof(T) <= 4, "types larger than 32bits are not supported");
    return CORRADE_CONSTEXPR_ASSERT(!(UnsignedInt(implementationSpecific) & (1u << 31)),
        "vertexFormatWrap(): implementation-specific value" << reinterpret_cast<void*>(implementationSpecific) << "already wrapped or too large"),
        VertexFormat((1u << 31)|UnsignedInt(implementationSpecific));
}

/**
@brief Unwrap an implementation-specific vertex format identifier from @ref VertexFormat
@m_since_latest

Unsets the highest bit from @p type to extract the implementation-specific
value. Expects that @p type has it set. Use @ref vertexFormatWrap() for
the inverse operation.
@see @ref isVertexFormatImplementationSpecific()
*/
template<class T = UnsignedInt> constexpr T vertexFormatUnwrap(VertexFormat format) {
    return CORRADE_CONSTEXPR_ASSERT(UnsignedInt(format) & (1u << 31),
        "vertexFormatUnwrap():" << format << "isn't a wrapped implementation-specific value"),
        T(UnsignedInt(format) & ~(1u << 31));
}

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
