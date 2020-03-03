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

#include "Mesh.h"

#include <string>
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/Debug.h>

namespace Magnum {

UnsignedInt meshIndexTypeSize(MeshIndexType type) {
    switch(type) {
        case MeshIndexType::UnsignedByte: return 1;
        case MeshIndexType::UnsignedShort: return 2;
        case MeshIndexType::UnsignedInt: return 4;
    }

    CORRADE_ASSERT(false, "meshIndexTypeSize(): invalid type" << type, {});
}

#ifndef DOXYGEN_GENERATING_OUTPUT
namespace {

constexpr const char* MeshPrimitiveNames[] {
    #define _c(primitive) #primitive,
    #include "Magnum/Implementation/meshPrimitiveMapping.hpp"
    #undef _c
};

}

Debug& operator<<(Debug& debug, const MeshPrimitive value) {
    debug << "MeshPrimitive" << Debug::nospace;

    if(UnsignedInt(value) - 1 < Containers::arraySize(MeshPrimitiveNames)) {
        return debug << "::" << Debug::nospace << MeshPrimitiveNames[UnsignedInt(value) - 1];
    }

    return debug << "(" << Debug::nospace << reinterpret_cast<void*>(UnsignedInt(value)) << Debug::nospace << ")";
}

namespace {

constexpr const char* MeshIndexTypeNames[] {
    #define _c(type) #type,
    #include "Magnum/Implementation/meshIndexTypeMapping.hpp"
    #undef _c
};

}

Debug& operator<<(Debug& debug, const MeshIndexType value) {
    debug << "MeshIndexType" << Debug::nospace;

    if(UnsignedInt(value) - 1 < Containers::arraySize(MeshIndexTypeNames)) {
        return debug << "::" << Debug::nospace << MeshIndexTypeNames[UnsignedInt(value) - 1];
    }

    return debug << "(" << Debug::nospace << reinterpret_cast<void*>(UnsignedInt(value)) << Debug::nospace << ")";
}
#endif

UnsignedInt vertexFormatSize(const VertexFormat format) {
    switch(format) {
        case VertexFormat::UnsignedByte:
        case VertexFormat::UnsignedByteNormalized:
        case VertexFormat::Byte:
        case VertexFormat::ByteNormalized:
            return 1;
        case VertexFormat::Half:
        case VertexFormat::UnsignedShort:
        case VertexFormat::UnsignedShortNormalized:
        case VertexFormat::Short:
        case VertexFormat::ShortNormalized:
        case VertexFormat::Vector2ub:
        case VertexFormat::Vector2ubNormalized:
        case VertexFormat::Vector2b:
        case VertexFormat::Vector2bNormalized:
            return 2;
        case VertexFormat::Vector3ub:
        case VertexFormat::Vector3ubNormalized:
        case VertexFormat::Vector3b:
        case VertexFormat::Vector3bNormalized:
            return 3;
        case VertexFormat::Float:
        case VertexFormat::UnsignedInt:
        case VertexFormat::Int:
        case VertexFormat::Vector2h:
        case VertexFormat::Vector2us:
        case VertexFormat::Vector2usNormalized:
        case VertexFormat::Vector2s:
        case VertexFormat::Vector2sNormalized:
        case VertexFormat::Vector4ub:
        case VertexFormat::Vector4ubNormalized:
        case VertexFormat::Vector4b:
        case VertexFormat::Vector4bNormalized:
            return 4;
        case VertexFormat::Vector3h:
        case VertexFormat::Vector3us:
        case VertexFormat::Vector3usNormalized:
        case VertexFormat::Vector3s:
        case VertexFormat::Vector3sNormalized:
            return 6;
        case VertexFormat::Double:
        case VertexFormat::Vector2:
        case VertexFormat::Vector2ui:
        case VertexFormat::Vector2i:
        case VertexFormat::Vector4h:
        case VertexFormat::Vector4us:
        case VertexFormat::Vector4usNormalized:
        case VertexFormat::Vector4s:
        case VertexFormat::Vector4sNormalized:
            return 8;
        case VertexFormat::Vector3:
        case VertexFormat::Vector3ui:
        case VertexFormat::Vector3i:
            return 12;
        case VertexFormat::Vector2d:
        case VertexFormat::Vector4:
        case VertexFormat::Vector4ui:
        case VertexFormat::Vector4i:
            return 16;
        case VertexFormat::Vector3d:
            return 24;
        case VertexFormat::Vector4d:
            return 32;
    }

    CORRADE_ASSERT(false, "vertexFormatSize(): invalid format" << format, {});
}

UnsignedInt vertexFormatComponentCount(const VertexFormat format) {
    switch(format) {
        case VertexFormat::Float:
        case VertexFormat::Half:
        case VertexFormat::Double:
        case VertexFormat::UnsignedByte:
        case VertexFormat::UnsignedByteNormalized:
        case VertexFormat::Byte:
        case VertexFormat::ByteNormalized:
        case VertexFormat::UnsignedShort:
        case VertexFormat::UnsignedShortNormalized:
        case VertexFormat::Short:
        case VertexFormat::ShortNormalized:
        case VertexFormat::UnsignedInt:
        case VertexFormat::Int:
            return 1;

        case VertexFormat::Vector2:
        case VertexFormat::Vector2h:
        case VertexFormat::Vector2d:
        case VertexFormat::Vector2ub:
        case VertexFormat::Vector2ubNormalized:
        case VertexFormat::Vector2b:
        case VertexFormat::Vector2bNormalized:
        case VertexFormat::Vector2us:
        case VertexFormat::Vector2usNormalized:
        case VertexFormat::Vector2s:
        case VertexFormat::Vector2sNormalized:
        case VertexFormat::Vector2ui:
        case VertexFormat::Vector2i:
            return 2;

        case VertexFormat::Vector3:
        case VertexFormat::Vector3h:
        case VertexFormat::Vector3d:
        case VertexFormat::Vector3ub:
        case VertexFormat::Vector3ubNormalized:
        case VertexFormat::Vector3b:
        case VertexFormat::Vector3bNormalized:
        case VertexFormat::Vector3us:
        case VertexFormat::Vector3usNormalized:
        case VertexFormat::Vector3s:
        case VertexFormat::Vector3sNormalized:
        case VertexFormat::Vector3ui:
        case VertexFormat::Vector3i:
            return 3;

        case VertexFormat::Vector4:
        case VertexFormat::Vector4h:
        case VertexFormat::Vector4d:
        case VertexFormat::Vector4ub:
        case VertexFormat::Vector4ubNormalized:
        case VertexFormat::Vector4b:
        case VertexFormat::Vector4bNormalized:
        case VertexFormat::Vector4us:
        case VertexFormat::Vector4usNormalized:
        case VertexFormat::Vector4s:
        case VertexFormat::Vector4sNormalized:
        case VertexFormat::Vector4ui:
        case VertexFormat::Vector4i:
            return 4;
    }

    CORRADE_ASSERT(false, "vertexFormatComponentCount(): invalid format" << format, {});
}

VertexFormat vertexFormatComponentFormat(const VertexFormat format) {
    switch(format) {
        case VertexFormat::Float:
        case VertexFormat::Vector2:
        case VertexFormat::Vector3:
        case VertexFormat::Vector4:
            return VertexFormat::Float;

        case VertexFormat::Half:
        case VertexFormat::Vector2h:
        case VertexFormat::Vector3h:
        case VertexFormat::Vector4h:
            return VertexFormat::Half;

        case VertexFormat::Double:
        case VertexFormat::Vector2d:
        case VertexFormat::Vector3d:
        case VertexFormat::Vector4d:
            return VertexFormat::Double;

        case VertexFormat::UnsignedByte:
        case VertexFormat::UnsignedByteNormalized:
        case VertexFormat::Vector2ub:
        case VertexFormat::Vector2ubNormalized:
        case VertexFormat::Vector3ub:
        case VertexFormat::Vector3ubNormalized:
        case VertexFormat::Vector4ub:
        case VertexFormat::Vector4ubNormalized:
            return VertexFormat::UnsignedByte;

        case VertexFormat::Byte:
        case VertexFormat::ByteNormalized:
        case VertexFormat::Vector2b:
        case VertexFormat::Vector2bNormalized:
        case VertexFormat::Vector3b:
        case VertexFormat::Vector3bNormalized:
        case VertexFormat::Vector4b:
        case VertexFormat::Vector4bNormalized:
            return VertexFormat::Byte;

        case VertexFormat::UnsignedShort:
        case VertexFormat::UnsignedShortNormalized:
        case VertexFormat::Vector2us:
        case VertexFormat::Vector2usNormalized:
        case VertexFormat::Vector3us:
        case VertexFormat::Vector3usNormalized:
        case VertexFormat::Vector4us:
        case VertexFormat::Vector4usNormalized:
            return VertexFormat::UnsignedShort;

        case VertexFormat::Short:
        case VertexFormat::ShortNormalized:
        case VertexFormat::Vector2s:
        case VertexFormat::Vector2sNormalized:
        case VertexFormat::Vector3s:
        case VertexFormat::Vector3sNormalized:
        case VertexFormat::Vector4s:
        case VertexFormat::Vector4sNormalized:
            return VertexFormat::Short;

        case VertexFormat::UnsignedInt:
        case VertexFormat::Vector2ui:
        case VertexFormat::Vector3ui:
        case VertexFormat::Vector4ui:
            return VertexFormat::UnsignedInt;

        case VertexFormat::Int:
        case VertexFormat::Vector2i:
        case VertexFormat::Vector3i:
        case VertexFormat::Vector4i:
            return VertexFormat::Int;
    }

    CORRADE_ASSERT(false, "vertexFormatComponentType(): invalid format" << format, {});
}

bool isVertexFormatNormalized(const VertexFormat format) {
    switch(format) {
        case VertexFormat::Float:
        case VertexFormat::Half:
        case VertexFormat::Double:
        case VertexFormat::UnsignedByte:
        case VertexFormat::Byte:
        case VertexFormat::UnsignedShort:
        case VertexFormat::Short:
        case VertexFormat::UnsignedInt:
        case VertexFormat::Int:
        case VertexFormat::Vector2:
        case VertexFormat::Vector2h:
        case VertexFormat::Vector2d:
        case VertexFormat::Vector2ub:
        case VertexFormat::Vector2b:
        case VertexFormat::Vector2us:
        case VertexFormat::Vector2s:
        case VertexFormat::Vector2ui:
        case VertexFormat::Vector2i:
        case VertexFormat::Vector3:
        case VertexFormat::Vector3h:
        case VertexFormat::Vector3d:
        case VertexFormat::Vector3ub:
        case VertexFormat::Vector3b:
        case VertexFormat::Vector3us:
        case VertexFormat::Vector3s:
        case VertexFormat::Vector3ui:
        case VertexFormat::Vector3i:
        case VertexFormat::Vector4:
        case VertexFormat::Vector4h:
        case VertexFormat::Vector4d:
        case VertexFormat::Vector4ub:
        case VertexFormat::Vector4b:
        case VertexFormat::Vector4us:
        case VertexFormat::Vector4s:
        case VertexFormat::Vector4ui:
        case VertexFormat::Vector4i:
            return false;

        case VertexFormat::UnsignedByteNormalized:
        case VertexFormat::ByteNormalized:
        case VertexFormat::UnsignedShortNormalized:
        case VertexFormat::ShortNormalized:
        case VertexFormat::Vector2ubNormalized:
        case VertexFormat::Vector2bNormalized:
        case VertexFormat::Vector2usNormalized:
        case VertexFormat::Vector2sNormalized:
        case VertexFormat::Vector3ubNormalized:
        case VertexFormat::Vector3bNormalized:
        case VertexFormat::Vector3usNormalized:
        case VertexFormat::Vector3sNormalized:
        case VertexFormat::Vector4ubNormalized:
        case VertexFormat::Vector4bNormalized:
        case VertexFormat::Vector4usNormalized:
        case VertexFormat::Vector4sNormalized:
            return true;
    }

    CORRADE_ASSERT(false, "isVertexFormatNormalized(): invalid format" << format, {});
}

VertexFormat vertexFormat(const VertexFormat format, UnsignedInt componentCount, bool normalized) {
    VertexFormat componentFormat = vertexFormatComponentFormat(format);

    /* First turn the format into a normalized one, if requested */
    if(normalized) {
        switch(componentFormat) {
            case VertexFormat::UnsignedByte:
                componentFormat = VertexFormat::UnsignedByteNormalized;
                break;
            case VertexFormat::Byte:
                componentFormat = VertexFormat::ByteNormalized;
                break;
            case VertexFormat::UnsignedShort:
                componentFormat = VertexFormat::UnsignedShortNormalized;
                break;
            case VertexFormat::Short:
                componentFormat = VertexFormat::ShortNormalized;
                break;
            default: CORRADE_ASSERT(false,
                "vertexFormat():" << format << "can't be made normalized", {});
        }
    }

    /* Then turn them into desired component count, assuming the initial order
       is the same in all cases */
    if(componentCount == 1)
        return componentFormat;
    else if(componentCount == 2)
        return VertexFormat(UnsignedInt(VertexFormat::Vector2) +
            UnsignedInt(componentFormat) - UnsignedInt(VertexFormat::Float));
    else if(componentCount == 3)
        return VertexFormat(UnsignedInt(VertexFormat::Vector3) +
            UnsignedInt(componentFormat) - UnsignedInt(VertexFormat::Float));
    else if(componentCount == 4)
        return VertexFormat(UnsignedInt(VertexFormat::Vector4) +
            UnsignedInt(componentFormat) - UnsignedInt(VertexFormat::Float));
    else CORRADE_ASSERT(false,
        "vertexFormat(): invalid component count" << componentCount, {});

    CORRADE_ASSERT_UNREACHABLE(); /* LCOV_EXCL_LINE */
}

namespace {

constexpr const char* VertexFormatNames[] {
    #define _c(format) #format,
    #include "Magnum/Implementation/vertexFormatMapping.hpp"
    #undef _c
};

}

Debug& operator<<(Debug& debug, const VertexFormat value) {
    debug << "VertexFormat" << Debug::nospace;

    if(UnsignedInt(value) - 1 < Containers::arraySize(VertexFormatNames)) {
        return debug << "::" << Debug::nospace << VertexFormatNames[UnsignedInt(value) - 1];
    }

    return debug << "(" << Debug::nospace << reinterpret_cast<void*>(UnsignedInt(value)) << Debug::nospace << ")";
}

}

