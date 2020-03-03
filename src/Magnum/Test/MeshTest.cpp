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

#include <sstream>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Configuration.h>
#include <Corrade/Utility/DebugStl.h>

#include "Magnum/Mesh.h"
#include "Magnum/Math/Vector4.h"

namespace Magnum { namespace Test { namespace {

struct MeshTest: TestSuite::Tester {
    explicit MeshTest();

    void primitiveMapping();
    void vertexFormatMapping();
    void indexTypeMapping();

    void vertexFormatSize();
    void vertexFormatSizeInvalid();
    void vertexFormatComponentCount();
    void vertexFormatComponentCountInvalid();
    void vertexFormatComponentFormat();
    void vertexFormatComponentFormatInvalid();
    void vertexFormatIsNormalized();
    void vertexFormatIsNormalizedInvalid();

    void vertexFormatAssemble();
    void vertexFormatAssembleRoundtrip();
    void vertexFormatAssembleCantNormalize();
    void vertexFormatAssembleInvalidComponentCount();

    void indexTypeSize();
    void indexTypeSizeInvalid();

    void debugPrimitive();
    void debugIndexType();
    void debugVertexFormat();

    void configurationPrimitive();
    void configurationIndexType();
    void configurationVertexFormat();
};

constexpr struct {
    VertexFormat componentType;
    bool normalized;
} CombineRoundtripData[] {
    {VertexFormat::Float, false},
    {VertexFormat::Double, false},
    {VertexFormat::UnsignedByte, false},
    {VertexFormat::UnsignedByte, true},
    {VertexFormat::Byte, false},
    {VertexFormat::Byte, true},
    {VertexFormat::UnsignedShort, false},
    {VertexFormat::UnsignedShort, true},
    {VertexFormat::Short, false},
    {VertexFormat::Short, true},
    {VertexFormat::UnsignedInt, false},
    {VertexFormat::Int, false}
};

MeshTest::MeshTest() {
    addTests({&MeshTest::primitiveMapping,
              &MeshTest::vertexFormatMapping,
              &MeshTest::indexTypeMapping,

              &MeshTest::vertexFormatSize,
              &MeshTest::vertexFormatSizeInvalid,
              &MeshTest::vertexFormatComponentCount,
              &MeshTest::vertexFormatComponentCountInvalid,
              &MeshTest::vertexFormatComponentFormat,
              &MeshTest::vertexFormatComponentFormatInvalid,
              &MeshTest::vertexFormatIsNormalized,
              &MeshTest::vertexFormatIsNormalizedInvalid,

              &MeshTest::vertexFormatAssemble});

    addRepeatedInstancedTests({&MeshTest::vertexFormatAssembleRoundtrip}, 4,
        Containers::arraySize(CombineRoundtripData));

    addTests({&MeshTest::vertexFormatAssembleCantNormalize,
              &MeshTest::vertexFormatAssembleInvalidComponentCount,

              &MeshTest::indexTypeSize,
              &MeshTest::indexTypeSizeInvalid,

              &MeshTest::debugPrimitive,
              &MeshTest::debugIndexType,
              &MeshTest::debugVertexFormat,

              &MeshTest::configurationPrimitive,
              &MeshTest::configurationIndexType,
              &MeshTest::configurationVertexFormat});
}

void MeshTest::primitiveMapping() {
    /* This goes through the first 8 bits, which should be enough. */
    UnsignedInt firstUnhandled = 0xff;
    UnsignedInt nextHandled = 1; /* 0 is an invalid primitive */
    for(UnsignedInt i = 1; i <= 0xff; ++i) {
        const auto primitive = MeshPrimitive(i);
        /* Each case verifies:
           - that the entries are ordered by number by comparing a function to
             expected result (so insertion here is done in proper place)
           - that there was no gap (unhandled value inside the range) */
        #ifdef __GNUC__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic error "-Wswitch"
        #endif
        switch(primitive) {
            #define _c(primitive) \
                case MeshPrimitive::primitive: \
                    CORRADE_COMPARE(Utility::ConfigurationValue<MeshPrimitive>::toString(MeshPrimitive::primitive, {}), #primitive); \
                    CORRADE_COMPARE(nextHandled, i); \
                    CORRADE_COMPARE(firstUnhandled, 0xff); \
                    ++nextHandled; \
                    continue;
            #include "Magnum/Implementation/meshPrimitiveMapping.hpp"
            #undef _c
        }
        #ifdef __GNUC__
        #pragma GCC diagnostic pop
        #endif

        /* Not handled by any value, remember -- we might either be at the end
           of the enum range (which is okay) or some value might be unhandled
           here */
        firstUnhandled = i;
    }

    CORRADE_COMPARE(firstUnhandled, 0xff);
}

void MeshTest::indexTypeMapping() {
    /* This goes through the first 8 bits, which should be enough. */
    UnsignedInt firstUnhandled = 0xff;
    UnsignedInt nextHandled = 1; /* 0 is an invalid type */
    for(UnsignedInt i = 1; i <= 0xff; ++i) {
        const auto type = MeshIndexType(i);
        /* Each case verifies:
           - that the entries are ordered by number by comparing a function to
             expected result (so insertion here is done in proper place)
           - that there was no gap (unhandled value inside the range) */
        #ifdef __GNUC__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic error "-Wswitch"
        #endif
        switch(type) {
            #define _c(type) \
                case MeshIndexType::type: \
                    CORRADE_COMPARE(Utility::ConfigurationValue<MeshIndexType>::toString(MeshIndexType::type, {}), #type); \
                    CORRADE_COMPARE(nextHandled, i); \
                    CORRADE_COMPARE(firstUnhandled, 0xff); \
                    ++nextHandled; \
                    continue;
            #include "Magnum/Implementation/meshIndexTypeMapping.hpp"
            #undef _c
        }
        #ifdef __GNUC__
        #pragma GCC diagnostic pop
        #endif

        /* Not handled by any value, remember -- we might either be at the end
           of the enum range (which is okay) or some value might be unhandled
           here */
        firstUnhandled = i;
    }

    CORRADE_COMPARE(firstUnhandled, 0xff);
}

void MeshTest::vertexFormatMapping() {
    /* This goes through the first 16 bits, which should be enough. Going
       through 32 bits takes 8 seconds, too much. */
    UnsignedInt firstUnhandled = 0xffff;
    UnsignedInt nextHandled = 1; /* 0 is an invalid type */
    for(UnsignedInt i = 1; i <= 0xffff; ++i) {
        const auto type = VertexFormat(i);
        /* Each case verifies:
           - that the entries are ordered by number by comparing a function to
             expected result (so insertion here is done in proper place)
           - that there was no gap (unhandled value inside the range) */
        #ifdef __GNUC__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic error "-Wswitch"
        #endif
        switch(type) {
            #define _c(type) \
                case VertexFormat::type: \
                    CORRADE_COMPARE(Utility::ConfigurationValue<VertexFormat>::toString(VertexFormat::type, {}), #type); \
                    CORRADE_COMPARE(nextHandled, i); \
                    CORRADE_COMPARE(firstUnhandled, 0xffff); \
                    ++nextHandled; \
                    continue;
            #include "Magnum/Implementation/vertexFormatMapping.hpp"
            #undef _c
        }
        #ifdef __GNUC__
        #pragma GCC diagnostic pop
        #endif

        /* Not handled by any value, remember -- we might either be at the end
           of the enum range (which is okay) or some value might be unhandled
           here */
        firstUnhandled = i;
    }

    CORRADE_COMPARE(firstUnhandled, 0xffff);
}

void MeshTest::vertexFormatSize() {
    CORRADE_COMPARE(Magnum::vertexFormatSize(VertexFormat::Vector2), sizeof(Vector2));
    CORRADE_COMPARE(Magnum::vertexFormatSize(VertexFormat::Vector3), sizeof(Vector3));
    CORRADE_COMPARE(Magnum::vertexFormatSize(VertexFormat::Vector4), sizeof(Vector4));
}

void MeshTest::vertexFormatSizeInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    Magnum::vertexFormatSize(VertexFormat{});
    Magnum::vertexFormatSize(VertexFormat(0xdead));

    CORRADE_COMPARE(out.str(),
        "vertexFormatSize(): invalid format VertexFormat(0x0)\n"
        "vertexFormatSize(): invalid format VertexFormat(0xdead)\n");
}

void MeshTest::vertexFormatComponentCount() {
    CORRADE_COMPARE(Magnum::vertexFormatComponentCount(VertexFormat::UnsignedByteNormalized), 1);
    CORRADE_COMPARE(Magnum::vertexFormatComponentCount(VertexFormat::Vector2us), 2);
    CORRADE_COMPARE(Magnum::vertexFormatComponentCount(VertexFormat::Vector3bNormalized), 3);
    CORRADE_COMPARE(Magnum::vertexFormatComponentCount(VertexFormat::Vector4), 4);
}

void MeshTest::vertexFormatComponentCountInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    Magnum::vertexFormatComponentCount(VertexFormat{});
    Magnum::vertexFormatComponentCount(VertexFormat(0xdead));

    CORRADE_COMPARE(out.str(),
        "vertexFormatComponentCount(): invalid format VertexFormat(0x0)\n"
        "vertexFormatComponentCount(): invalid format VertexFormat(0xdead)\n");
}

void MeshTest::vertexFormatComponentFormat() {
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector4), VertexFormat::Float);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector3h), VertexFormat::Half);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector2d), VertexFormat::Double);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::UnsignedByte), VertexFormat::UnsignedByte);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::UnsignedByteNormalized), VertexFormat::UnsignedByte);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector3bNormalized), VertexFormat::Byte);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector2us), VertexFormat::UnsignedShort);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector2sNormalized), VertexFormat::Short);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector2ui), VertexFormat::UnsignedInt);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(VertexFormat::Vector3i), VertexFormat::Int);
}

void MeshTest::vertexFormatComponentFormatInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    Magnum::vertexFormatComponentFormat(VertexFormat{});
    Magnum::vertexFormatComponentFormat(VertexFormat(0xdead));

    CORRADE_COMPARE(out.str(),
        "vertexFormatComponentType(): invalid format VertexFormat(0x0)\n"
        "vertexFormatComponentType(): invalid format VertexFormat(0xdead)\n");
}

void MeshTest::vertexFormatIsNormalized() {
    CORRADE_VERIFY(isVertexFormatNormalized(VertexFormat::UnsignedByteNormalized));
    CORRADE_VERIFY(!isVertexFormatNormalized(VertexFormat::Vector2us));
    CORRADE_VERIFY(isVertexFormatNormalized(VertexFormat::Vector3bNormalized));
    CORRADE_VERIFY(!isVertexFormatNormalized(VertexFormat::Vector4));
}

void MeshTest::vertexFormatIsNormalizedInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    isVertexFormatNormalized(VertexFormat{});
    isVertexFormatNormalized(VertexFormat(0xdead));

    CORRADE_COMPARE(out.str(),
        "isVertexFormatNormalized(): invalid format VertexFormat(0x0)\n"
        "isVertexFormatNormalized(): invalid format VertexFormat(0xdead)\n");
}

void MeshTest::vertexFormatAssemble() {
    CORRADE_COMPARE(vertexFormat(VertexFormat::UnsignedShort, 3, true),
        VertexFormat::Vector3usNormalized);
    CORRADE_COMPARE(vertexFormat(VertexFormat::Int, 4, false),
        VertexFormat::Vector4i);
    CORRADE_COMPARE(vertexFormat(VertexFormat::Double, 1, false),
        VertexFormat::Double);
    CORRADE_COMPARE(vertexFormat(VertexFormat::Byte, 1, true),
        VertexFormat::ByteNormalized);

    /* Non-scalar types allowed too, as that makes the internal checking
       much simpler than when requiring the type to be scalar non-normalized */
    CORRADE_COMPARE(vertexFormat(VertexFormat::Vector4bNormalized, 2, false),
        VertexFormat::Vector2b);
    CORRADE_COMPARE(vertexFormat(VertexFormat::Vector3h, 2, false),
        VertexFormat::Vector2h);
}

void MeshTest::vertexFormatAssembleRoundtrip() {
    auto&& data = CombineRoundtripData[testCaseInstanceId()];

    std::ostringstream out;
    {
        Debug d{&out, Debug::Flag::NoNewlineAtTheEnd};
        d << data.componentType;
        if(data.normalized) d << Debug::nospace << ", normalized";
    }
    setTestCaseDescription(out.str());

    VertexFormat result = vertexFormat(data.componentType, testCaseRepeatId() + 1, data.normalized);
    CORRADE_COMPARE(Magnum::vertexFormatComponentFormat(result), data.componentType);
    CORRADE_COMPARE(Magnum::vertexFormatComponentCount(result), testCaseRepeatId() + 1);
    CORRADE_COMPARE(isVertexFormatNormalized(result), data.normalized);
}

void MeshTest::vertexFormatAssembleCantNormalize() {
    std::ostringstream out;
    Error redirectError{&out};
    vertexFormat(VertexFormat::Vector2, 1, true);
    CORRADE_COMPARE(out.str(),
        "vertexFormat(): VertexFormat::Vector2 can't be made normalized\n");
}