namespace Corrade { namespace Utility {

std::string ConfigurationValue<Magnum::MeshPrimitive>::toString(Magnum::MeshPrimitive value, ConfigurationValueFlags) {
    if(Magnum::UnsignedInt(value) - 1 < Containers::arraySize(Magnum::MeshPrimitiveNames))
        return Magnum::MeshPrimitiveNames[Magnum::UnsignedInt(value) - 1];

    return {};
}

Magnum::MeshPrimitive ConfigurationValue<Magnum::MeshPrimitive>::fromString(const std::string& stringValue, ConfigurationValueFlags) {
    for(std::size_t i = 0; i != Containers::arraySize(Magnum::MeshPrimitiveNames); ++i)
        if(stringValue == Magnum::MeshPrimitiveNames[i]) return Magnum::MeshPrimitive(i + 1);

    return {};
}

std::string ConfigurationValue<Magnum::VertexFormat>::toString(Magnum::VertexFormat value, ConfigurationValueFlags) {
    if(Magnum::UnsignedInt(value) - 1 < Containers::arraySize(Magnum::VertexFormatNames))
        return Magnum::VertexFormatNames[Magnum::UnsignedInt(value) - 1];

    return {};
}

Magnum::VertexFormat ConfigurationValue<Magnum::VertexFormat>::fromString(const std::string& stringValue, ConfigurationValueFlags) {
    for(std::size_t i = 0; i != Containers::arraySize(Magnum::VertexFormatNames); ++i)
        if(stringValue == Magnum::VertexFormatNames[i]) return Magnum::VertexFormat(i + 1);

    return {};
}

std::string ConfigurationValue<Magnum::MeshIndexType>::toString(Magnum::MeshIndexType value, ConfigurationValueFlags) {
    if(Magnum::UnsignedInt(value) - 1 < Containers::arraySize(Magnum::MeshIndexTypeNames))
        return Magnum::MeshIndexTypeNames[Magnum::UnsignedInt(value) - 1];

    return {};
}

Magnum::MeshIndexType ConfigurationValue<Magnum::MeshIndexType>::fromString(const std::string& stringValue, ConfigurationValueFlags) {
    for(std::size_t i = 0; i != Containers::arraySize(Magnum::MeshIndexTypeNames); ++i)
        if(stringValue == Magnum::MeshIndexTypeNames[i]) return Magnum::MeshIndexType(i + 1);

    return {};
}

}}