void MeshTest::vertexFormatAssembleInvalidComponentCount() {
    std::ostringstream out;
    Error redirectError{&out};
    vertexFormat(VertexFormat::Vector3, 5, false);
    CORRADE_COMPARE(out.str(),
        "vertexFormat(): invalid component count 5\n");
}

void MeshTest::indexTypeSize() {
    CORRADE_COMPARE(meshIndexTypeSize(MeshIndexType::UnsignedByte), 1);
    CORRADE_COMPARE(meshIndexTypeSize(MeshIndexType::UnsignedShort), 2);
    CORRADE_COMPARE(meshIndexTypeSize(MeshIndexType::UnsignedInt), 4);
}

void MeshTest::indexTypeSizeInvalid() {
    std::ostringstream out;
    Error redirectError{&out};

    meshIndexTypeSize(MeshIndexType{});
    meshIndexTypeSize(MeshIndexType(0xfe));

    CORRADE_COMPARE(out.str(),
        "meshIndexTypeSize(): invalid type MeshIndexType(0x0)\n"
        "meshIndexTypeSize(): invalid type MeshIndexType(0xfe)\n");
}

void MeshTest::debugPrimitive() {
    std::ostringstream o;
    Debug(&o) << MeshPrimitive::TriangleFan << MeshPrimitive(0xfe);
    CORRADE_COMPARE(o.str(), "MeshPrimitive::TriangleFan MeshPrimitive(0xfe)\n");
}

void MeshTest::debugIndexType() {
    std::ostringstream o;
    Debug(&o) << MeshIndexType::UnsignedShort << MeshIndexType(0xfe);
    CORRADE_COMPARE(o.str(), "MeshIndexType::UnsignedShort MeshIndexType(0xfe)\n");
}

void MeshTest::debugVertexFormat() {
    std::ostringstream o;
    Debug(&o) << VertexFormat::Vector4 << VertexFormat(0xdead);
    CORRADE_COMPARE(o.str(), "VertexFormat::Vector4 VertexFormat(0xdead)\n");
}

void MeshTest::configurationPrimitive() {
    Utility::Configuration c;

    c.setValue("primitive", MeshPrimitive::LineStrip);
    CORRADE_COMPARE(c.value("primitive"), "LineStrip");
    CORRADE_COMPARE(c.value<MeshPrimitive>("primitive"), MeshPrimitive::LineStrip);

    c.setValue("zero", MeshPrimitive{});
    CORRADE_COMPARE(c.value("zero"), "");
    CORRADE_COMPARE(c.value<MeshPrimitive>("zero"), MeshPrimitive{});

    c.setValue("invalid", MeshPrimitive(0xdead));
    CORRADE_COMPARE(c.value("invalid"), "");
    CORRADE_COMPARE(c.value<MeshPrimitive>("invalid"), MeshPrimitive{});
}

void MeshTest::configurationIndexType() {
    Utility::Configuration c;

    c.setValue("type", MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(c.value("type"), "UnsignedShort");
    CORRADE_COMPARE(c.value<MeshIndexType>("type"), MeshIndexType::UnsignedShort);

    c.setValue("zero", MeshIndexType{});
    CORRADE_COMPARE(c.value("zero"), "");
    CORRADE_COMPARE(c.value<MeshIndexType>("zero"), MeshIndexType{});

    c.setValue("invalid", MeshIndexType(0xdead));
    CORRADE_COMPARE(c.value("invalid"), "");
    CORRADE_COMPARE(c.value<MeshIndexType>("invalid"), MeshIndexType{});
}

void MeshTest::configurationVertexFormat() {
    Utility::Configuration c;

    c.setValue("type", VertexFormat::Vector3);
    CORRADE_COMPARE(c.value("type"), "Vector3");
    CORRADE_COMPARE(c.value<VertexFormat>("type"), VertexFormat::Vector3);

    c.setValue("zero", VertexFormat{});
    CORRADE_COMPARE(c.value("zero"), "");
    CORRADE_COMPARE(c.value<VertexFormat>("zero"), VertexFormat{});

    c.setValue("invalid", VertexFormat(0xdead));
    CORRADE_COMPARE(c.value("invalid"), "");
    CORRADE_COMPARE(c.value<VertexFormat>("invalid"), VertexFormat{});
}

}}}

CORRADE_TEST_MAIN(Magnum::Test::MeshTest)
