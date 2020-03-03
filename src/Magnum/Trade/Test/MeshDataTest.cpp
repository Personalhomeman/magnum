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
#include <Corrade/TestSuite/Compare/Container.h>
#include <Corrade/Utility/DebugStl.h>

#include "Magnum/Math/Color.h"
#include "Magnum/Math/Half.h"
#include "Magnum/Trade/MeshData.h"

namespace Magnum { namespace Trade { namespace Test { namespace {

struct MeshDataTest: TestSuite::Tester {
    explicit MeshDataTest();

    void customAttributeName();
    void customAttributeNameTooLarge();
    void customAttributeNameNotCustom();
    void debugAttributeName();

    void constructIndex();
    void constructIndexTypeErased();
    void constructIndexTypeErasedWrongSize();
    void constructIndex2D();
    void constructIndex2DWrongSize();
    void constructIndex2DNonContiguous();
    void constructIndexNullptr();

    void constructAttribute();
    void constructAttributeCustom();
    void constructAttributeWrongFormat();
    void constructAttribute2D();
    void constructAttribute2DWrongSize();
    void constructAttribute2DNonContiguous();
    void constructAttributeTypeErased();
    void constructAttributeNullptr();
    void constructAttributePadding();
    void constructAttributeNonOwningArray();
    void constructAttributeWrongStride();

    void construct();
    void constructZeroIndices();
    void constructZeroAttributes();
    void constructZeroVertices();
    void constructIndexless();
    void constructIndexlessZeroVertices();
    void constructAttributeless();
    void constructIndexlessAttributeless();
    void constructIndexlessAttributelessZeroVertices();

    void constructNotOwned();
    void constructIndicesNotOwned();
    void constructVerticesNotOwned();
    void constructIndexlessNotOwned();
    void constructAttributelessNotOwned();

    void constructIndexDataButNotIndexed();
    void constructAttributelessInvalidIndices();
    void constructIndicesNotContained();
    void constructAttributeNotContained();
    void constructInconsitentVertexCount();
    void constructNotOwnedIndexFlagOwned();
    void constructNotOwnedVertexFlagOwned();
    void constructIndicesNotOwnedFlagOwned();
    void constructVerticesNotOwnedFlagOwned();
    void constructIndexlessNotOwnedFlagOwned();
    void constructAttributelessNotOwnedFlagOwned();
    void constructInvalidAttributeData();

    void constructCopy();
    void constructMove();

    template<class T> void indicesAsArray();
    void indicesIntoArrayInvalidSize();
    template<class T> void positions2DAsArray();
    template<class T> void positions2DAsArrayPackedUnsigned();
    template<class T> void positions2DAsArrayPackedSigned();
    template<class T> void positions2DAsArrayPackedUnsignedNormalized();
    template<class T> void positions2DAsArrayPackedSignedNormalized();
    void positions2DIntoArrayInvalidSize();
    template<class T> void positions3DAsArray();
    template<class T> void positions3DAsArrayPackedUnsigned();
    template<class T> void positions3DAsArrayPackedSigned();
    template<class T> void positions3DAsArrayPackedUnsignedNormalized();
    template<class T> void positions3DAsArrayPackedSignedNormalized();
    void positions3DIntoArrayInvalidSize();
    template<class T> void normalsAsArray();
    template<class T> void normalsAsArrayPackedSignedNormalized();
    void normalsIntoArrayInvalidSize();
    template<class T> void textureCoordinates2DAsArray();
    template<class T> void textureCoordinates2DAsArrayPackedUnsigned();
    template<class T> void textureCoordinates2DAsArrayPackedSigned();
    template<class T> void textureCoordinates2DAsArrayPackedUnsignedNormalized();
    template<class T> void textureCoordinates2DAsArrayPackedSignedNormalized();
    void textureCoordinates2DIntoArrayInvalidSize();
    template<class T> void colorsAsArray();
    template<class T> void colorsAsArrayPackedUnsignedNormalized();
    void colorsIntoArrayInvalidSize();

    void mutableAccessNotAllowed();

    void indicesNotIndexed();
    void indicesWrongType();

    void attributeNotFound();
    void attributeWrongType();

    void releaseIndexData();
    void releaseAttributeData();
    void releaseVertexData();
};

struct {
    const char* name;
    DataFlags indexDataFlags, vertexDataFlags;
} NotOwnedData[] {
    {"", {}, {}},
    {"indices mutable", DataFlag::Mutable, {}},
    {"vertices mutable", {}, DataFlag::Mutable},
    {"both mutable", DataFlag::Mutable, DataFlag::Mutable}
};

struct {
    const char* name;
    DataFlags dataFlags;
} SingleNotOwnedData[] {
    {"", {}},
    {"mutable", DataFlag::Mutable}
};

MeshDataTest::MeshDataTest() {
    addTests({&MeshDataTest::customAttributeName,
              &MeshDataTest::customAttributeNameTooLarge,
              &MeshDataTest::customAttributeNameNotCustom,
              &MeshDataTest::debugAttributeName,

              &MeshDataTest::constructIndex,
              &MeshDataTest::constructIndexTypeErased,
              &MeshDataTest::constructIndexTypeErasedWrongSize,
              &MeshDataTest::constructIndex2D,
              &MeshDataTest::constructIndex2DWrongSize,
              &MeshDataTest::constructIndex2DNonContiguous,
              &MeshDataTest::constructIndexNullptr,

              &MeshDataTest::constructAttribute,
              &MeshDataTest::constructAttributeCustom,
              &MeshDataTest::constructAttributeWrongFormat,
              &MeshDataTest::constructAttribute2D,
              &MeshDataTest::constructAttribute2DWrongSize,
              &MeshDataTest::constructAttribute2DNonContiguous,
              &MeshDataTest::constructAttributeTypeErased,
              &MeshDataTest::constructAttributeNullptr,
              &MeshDataTest::constructAttributePadding,
              &MeshDataTest::constructAttributeNonOwningArray,
              &MeshDataTest::constructAttributeWrongStride,

              &MeshDataTest::construct,
              &MeshDataTest::constructZeroIndices,
              &MeshDataTest::constructZeroAttributes,
              &MeshDataTest::constructZeroVertices,
              &MeshDataTest::constructIndexless,
              &MeshDataTest::constructIndexlessZeroVertices,
              &MeshDataTest::constructAttributeless,
              &MeshDataTest::constructIndexlessAttributeless,
              &MeshDataTest::constructIndexlessAttributelessZeroVertices});

    addInstancedTests({&MeshDataTest::constructNotOwned},
        Containers::arraySize(NotOwnedData));
    addInstancedTests({&MeshDataTest::constructIndicesNotOwned,
                       &MeshDataTest::constructVerticesNotOwned,
                       &MeshDataTest::constructIndexlessNotOwned,
                       &MeshDataTest::constructAttributelessNotOwned},
        Containers::arraySize(SingleNotOwnedData));

    addTests({&MeshDataTest::constructIndexDataButNotIndexed,
              &MeshDataTest::constructAttributelessInvalidIndices,
              &MeshDataTest::constructIndicesNotContained,
              &MeshDataTest::constructAttributeNotContained,
              &MeshDataTest::constructInconsitentVertexCount,
              &MeshDataTest::constructNotOwnedIndexFlagOwned,
              &MeshDataTest::constructNotOwnedVertexFlagOwned,
              &MeshDataTest::constructIndicesNotOwnedFlagOwned,
              &MeshDataTest::constructVerticesNotOwnedFlagOwned,
              &MeshDataTest::constructIndexlessNotOwnedFlagOwned,
              &MeshDataTest::constructAttributelessNotOwnedFlagOwned,
              &MeshDataTest::constructInvalidAttributeData,

              &MeshDataTest::constructCopy,
              &MeshDataTest::constructMove,

              &MeshDataTest::indicesAsArray<UnsignedByte>,
              &MeshDataTest::indicesAsArray<UnsignedShort>,
              &MeshDataTest::indicesAsArray<UnsignedInt>,
              &MeshDataTest::indicesIntoArrayInvalidSize,
              &MeshDataTest::positions2DAsArray<Vector2>,
              &MeshDataTest::positions2DAsArray<Vector2h>,
              &MeshDataTest::positions2DAsArray<Vector3>,
              &MeshDataTest::positions2DAsArray<Vector3h>,
              &MeshDataTest::positions2DAsArrayPackedUnsigned<Vector2ub>,
              &MeshDataTest::positions2DAsArrayPackedUnsigned<Vector2us>,
              &MeshDataTest::positions2DAsArrayPackedUnsigned<Vector3ub>,
              &MeshDataTest::positions2DAsArrayPackedUnsigned<Vector3us>,
              &MeshDataTest::positions2DAsArrayPackedSigned<Vector2b>,
              &MeshDataTest::positions2DAsArrayPackedSigned<Vector2s>,
              &MeshDataTest::positions2DAsArrayPackedSigned<Vector3b>,
              &MeshDataTest::positions2DAsArrayPackedSigned<Vector3s>,
              &MeshDataTest::positions2DAsArrayPackedUnsignedNormalized<Vector2ub>,
              &MeshDataTest::positions2DAsArrayPackedUnsignedNormalized<Vector2us>,
              &MeshDataTest::positions2DAsArrayPackedUnsignedNormalized<Vector3ub>,
              &MeshDataTest::positions2DAsArrayPackedUnsignedNormalized<Vector3us>,
              &MeshDataTest::positions2DAsArrayPackedSignedNormalized<Vector2b>,
              &MeshDataTest::positions2DAsArrayPackedSignedNormalized<Vector2s>,
              &MeshDataTest::positions2DAsArrayPackedSignedNormalized<Vector3b>,
              &MeshDataTest::positions2DAsArrayPackedSignedNormalized<Vector3s>,
              &MeshDataTest::positions2DIntoArrayInvalidSize,
              &MeshDataTest::positions3DAsArray<Vector2>,
              &MeshDataTest::positions3DAsArray<Vector2h>,
              &MeshDataTest::positions3DAsArray<Vector3>,
              &MeshDataTest::positions3DAsArray<Vector3h>,
              &MeshDataTest::positions3DAsArrayPackedUnsigned<Vector2ub>,
              &MeshDataTest::positions3DAsArrayPackedUnsigned<Vector2us>,
              &MeshDataTest::positions3DAsArrayPackedUnsigned<Vector3ub>,
              &MeshDataTest::positions3DAsArrayPackedUnsigned<Vector3us>,
              &MeshDataTest::positions3DAsArrayPackedSigned<Vector2b>,
              &MeshDataTest::positions3DAsArrayPackedSigned<Vector2s>,
              &MeshDataTest::positions3DAsArrayPackedSigned<Vector3b>,
              &MeshDataTest::positions3DAsArrayPackedSigned<Vector3s>,
              &MeshDataTest::positions3DAsArrayPackedUnsignedNormalized<Vector2ub>,
              &MeshDataTest::positions3DAsArrayPackedUnsignedNormalized<Vector2us>,
              &MeshDataTest::positions3DAsArrayPackedUnsignedNormalized<Vector3ub>,
              &MeshDataTest::positions3DAsArrayPackedUnsignedNormalized<Vector3us>,
              &MeshDataTest::positions3DAsArrayPackedSignedNormalized<Vector2b>,
              &MeshDataTest::positions3DAsArrayPackedSignedNormalized<Vector2s>,
              &MeshDataTest::positions3DAsArrayPackedSignedNormalized<Vector3b>,
              &MeshDataTest::positions3DAsArrayPackedSignedNormalized<Vector3s>,
              &MeshDataTest::positions3DIntoArrayInvalidSize,
              &MeshDataTest::normalsAsArray<Vector3>,
              &MeshDataTest::normalsAsArray<Vector3h>,
              &MeshDataTest::normalsAsArrayPackedSignedNormalized<Vector3b>,
              &MeshDataTest::normalsAsArrayPackedSignedNormalized<Vector3s>,
              &MeshDataTest::normalsIntoArrayInvalidSize,
              &MeshDataTest::textureCoordinates2DAsArray<Vector2>,
              &MeshDataTest::textureCoordinates2DAsArray<Vector2h>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedUnsigned<Vector2ub>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedUnsigned<Vector2us>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedSigned<Vector2b>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedSigned<Vector2s>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedUnsignedNormalized<Vector2ub>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedUnsignedNormalized<Vector2us>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedSignedNormalized<Vector2b>,
              &MeshDataTest::textureCoordinates2DAsArrayPackedSignedNormalized<Vector2s>,
              &MeshDataTest::textureCoordinates2DIntoArrayInvalidSize,
              &MeshDataTest::colorsAsArray<Color3>,
              &MeshDataTest::colorsAsArray<Color3h>,
              &MeshDataTest::colorsAsArray<Color4>,
              &MeshDataTest::colorsAsArray<Color4h>,
              &MeshDataTest::colorsAsArrayPackedUnsignedNormalized<Color3ub>,
              &MeshDataTest::colorsAsArrayPackedUnsignedNormalized<Color3us>,
              &MeshDataTest::colorsAsArrayPackedUnsignedNormalized<Color4ub>,
              &MeshDataTest::colorsAsArrayPackedUnsignedNormalized<Color4us>,
              &MeshDataTest::colorsIntoArrayInvalidSize,

              &MeshDataTest::mutableAccessNotAllowed,

              &MeshDataTest::indicesNotIndexed,
              &MeshDataTest::indicesWrongType,

              &MeshDataTest::attributeNotFound,
              &MeshDataTest::attributeWrongType,

              &MeshDataTest::releaseIndexData,
              &MeshDataTest::releaseAttributeData,
              &MeshDataTest::releaseVertexData});
}

void MeshDataTest::customAttributeName() {
    CORRADE_VERIFY(!isMeshAttributeCustom(MeshAttribute::Position));
    CORRADE_VERIFY(!isMeshAttributeCustom(MeshAttribute(32767)));
    CORRADE_VERIFY(isMeshAttributeCustom(MeshAttribute::Custom));
    CORRADE_VERIFY(isMeshAttributeCustom(MeshAttribute(65535)));

    CORRADE_COMPARE(UnsignedShort(meshAttributeCustom(0)), 32768);
    CORRADE_COMPARE(UnsignedShort(meshAttributeCustom(8290)), 41058);
    CORRADE_COMPARE(UnsignedShort(meshAttributeCustom(32767)), 65535);

    CORRADE_COMPARE(meshAttributeCustom(MeshAttribute::Custom), 0);
    CORRADE_COMPARE(meshAttributeCustom(MeshAttribute(41058)), 8290);
    CORRADE_COMPARE(meshAttributeCustom(MeshAttribute(65535)), 32767);

    constexpr bool is = isMeshAttributeCustom(MeshAttribute(41058));
    CORRADE_VERIFY(is);
    constexpr MeshAttribute a = meshAttributeCustom(8290);
    CORRADE_COMPARE(UnsignedShort(a), 41058);
    constexpr UnsignedShort b = meshAttributeCustom(a);
    CORRADE_COMPARE(b, 8290);
}

void MeshDataTest::customAttributeNameTooLarge() {
    std::ostringstream out;
    Error redirectError{&out};
    meshAttributeCustom(32768);
    CORRADE_COMPARE(out.str(), "Trade::meshAttributeCustom(): index 32768 too large\n");
}

void MeshDataTest::customAttributeNameNotCustom() {
    std::ostringstream out;
    Error redirectError{&out};
    meshAttributeCustom(MeshAttribute::TextureCoordinates);
    CORRADE_COMPARE(out.str(), "Trade::meshAttributeCustom(): Trade::MeshAttribute::TextureCoordinates is not custom\n");
}

void MeshDataTest::debugAttributeName() {
    std::ostringstream out;
    Debug{&out} << MeshAttribute::Position << meshAttributeCustom(73) << MeshAttribute(0x73);
    CORRADE_COMPARE(out.str(), "Trade::MeshAttribute::Position Trade::MeshAttribute::Custom(73) Trade::MeshAttribute(0x73)\n");
}

using namespace Math::Literals;

constexpr UnsignedByte IndexBytes[]{25, 132, 3};
constexpr UnsignedShort IndexShorts[]{2575, 13224, 3};
constexpr UnsignedInt IndexInts[]{2110122, 132257, 3};

void MeshDataTest::constructIndex() {
    {
        const UnsignedByte indexData[]{25, 132, 3};
        MeshIndexData indices{indexData};
        CORRADE_COMPARE(indices.type(), MeshIndexType::UnsignedByte);
        CORRADE_COMPARE(indices.data().data(), indexData);

        constexpr MeshIndexData cindices{IndexBytes};
        constexpr MeshIndexType type = cindices.type();
        constexpr Containers::ArrayView<const void> data = cindices.data();
        CORRADE_COMPARE(type, MeshIndexType::UnsignedByte);
        CORRADE_COMPARE(data.data(), IndexBytes);
    } {
        const UnsignedShort indexData[]{2575, 13224, 3};
        MeshIndexData indices{indexData};
        CORRADE_COMPARE(indices.type(), MeshIndexType::UnsignedShort);
        CORRADE_COMPARE(indices.data().data(), indexData);

        constexpr MeshIndexData cindices{IndexShorts};
        constexpr MeshIndexType type = cindices.type();
        constexpr Containers::ArrayView<const void> data = cindices.data();
        CORRADE_COMPARE(type, MeshIndexType::UnsignedShort);
        CORRADE_COMPARE(data.data(), IndexShorts);
    } {
        const UnsignedInt indexData[]{2110122, 132257, 3};
        MeshIndexData indices{indexData};
        CORRADE_COMPARE(indices.type(), MeshIndexType::UnsignedInt);
        CORRADE_COMPARE(indices.data().data(), indexData);

        constexpr MeshIndexData cindices{IndexInts};
        constexpr MeshIndexType type = cindices.type();
        constexpr Containers::ArrayView<const void> data = cindices.data();
        CORRADE_COMPARE(type, MeshIndexType::UnsignedInt);
        CORRADE_COMPARE(data.data(), IndexInts);
    }
}

void MeshDataTest::constructIndexTypeErased() {
    const char indexData[3*2]{};
    MeshIndexData indices{MeshIndexType::UnsignedShort, indexData};
    CORRADE_COMPARE(indices.type(), MeshIndexType::UnsignedShort);
    CORRADE_VERIFY(indices.data().data() == indexData);
}

void MeshDataTest::constructIndexTypeErasedWrongSize() {
    const char indexData[3*2]{};

    std::ostringstream out;
    Error redirectError{&out};
    MeshIndexData{MeshIndexType::UnsignedInt, indexData};
    CORRADE_COMPARE(out.str(), "Trade::MeshIndexData: view size 6 does not correspond to MeshIndexType::UnsignedInt\n");
}

void MeshDataTest::constructIndex2D() {
    {
        const UnsignedByte indexData[]{25, 132, 3};
        MeshIndexData indices{Containers::arrayCast<2, const char>(Containers::stridedArrayView(indexData))};
        CORRADE_COMPARE(indices.type(), MeshIndexType::UnsignedByte);
        CORRADE_COMPARE(indices.data().data(), indexData);
    } {
        const UnsignedShort indexData[]{2575, 13224, 3};
        MeshIndexData indices{Containers::arrayCast<2, const char>(Containers::stridedArrayView(indexData))};
        CORRADE_COMPARE(indices.type(), MeshIndexType::UnsignedShort);
        CORRADE_COMPARE(indices.data().data(), indexData);
    } {
        const UnsignedInt indexData[]{2110122, 132257, 3};
        MeshIndexData indices{Containers::arrayCast<2, const char>(Containers::stridedArrayView(indexData))};
        CORRADE_COMPARE(indices.type(), MeshIndexType::UnsignedInt);
        CORRADE_COMPARE(indices.data().data(), indexData);
    }
}

void MeshDataTest::constructIndex2DWrongSize() {
    const char data[3*3]{};

    std::ostringstream out;
    Error redirectError{&out};
    MeshIndexData{Containers::StridedArrayView2D<const char>{data, {3, 3}}};
    CORRADE_COMPARE(out.str(), "Trade::MeshIndexData: expected index type size 1, 2 or 4 but got 3\n");
}

void MeshDataTest::constructIndex2DNonContiguous() {
    const char data[3*4]{};

    std::ostringstream out;
    Error redirectError{&out};
    MeshIndexData{Containers::StridedArrayView2D<const char>{data, {3, 2}, {4, 2}}};
    CORRADE_COMPARE(out.str(), "Trade::MeshIndexData: view is not contiguous\n");
}

void MeshDataTest::constructIndexNullptr() {
    /* Just verify it's not ambiguous */
    MeshIndexData data{nullptr};
    CORRADE_VERIFY(!data.data());
}

constexpr Vector2 Positions[] {
    {1.2f, 0.2f},
    {2.2f, 1.1f},
    {-0.2f, 7.2f}
};

void MeshDataTest::constructAttribute() {
    const Vector2 positionData[3];
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(positionData)};
    CORRADE_COMPARE(positions.name(), MeshAttribute::Position);
    CORRADE_COMPARE(positions.format(), VertexFormat::Vector2);
    CORRADE_VERIFY(positions.data().data() == positionData);

    constexpr MeshAttributeData cpositions{MeshAttribute::Position, Containers::arrayView(Positions)};
    constexpr MeshAttribute name = cpositions.name();
    constexpr VertexFormat format = cpositions.format();
    constexpr Containers::StridedArrayView1D<const void> data = cpositions.data();
    CORRADE_COMPARE(name, MeshAttribute::Position);
    CORRADE_COMPARE(format, VertexFormat::Vector2);
    CORRADE_COMPARE(data.data(), Positions);
}

void MeshDataTest::constructAttributeCustom() {
    const Short idData[3]{};
    MeshAttributeData ids{meshAttributeCustom(13), Containers::arrayView(idData)};
    CORRADE_COMPARE(ids.name(), meshAttributeCustom(13));
    CORRADE_COMPARE(ids.format(), VertexFormat::Short);
    CORRADE_VERIFY(ids.data().data() == idData);
}

void MeshDataTest::constructAttributeWrongFormat() {
    Vector2 positionData[3];

    std::ostringstream out;
    Error redirectError{&out};
    MeshAttributeData{MeshAttribute::Color, Containers::arrayView(positionData)};
    CORRADE_COMPARE(out.str(), "Trade::MeshAttributeData: VertexFormat::Vector2 is not a valid format for Trade::MeshAttribute::Color\n");
}

void MeshDataTest::constructAttribute2D() {
    char positionData[4*sizeof(Vector2)]{};
    auto positionView = Containers::StridedArrayView2D<char>{positionData,
        {4, sizeof(Vector2)}}.every(2);

    MeshAttributeData positions{MeshAttribute::Position, VertexFormat::Vector2, positionView};
    CORRADE_COMPARE(positions.name(), MeshAttribute::Position);
    CORRADE_COMPARE(positions.format(), VertexFormat::Vector2);
    CORRADE_COMPARE(positions.data().data(), positionView.data());
}

void MeshDataTest::constructAttribute2DWrongSize() {
    char positionData[4*sizeof(Vector2)]{};

    std::ostringstream out;
    Error redirectError{&out};
    MeshAttributeData{MeshAttribute::Position, VertexFormat::Vector3,
        Containers::StridedArrayView2D<char>{positionData,
            {4, sizeof(Vector2)}}.every(2)};
    CORRADE_COMPARE(out.str(), "Trade::MeshAttributeData: second view dimension size 8 doesn't match VertexFormat::Vector3\n");
}

void MeshDataTest::constructAttribute2DNonContiguous() {
    char positionData[4*sizeof(Vector2)]{};

    std::ostringstream out;
    Error redirectError{&out};
    MeshAttributeData{MeshAttribute::Position, VertexFormat::Vector2,
        Containers::StridedArrayView2D<char>{positionData,
            {2, sizeof(Vector2)*2}}.every({1, 2})};
    CORRADE_COMPARE(out.str(), "Trade::MeshAttributeData: second view dimension is not contiguous\n");
}

void MeshDataTest::constructAttributeTypeErased() {
    const Vector3 positionData[3]{};
    MeshAttributeData positions{MeshAttribute::Position, VertexFormat::Vector3, Containers::arrayCast<const char>(Containers::stridedArrayView(positionData))};
    CORRADE_COMPARE(positions.name(), MeshAttribute::Position);
    CORRADE_COMPARE(positions.format(), VertexFormat::Vector3);
    CORRADE_VERIFY(positions.data().data() == positionData);
}

void MeshDataTest::constructAttributeNullptr() {
    MeshAttributeData positions{MeshAttribute::Position, VertexFormat::Vector2, nullptr};
    CORRADE_COMPARE(positions.name(), MeshAttribute::Position);
    CORRADE_COMPARE(positions.format(), VertexFormat::Vector2);
    CORRADE_VERIFY(!positions.data().data());
}

void MeshDataTest::constructAttributePadding() {
    MeshAttributeData padding{-35};
    CORRADE_COMPARE(padding.name(), MeshAttribute{});
    CORRADE_COMPARE(padding.format(), VertexFormat{});
    CORRADE_COMPARE(padding.data().size(), 0);
    CORRADE_COMPARE(padding.data().stride(), -35);
    CORRADE_VERIFY(!padding.data());
}

void MeshDataTest::constructAttributeNonOwningArray() {
    const MeshAttributeData data[3];
    Containers::Array<MeshAttributeData> array = meshAttributeDataNonOwningArray(data);
    CORRADE_COMPARE(array.size(), 3);
    CORRADE_COMPARE(static_cast<const void*>(array.data()), data);
}

void MeshDataTest::constructAttributeWrongStride() {
    char positionData[3*sizeof(Vector3)]{};

    std::ostringstream out;
    Error redirectError{&out};
    MeshAttributeData{MeshAttribute::Position, VertexFormat::Vector3, Containers::arrayCast<const char>(positionData)};
    MeshAttributeData{MeshAttribute::Position, VertexFormat::Vector3, Containers::StridedArrayView1D<const void>{positionData, 0, -16}};
    MeshAttributeData{MeshAttribute::Position, VertexFormat::Vector3, Containers::StridedArrayView1D<const void>{positionData, 0, 65000}};
    MeshAttributeData{65000};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshAttributeData: expected stride to be positive and enough to fit VertexFormat::Vector3, got 1\n"
        "Trade::MeshAttributeData: expected stride to be positive and at most 32k, got -16\n"
        "Trade::MeshAttributeData: expected stride to be positive and at most 32k, got 65000\n"
        "Trade::MeshAttributeData: at most 32k padding supported, got 65000\n"
    );
}

void MeshDataTest::construct() {
    struct Vertex {
        Vector3 position;
        Vector3 normal;
        Vector2 textureCoordinate;
        Short id;
    };

    Containers::Array<char> indexData{8*sizeof(UnsignedShort)};
    auto indexView = Containers::arrayCast<UnsignedShort>(indexData).slice(1, 7);
    indexView[0] = 0;
    indexView[1] = 1;
    indexView[2] = 2;
    indexView[3] = 0;
    indexView[4] = 2;
    indexView[5] = 1;

    Containers::Array<char> vertexData{3*sizeof(Vertex)};
    auto vertexView = Containers::arrayCast<Vertex>(vertexData);
    vertexView[0].position = {0.1f, 0.2f, 0.3f};
    vertexView[1].position = {0.4f, 0.5f, 0.6f};
    vertexView[2].position = {0.7f, 0.8f, 0.9f};
    vertexView[0].normal = Vector3::xAxis();
    vertexView[1].normal = Vector3::yAxis();
    vertexView[2].normal = Vector3::zAxis();
    vertexView[0].textureCoordinate = {0.000f, 0.125f};
    vertexView[1].textureCoordinate = {0.250f, 0.375f};
    vertexView[2].textureCoordinate = {0.500f, 0.625f};
    vertexView[0].id = 15;
    vertexView[1].id = -374;
    vertexView[2].id = 22;

    int importerState;
    MeshIndexData indices{indexView};
    MeshAttributeData positions{MeshAttribute::Position,
        Containers::StridedArrayView1D<Vector3>{vertexData, &vertexView[0].position, vertexView.size(), sizeof(Vertex)}};
    MeshAttributeData normals{MeshAttribute::Normal,
        Containers::StridedArrayView1D<Vector3>{vertexData, &vertexView[0].normal, vertexView.size(), sizeof(Vertex)}};
    MeshAttributeData textureCoordinates{MeshAttribute::TextureCoordinates,
        Containers::StridedArrayView1D<Vector2>{vertexData, &vertexView[0].textureCoordinate, vertexView.size(), sizeof(Vertex)}};
    MeshAttributeData ids{meshAttributeCustom(13),
        Containers::StridedArrayView1D<Short>{vertexData, &vertexView[0].id, vertexView.size(), sizeof(Vertex)}};
    MeshData data{MeshPrimitive::Triangles,
        std::move(indexData), indices,
        /* Texture coordinates deliberately twice (though aliased) */
        std::move(vertexData), {positions, textureCoordinates, normals, textureCoordinates, ids}, &importerState};

    /* Basics */
    CORRADE_COMPARE(data.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::Triangles);
    CORRADE_VERIFY(!data.attributeData().empty());
    CORRADE_COMPARE(static_cast<const void*>(data.indexData() + 2), indexView.data());
    CORRADE_COMPARE(static_cast<const void*>(data.vertexData()), vertexView.data());
    CORRADE_COMPARE(static_cast<void*>(data.mutableIndexData() + 2), indexView.data());
    CORRADE_COMPARE(static_cast<void*>(data.mutableVertexData()), vertexView.data());
    CORRADE_COMPARE(data.importerState(), &importerState);

    /* Index access */
    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 6);
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(data.indexOffset(), 2);

    /* Typeless index access with a cast later */
    CORRADE_COMPARE((Containers::arrayCast<1, const UnsignedShort>(data.indices())[1]), 1);
    CORRADE_COMPARE((Containers::arrayCast<1, const UnsignedShort>(data.indices())[3]), 0);
    CORRADE_COMPARE((Containers::arrayCast<1, const UnsignedShort>(data.indices())[4]), 2);

    /* Typed index access */
    CORRADE_COMPARE(data.indices<UnsignedShort>()[0], 0);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[2], 2);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[5], 1);

    /* Attribute access by ID */
    CORRADE_COMPARE(data.vertexCount(), 3);
    CORRADE_COMPARE(data.attributeCount(), 5);
    CORRADE_COMPARE(data.attributeName(0), MeshAttribute::Position);
    CORRADE_COMPARE(data.attributeName(1), MeshAttribute::TextureCoordinates);
    CORRADE_COMPARE(data.attributeName(2), MeshAttribute::Normal);
    CORRADE_COMPARE(data.attributeName(3), MeshAttribute::TextureCoordinates);
    CORRADE_COMPARE(data.attributeName(4), meshAttributeCustom(13));
    CORRADE_COMPARE(data.attributeFormat(0), VertexFormat::Vector3);
    CORRADE_COMPARE(data.attributeFormat(1), VertexFormat::Vector2);
    CORRADE_COMPARE(data.attributeFormat(2), VertexFormat::Vector3);
    CORRADE_COMPARE(data.attributeFormat(3), VertexFormat::Vector2);
    CORRADE_COMPARE(data.attributeFormat(4), VertexFormat::Short);
    CORRADE_COMPARE(data.attributeOffset(0), 0);
    CORRADE_COMPARE(data.attributeOffset(1), 2*sizeof(Vector3));
    CORRADE_COMPARE(data.attributeOffset(2), sizeof(Vector3));
    CORRADE_COMPARE(data.attributeOffset(3), 2*sizeof(Vector3));
    CORRADE_COMPARE(data.attributeOffset(4), 2*sizeof(Vector3) + sizeof(Vector2));
    CORRADE_COMPARE(data.attributeStride(0), sizeof(Vertex));
    CORRADE_COMPARE(data.attributeStride(1), sizeof(Vertex));
    CORRADE_COMPARE(data.attributeStride(2), sizeof(Vertex));
    CORRADE_COMPARE(data.attributeStride(3), sizeof(Vertex));

    /* Typeless access by ID with a cast later */
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector3>(
        data.attribute(0))[1]), (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector2>(
        data.attribute(1))[0]), (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector3>(
        data.attribute(2))[2]), Vector3::zAxis());
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector2>(
        data.attribute(3))[1]), (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE((Containers::arrayCast<1, const Short>(
        data.attribute(4))[0]), 15);
    CORRADE_COMPARE((Containers::arrayCast<1, Vector3>(
        data.mutableAttribute(0))[1]), (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE((Containers::arrayCast<1, Vector2>(
        data.mutableAttribute(1))[0]), (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE((Containers::arrayCast<1, Vector3>(
        data.mutableAttribute(2))[2]), Vector3::zAxis());
    CORRADE_COMPARE((Containers::arrayCast<1, Vector2>(
        data.mutableAttribute(3))[1]), (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE((Containers::arrayCast<1, Short>(
        data.mutableAttribute(4))[0]), 15);

    /* Typed access by ID */
    CORRADE_COMPARE(data.attribute<Vector3>(0)[1], (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE(data.attribute<Vector2>(1)[0], (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE(data.attribute<Vector3>(2)[2], Vector3::zAxis());
    CORRADE_COMPARE(data.attribute<Vector2>(3)[1], (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE(data.attribute<Short>(4)[1], -374);
    CORRADE_COMPARE(data.mutableAttribute<Vector3>(0)[1], (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE(data.mutableAttribute<Vector2>(1)[0], (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE(data.mutableAttribute<Vector3>(2)[2], Vector3::zAxis());
    CORRADE_COMPARE(data.mutableAttribute<Vector2>(3)[1], (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE(data.mutableAttribute<Short>(4)[1], -374);

    /* Attribute access by name */
    CORRADE_VERIFY(data.hasAttribute(MeshAttribute::Position));
    CORRADE_VERIFY(data.hasAttribute(MeshAttribute::Normal));
    CORRADE_VERIFY(data.hasAttribute(MeshAttribute::TextureCoordinates));
    CORRADE_VERIFY(data.hasAttribute(meshAttributeCustom(13)));
    CORRADE_VERIFY(!data.hasAttribute(MeshAttribute::Color));
    CORRADE_VERIFY(!data.hasAttribute(meshAttributeCustom(23)));
    CORRADE_COMPARE(data.attributeCount(MeshAttribute::Position), 1);
    CORRADE_COMPARE(data.attributeCount(MeshAttribute::Normal), 1);
    CORRADE_COMPARE(data.attributeCount(MeshAttribute::TextureCoordinates), 2);
    CORRADE_COMPARE(data.attributeCount(meshAttributeCustom(13)), 1);
    CORRADE_COMPARE(data.attributeCount(MeshAttribute::Color), 0);
    CORRADE_COMPARE(data.attributeCount(meshAttributeCustom(23)), 0);
    CORRADE_COMPARE(data.attributeId(MeshAttribute::Position), 0);
    CORRADE_COMPARE(data.attributeId(MeshAttribute::Normal), 2);
    CORRADE_COMPARE(data.attributeId(MeshAttribute::TextureCoordinates), 1);
    CORRADE_COMPARE(data.attributeId(MeshAttribute::TextureCoordinates, 1), 3);
    CORRADE_COMPARE(data.attributeId(meshAttributeCustom(13)), 4);
    CORRADE_COMPARE(data.attributeFormat(MeshAttribute::Position),
        VertexFormat::Vector3);
    CORRADE_COMPARE(data.attributeFormat(MeshAttribute::Normal),
        VertexFormat::Vector3);
    CORRADE_COMPARE(data.attributeFormat(MeshAttribute::TextureCoordinates, 0),
        VertexFormat::Vector2);
    CORRADE_COMPARE(data.attributeFormat(MeshAttribute::TextureCoordinates, 1),
        VertexFormat::Vector2);
    CORRADE_COMPARE(data.attributeFormat(meshAttributeCustom(13)),
        VertexFormat::Short);
    CORRADE_COMPARE(data.attributeOffset(MeshAttribute::Position), 0);
    CORRADE_COMPARE(data.attributeOffset(MeshAttribute::Normal), sizeof(Vector3));
    CORRADE_COMPARE(data.attributeOffset(MeshAttribute::TextureCoordinates, 0), 2*sizeof(Vector3));
    CORRADE_COMPARE(data.attributeOffset(MeshAttribute::TextureCoordinates, 1), 2*sizeof(Vector3));    CORRADE_COMPARE(data.attributeOffset(meshAttributeCustom(13)), 2*sizeof(Vector3) + sizeof(Vector2));
    CORRADE_COMPARE(data.attributeStride(MeshAttribute::Position), sizeof(Vertex));
    CORRADE_COMPARE(data.attributeStride(MeshAttribute::Normal), sizeof(Vertex));
    CORRADE_COMPARE(data.attributeStride(MeshAttribute::TextureCoordinates, 0), sizeof(Vertex));
    CORRADE_COMPARE(data.attributeStride(MeshAttribute::TextureCoordinates, 1), sizeof(Vertex));
    CORRADE_COMPARE(data.attributeStride(meshAttributeCustom(13)), sizeof(Vertex));

    /* Typeless access by name with a cast later */
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector3>(
        data.attribute(MeshAttribute::Position))[1]), (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector3>(
        data.attribute(MeshAttribute::Normal))[2]), Vector3::zAxis());
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector2>(
        data.attribute(MeshAttribute::TextureCoordinates, 0))[0]), (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector2>(
        data.attribute(MeshAttribute::TextureCoordinates, 1))[1]), (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE((Containers::arrayCast<1, const Short>(
        data.attribute(meshAttributeCustom(13)))[1]), -374);
    CORRADE_COMPARE((Containers::arrayCast<1, const Vector3>(
        data.mutableAttribute(MeshAttribute::Position))[1]), (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE((Containers::arrayCast<1, Vector3>(
        data.mutableAttribute(MeshAttribute::Normal))[2]), Vector3::zAxis());
    CORRADE_COMPARE((Containers::arrayCast<1, Vector2>(
        data.mutableAttribute(MeshAttribute::TextureCoordinates, 0))[0]), (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE((Containers::arrayCast<1, Vector2>(
        data.mutableAttribute(MeshAttribute::TextureCoordinates, 1))[1]), (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE((Containers::arrayCast<1, Short>(
        data.mutableAttribute(meshAttributeCustom(13)))[1]), -374);

    /* Typed access by name */
    CORRADE_COMPARE(data.attribute<Vector3>(MeshAttribute::Position)[1], (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE(data.attribute<Vector3>(MeshAttribute::Normal)[2], Vector3::zAxis());
    CORRADE_COMPARE(data.attribute<Vector2>(MeshAttribute::TextureCoordinates, 0)[0], (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE(data.attribute<Vector2>(MeshAttribute::TextureCoordinates, 1)[1], (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE(data.attribute<Short>(meshAttributeCustom(13))[2], 22);
    CORRADE_COMPARE(data.mutableAttribute<Vector3>(MeshAttribute::Position)[1], (Vector3{0.4f, 0.5f, 0.6f}));
    CORRADE_COMPARE(data.mutableAttribute<Vector3>(MeshAttribute::Normal)[2], Vector3::zAxis());
    CORRADE_COMPARE(data.mutableAttribute<Vector2>(MeshAttribute::TextureCoordinates, 0)[0], (Vector2{0.000f, 0.125f}));
    CORRADE_COMPARE(data.mutableAttribute<Vector2>(MeshAttribute::TextureCoordinates, 1)[1], (Vector2{0.250f, 0.375f}));
    CORRADE_COMPARE(data.attribute<Short>(meshAttributeCustom(13))[2], 22);
}

void MeshDataTest::constructZeroIndices() {
    /* This is a valid use case because this could be an empty slice of a
       well-defined indexed mesh */
    Containers::Array<char> vertexData{3*sizeof(Vector3)};
    auto vertexView = Containers::arrayCast<Vector3>(vertexData);
    MeshAttributeData positions{MeshAttribute::Position, vertexView};
    MeshData data{MeshPrimitive::Triangles,
        nullptr, MeshIndexData{MeshIndexType::UnsignedInt, nullptr},
        std::move(vertexData), {positions}};

    CORRADE_COMPARE(data.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_VERIFY(!data.indexData());
    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedInt);
    CORRADE_COMPARE(data.indexCount(), 0);
    CORRADE_COMPARE(data.vertexCount(), 3);
}

void MeshDataTest::constructZeroAttributes() {
    /* This is a valid use case because e.g. the index/vertex data can be
       shared by multiple meshes and this particular one is just a plain
       index array */
    Containers::Array<char> indexData{3*sizeof(UnsignedInt)};
    Containers::Array<char> vertexData{3};
    auto indexView = Containers::arrayCast<UnsignedInt>(indexData);
    MeshData data{MeshPrimitive::Triangles,
        std::move(indexData), MeshIndexData{indexView},
        std::move(vertexData), {}};

    CORRADE_COMPARE(data.indexCount(), 3);
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.attributeCount(), 0);
    CORRADE_VERIFY(!data.attributeData());
    CORRADE_COMPARE(data.vertexData().size(), 3);
    CORRADE_COMPARE(data.vertexCount(), 0);
}

void MeshDataTest::constructZeroVertices() {
    /* This is a valid use case because this could be an empty slice of a
       well-defined indexed mesh */
    Containers::Array<char> indexData{3*sizeof(UnsignedInt)};
    auto indexView = Containers::arrayCast<UnsignedInt>(indexData);
    MeshAttributeData positions{MeshAttribute::Position, VertexFormat::Vector3, nullptr};
    MeshData data{MeshPrimitive::Triangles,
        std::move(indexData), MeshIndexData{indexView},
        nullptr, {positions}};

    CORRADE_COMPARE(data.indexCount(), 3);
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.attributeCount(), 1);
    CORRADE_COMPARE(data.attributeName(0), MeshAttribute::Position);
    CORRADE_COMPARE(data.attributeFormat(0), VertexFormat::Vector3);
    CORRADE_COMPARE(data.attribute<Vector3>(0).size(), 0);
    CORRADE_VERIFY(!data.vertexData());
    CORRADE_COMPARE(data.vertexCount(), 0);
}

void MeshDataTest::constructIndexless() {
    Containers::Array<char> vertexData{3*sizeof(Vector2)};
    auto vertexView = Containers::arrayCast<Vector2>(vertexData);
    vertexView[0] = {0.1f, 0.2f};
    vertexView[1] = {0.4f, 0.5f};
    vertexView[2] = {0.7f, 0.8f};

    int importerState;
    MeshAttributeData positions{MeshAttribute::Position, vertexView};
    MeshData data{MeshPrimitive::LineLoop, std::move(vertexData), {positions}, &importerState};
    CORRADE_COMPARE(data.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    /* These are empty so it doesn't matter, but this is a nice non-restrictive
       default */
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::LineLoop);
    CORRADE_COMPARE(data.indexData(), nullptr);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(!data.isIndexed());
    CORRADE_COMPARE(data.vertexCount(), 3);
    CORRADE_COMPARE(data.attributeCount(), 1);
    CORRADE_COMPARE(data.attributeFormat(MeshAttribute::Position), VertexFormat::Vector2);
    CORRADE_COMPARE(data.attribute<Vector2>(MeshAttribute::Position)[1], (Vector2{0.4f, 0.5f}));
}

void MeshDataTest::constructIndexlessZeroVertices() {
    MeshAttributeData positions{MeshAttribute::Position, VertexFormat::Vector2, nullptr};
    MeshData data{MeshPrimitive::LineLoop, nullptr, {positions}};
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::LineLoop);
    CORRADE_COMPARE(data.indexData(), nullptr);
    CORRADE_COMPARE(data.vertexData(), nullptr);

    CORRADE_VERIFY(!data.isIndexed());
    CORRADE_COMPARE(data.vertexCount(), 0);
    CORRADE_COMPARE(data.attributeCount(), 1);
    CORRADE_COMPARE(data.attributeFormat(MeshAttribute::Position), VertexFormat::Vector2);
}

void MeshDataTest::constructAttributeless() {
    Containers::Array<char> indexData{6*sizeof(UnsignedInt)};
    auto indexView = Containers::arrayCast<UnsignedInt>(indexData);
    indexView[0] = 0;
    indexView[1] = 1;
    indexView[2] = 2;
    indexView[3] = 0;
    indexView[4] = 2;
    indexView[5] = 1;

    int importerState;
    MeshIndexData indices{indexView};
    MeshData data{MeshPrimitive::TriangleStrip, std::move(indexData), indices, &importerState};
    /* These are empty so it doesn't matter, but this is a nice non-restrictive
       default */
    CORRADE_COMPARE(data.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::TriangleStrip);
    CORRADE_VERIFY(!data.attributeData());
    CORRADE_COMPARE(data.vertexData(), nullptr);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 6);
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedInt);
    CORRADE_COMPARE(data.indices<UnsignedInt>()[0], 0);
    CORRADE_COMPARE(data.indices<UnsignedInt>()[2], 2);
    CORRADE_COMPARE(data.indices<UnsignedInt>()[5], 1);

    CORRADE_COMPARE(data.vertexCount(), 0); /** @todo what to return here? */
    CORRADE_COMPARE(data.attributeCount(), 0);
}

void MeshDataTest::constructNotOwned() {
    auto&& instanceData = NotOwnedData[testCaseInstanceId()];
    setTestCaseDescription(instanceData.name);

    UnsignedShort indexData[]{0, 1, 0};
    Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};

    int importerState;
    MeshIndexData indices{indexData};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};
    MeshData data{MeshPrimitive::Triangles, instanceData.indexDataFlags, Containers::arrayView(indexData), indices, instanceData.vertexDataFlags, Containers::arrayView(vertexData), {positions}, &importerState};

    CORRADE_COMPARE(data.indexDataFlags(), instanceData.indexDataFlags);
    CORRADE_COMPARE(data.vertexDataFlags(), instanceData.vertexDataFlags);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::Triangles);
    CORRADE_COMPARE(static_cast<const void*>(data.indexData()), +indexData);
    CORRADE_COMPARE(static_cast<const void*>(data.vertexData()), +vertexData);
    if(instanceData.indexDataFlags & DataFlag::Mutable)
        CORRADE_COMPARE(static_cast<const void*>(data.mutableIndexData()), +indexData);
    if(instanceData.vertexDataFlags & DataFlag::Mutable)
        CORRADE_COMPARE(static_cast<const void*>(data.mutableVertexData()), +vertexData);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 3);
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[1], 1);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[2], 0);
    if(instanceData.indexDataFlags & DataFlag::Mutable) {
        CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[1], 1);
        CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[2], 0);
    }

    CORRADE_COMPARE(data.vertexCount(), 2);
    CORRADE_COMPARE(data.attributeCount(), 1);
    CORRADE_COMPARE(data.attributeName(0), MeshAttribute::Position);
    CORRADE_COMPARE(data.attributeFormat(0), VertexFormat::Vector2);
    CORRADE_COMPARE(data.attributeOffset(0), 0);
    CORRADE_COMPARE(data.attributeStride(0), sizeof(Vector2));
    CORRADE_COMPARE(data.attribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
    CORRADE_COMPARE(data.attribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));
    if(instanceData.vertexDataFlags & DataFlag::Mutable) {
        CORRADE_COMPARE(data.mutableAttribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
        CORRADE_COMPARE(data.mutableAttribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));
    }
}

void MeshDataTest::constructIndicesNotOwned() {
    auto&& instanceData = SingleNotOwnedData[testCaseInstanceId()];
    setTestCaseDescription(instanceData.name);

    UnsignedShort indexData[]{0, 1, 0};
    Containers::Array<char> vertexData{2*sizeof(Vector2)};
    auto vertexView = Containers::arrayCast<Vector2>(vertexData);
    vertexView[0] = {0.1f, 0.2f};
    vertexView[1] = {0.4f, 0.5f};

    int importerState;
    MeshIndexData indices{indexData};
    MeshAttributeData positions{MeshAttribute::Position, vertexView};
    MeshData data{MeshPrimitive::Triangles, instanceData.dataFlags, Containers::arrayView(indexData), indices, std::move(vertexData), {positions}, &importerState};

    CORRADE_COMPARE(data.indexDataFlags(), instanceData.dataFlags);
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::Triangles);
    CORRADE_COMPARE(static_cast<const void*>(data.indexData()), +indexData);
    CORRADE_COMPARE(static_cast<const void*>(data.vertexData()), vertexView.data());
    if(instanceData.dataFlags & DataFlag::Mutable)
        CORRADE_COMPARE(static_cast<const void*>(data.mutableIndexData()), +indexData);
    CORRADE_COMPARE(static_cast<const void*>(data.mutableVertexData()), vertexView.data());
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 3);
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[1], 1);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[2], 0);
    if(instanceData.dataFlags & DataFlag::Mutable) {
        CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[1], 1);
        CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[2], 0);
    }

    CORRADE_COMPARE(data.vertexCount(), 2);
    CORRADE_COMPARE(data.attributeCount(), 1);
    CORRADE_COMPARE(data.attributeName(0), MeshAttribute::Position);
    CORRADE_COMPARE(data.attributeFormat(0), VertexFormat::Vector2);
    CORRADE_COMPARE(data.attributeOffset(0), 0);
    CORRADE_COMPARE(data.attributeStride(0), sizeof(Vector2));
    CORRADE_COMPARE(data.attribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
    CORRADE_COMPARE(data.attribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));
    CORRADE_COMPARE(data.mutableAttribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
    CORRADE_COMPARE(data.mutableAttribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));
}

void MeshDataTest::constructVerticesNotOwned() {
    auto&& instanceData = SingleNotOwnedData[testCaseInstanceId()];
    setTestCaseDescription(instanceData.name);

    Containers::Array<char> indexData{3*sizeof(UnsignedShort)};
    auto indexView = Containers::arrayCast<UnsignedShort>(indexData);
    indexView[0] = 0;
    indexView[1] = 1;
    indexView[2] = 0;
    Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};

    int importerState;
    MeshIndexData indices{indexView};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};
    MeshData data{MeshPrimitive::Triangles, std::move(indexData), indices, instanceData.dataFlags, Containers::arrayView(vertexData), {positions}, &importerState};

    CORRADE_COMPARE(data.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.vertexDataFlags(), instanceData.dataFlags);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::Triangles);
    CORRADE_COMPARE(static_cast<const void*>(data.indexData()), indexView.data());
    CORRADE_COMPARE(static_cast<const void*>(data.vertexData()), +vertexData);
    CORRADE_COMPARE(static_cast<const void*>(data.mutableIndexData()), indexView.data());
    if(instanceData.dataFlags & DataFlag::Mutable)
        CORRADE_COMPARE(static_cast<const void*>(data.mutableVertexData()), +vertexData);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 3);
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[1], 1);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[2], 0);
    CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[1], 1);
    CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[2], 0);

    CORRADE_COMPARE(data.vertexCount(), 2);
    CORRADE_COMPARE(data.attributeCount(), 1);
    CORRADE_COMPARE(data.attributeName(0), MeshAttribute::Position);
    CORRADE_COMPARE(data.attributeFormat(0), VertexFormat::Vector2);
    CORRADE_COMPARE(data.attributeOffset(0), 0);
    CORRADE_COMPARE(data.attributeStride(0), sizeof(Vector2));
    CORRADE_COMPARE(data.attribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
    CORRADE_COMPARE(data.attribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));
    if(instanceData.dataFlags & DataFlag::Mutable) {
        CORRADE_COMPARE(data.mutableAttribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
        CORRADE_COMPARE(data.mutableAttribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));
    }
}

void MeshDataTest::constructIndexlessNotOwned() {
    auto&& instanceData = SingleNotOwnedData[testCaseInstanceId()];
    setTestCaseDescription(instanceData.name);

    Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};

    int importerState;
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};
    MeshData data{MeshPrimitive::LineLoop, instanceData.dataFlags, vertexData, {positions}, &importerState};

    CORRADE_COMPARE(data.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.vertexDataFlags(), instanceData.dataFlags);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::LineLoop);
    CORRADE_COMPARE(data.indexData(), nullptr);
    if(instanceData.dataFlags & DataFlag::Mutable)
        CORRADE_COMPARE(data.mutableIndexData(), nullptr);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(!data.isIndexed());
    CORRADE_COMPARE(data.vertexCount(), 2);
    CORRADE_COMPARE(data.attributeCount(), 1);
    CORRADE_COMPARE(data.attributeFormat(MeshAttribute::Position), VertexFormat::Vector2);
    CORRADE_COMPARE(data.attribute<Vector2>(MeshAttribute::Position)[1], (Vector2{0.4f, 0.5f}));
    if(instanceData.dataFlags & DataFlag::Mutable)
        CORRADE_COMPARE(data.mutableAttribute<Vector2>(MeshAttribute::Position)[1], (Vector2{0.4f, 0.5f}));
}

void MeshDataTest::constructAttributelessNotOwned() {
    auto&& instanceData = SingleNotOwnedData[testCaseInstanceId()];
    setTestCaseDescription(instanceData.name);

    UnsignedShort indexData[]{0, 1, 0};

    int importerState;
    MeshIndexData indices{indexData};
    MeshData data{MeshPrimitive::TriangleStrip, instanceData.dataFlags, indexData, indices, &importerState};
    CORRADE_COMPARE(data.indexDataFlags(), instanceData.dataFlags);
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::TriangleStrip);
    CORRADE_VERIFY(!data.attributeData());
    CORRADE_COMPARE(data.vertexData(), nullptr);
    if(instanceData.dataFlags & DataFlag::Mutable)
        CORRADE_COMPARE(data.mutableVertexData(), nullptr);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 3);
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[0], 0);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[1], 1);
    CORRADE_COMPARE(data.indices<UnsignedShort>()[2], 0);
    if(instanceData.dataFlags & DataFlag::Mutable) {
        CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[0], 0);
        CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[1], 1);
        CORRADE_COMPARE(data.mutableIndices<UnsignedShort>()[2], 0);
    }

    CORRADE_COMPARE(data.vertexCount(), 0); /** @todo what to return here? */
    CORRADE_COMPARE(data.attributeCount(), 0);
}

void MeshDataTest::constructIndexlessAttributeless() {
    int importerState;
    MeshData data{MeshPrimitive::TriangleStrip, 37, &importerState};
    /* These are both empty so it doesn't matter, but this is a nice
       non-restrictive default */
    CORRADE_COMPARE(data.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::TriangleStrip);
    CORRADE_VERIFY(!data.attributeData());
    CORRADE_COMPARE(data.indexData(), nullptr);
    CORRADE_COMPARE(data.vertexData(), nullptr);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(!data.isIndexed());
    CORRADE_COMPARE(data.vertexCount(), 37);
    CORRADE_COMPARE(data.attributeCount(), 0);
}

void MeshDataTest::constructIndexlessAttributelessZeroVertices() {
    int importerState;
    MeshData data{MeshPrimitive::TriangleStrip, 0, &importerState};
    CORRADE_COMPARE(data.primitive(), MeshPrimitive::TriangleStrip);
    CORRADE_VERIFY(!data.attributeData());
    CORRADE_COMPARE(data.indexData(), nullptr);
    CORRADE_COMPARE(data.vertexData(), nullptr);
    CORRADE_COMPARE(data.importerState(), &importerState);

    CORRADE_VERIFY(!data.isIndexed());
    CORRADE_COMPARE(data.vertexCount(), 0);
    CORRADE_COMPARE(data.attributeCount(), 0);
}

void MeshDataTest::constructIndexDataButNotIndexed() {
    Containers::Array<char> indexData{6};

    std::ostringstream out;
    Error redirectError{&out};
    MeshAttributeData positions{MeshAttribute::Position, VertexFormat::Vector2, nullptr};
    MeshData{MeshPrimitive::Points, std::move(indexData), MeshIndexData{}, nullptr, {positions}};
    CORRADE_COMPARE(out.str(), "Trade::MeshData: indexData passed for a non-indexed mesh\n");
}

void MeshDataTest::constructAttributelessInvalidIndices() {
    std::ostringstream out;
    Error redirectError{&out};
    MeshData{MeshPrimitive::Points, nullptr, MeshIndexData{}};
    CORRADE_COMPARE(out.str(), "Trade::MeshData: indices are expected to be valid if there are no attributes and vertex count isn't passed explicitly\n");
}

void MeshDataTest::constructIndicesNotContained() {
    Containers::Array<char> indexData{reinterpret_cast<char*>(0xbadda9), 6, [](char*, std::size_t){}};
    Containers::ArrayView<UnsignedShort> indexData2{reinterpret_cast<UnsignedShort*>(0xdead), 3};
    MeshIndexData indices{indexData2};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData{MeshPrimitive::Triangles, std::move(indexData), indices};
    MeshData{MeshPrimitive::Triangles, nullptr, indices};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: indices [0xdead:0xdeb3] are not contained in passed indexData array [0xbadda9:0xbaddaf]\n"
        "Trade::MeshData: indices [0xdead:0xdeb3] are not contained in passed indexData array [0x0:0x0]\n");
}

void MeshDataTest::constructAttributeNotContained() {
    Containers::Array<char> vertexData{reinterpret_cast<char*>(0xbadda9), 24, [](char*, std::size_t){}};
    Containers::ArrayView<Vector2> vertexData2{reinterpret_cast<Vector2*>(0xdead), 3};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayCast<Vector2>(vertexData)};
    MeshAttributeData positions2{MeshAttribute::Position, Containers::arrayView(vertexData2)};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData{MeshPrimitive::Triangles, std::move(vertexData), {positions, positions2}};
    MeshData{MeshPrimitive::Triangles, nullptr, {positions}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: attribute 1 [0xdead:0xdec5] is not contained in passed vertexData array [0xbadda9:0xbaddc1]\n"
        "Trade::MeshData: attribute 0 [0xbadda9:0xbaddc1] is not contained in passed vertexData array [0x0:0x0]\n");
}

void MeshDataTest::constructInconsitentVertexCount() {
    Containers::Array<char> vertexData{24};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayCast<Vector2>(vertexData)};
    MeshAttributeData positions2{MeshAttribute::Position, Containers::arrayCast<Vector2>(vertexData).prefix(2)};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData{MeshPrimitive::Triangles, std::move(vertexData), {positions, positions2}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: attribute 1 has 2 vertices but 3 expected\n");
}

void MeshDataTest::constructNotOwnedIndexFlagOwned() {
    const UnsignedShort indexData[]{0, 1, 0};
    const Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};

    MeshIndexData indices{indexData};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData data{MeshPrimitive::Triangles, DataFlag::Owned, indexData, indices, {}, vertexData, {positions}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: can't construct with non-owned index data but Trade::DataFlag::Owned\n");
}

void MeshDataTest::constructNotOwnedVertexFlagOwned() {
    const UnsignedShort indexData[]{0, 1, 0};
    const Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};

    MeshIndexData indices{indexData};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData data{MeshPrimitive::Triangles, {}, indexData, indices, DataFlag::Owned, vertexData, {positions}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: can't construct with non-owned vertex data but Trade::DataFlag::Owned\n");
}

void MeshDataTest::constructIndicesNotOwnedFlagOwned() {
    UnsignedShort indexData[]{0, 1, 0};
    Containers::Array<char> vertexData{2*sizeof(Vector2)};
    auto vertexView = Containers::arrayCast<Vector2>(vertexData);
    vertexView[0] = {0.1f, 0.2f};
    vertexView[1] = {0.4f, 0.5f};

    MeshIndexData indices{indexData};
    MeshAttributeData positions{MeshAttribute::Position, vertexView};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData data{MeshPrimitive::Triangles, DataFlag::Owned, indexData, indices, std::move(vertexData), {positions}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: can't construct with non-owned index data but Trade::DataFlag::Owned\n");
}

void MeshDataTest::constructVerticesNotOwnedFlagOwned() {
    Containers::Array<char> indexData{3*sizeof(UnsignedShort)};
    auto indexView = Containers::arrayCast<UnsignedShort>(indexData);
    indexView[0] = 0;
    indexView[1] = 1;
    indexView[2] = 0;
    Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};

    MeshIndexData indices{indexView};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData data{MeshPrimitive::Triangles, std::move(indexData), indices, DataFlag::Owned, vertexData, {positions}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: can't construct with non-owned vertex data but Trade::DataFlag::Owned\n");
}

void MeshDataTest::constructIndexlessNotOwnedFlagOwned() {
    const Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData data{MeshPrimitive::Triangles, DataFlag::Owned, vertexData, {positions}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: can't construct with non-owned vertex data but Trade::DataFlag::Owned\n");
}

void MeshDataTest::constructAttributelessNotOwnedFlagOwned() {
    const UnsignedShort indexData[]{0, 1, 0};
    MeshIndexData indices{indexData};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData data{MeshPrimitive::Triangles, DataFlag::Owned, indexData, indices};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: can't construct with non-owned index data but Trade::DataFlag::Owned\n");
}

void MeshDataTest::constructInvalidAttributeData() {
    MeshAttributeData a;
    MeshAttributeData b{3};

    std::ostringstream out;
    Error redirectError{&out};
    MeshData{MeshPrimitive::Triangles, nullptr, {a}};
    MeshData{MeshPrimitive::Triangles, nullptr, {b}};
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData: attribute 0 doesn't specify anything\n"
        "Trade::MeshData: attribute 0 doesn't specify anything\n");
}

void MeshDataTest::constructCopy() {
    CORRADE_VERIFY(!(std::is_constructible<MeshData, const MeshData&>{}));
    CORRADE_VERIFY(!(std::is_assignable<MeshData, const MeshData&>{}));
}

void MeshDataTest::constructMove() {
    Containers::Array<char> indexData{3*sizeof(UnsignedShort)};
    auto indexView = Containers::arrayCast<UnsignedShort>(indexData);
    indexView[0] = 0;
    indexView[1] = 1;
    indexView[2] = 0;

    Containers::Array<char> vertexData{2*sizeof(Vector2)};
    auto vertexView = Containers::arrayCast<Vector2>(vertexData);
    vertexView[0] = {0.1f, 0.2f};
    vertexView[1] = {0.4f, 0.5f};

    int importerState;
    MeshIndexData indices{indexView};
    MeshAttributeData positions{MeshAttribute::Position, vertexView};
    MeshData a{MeshPrimitive::Triangles, std::move(indexData), indices, std::move(vertexData), {positions}, &importerState};

    MeshData b{std::move(a)};

    CORRADE_COMPARE(b.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(b.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(b.primitive(), MeshPrimitive::Triangles);
    CORRADE_COMPARE(static_cast<const void*>(b.indexData()), indexView.data());
    CORRADE_COMPARE(static_cast<const void*>(b.vertexData()), vertexView.data());
    CORRADE_COMPARE(b.importerState(), &importerState);

    CORRADE_VERIFY(b.isIndexed());
    CORRADE_COMPARE(b.indexCount(), 3);
    CORRADE_COMPARE(b.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(b.indices<UnsignedShort>()[1], 1);
    CORRADE_COMPARE(b.indices<UnsignedShort>()[2], 0);

    CORRADE_COMPARE(b.vertexCount(), 2);
    CORRADE_COMPARE(b.attributeCount(), 1);
    CORRADE_COMPARE(b.attributeName(0), MeshAttribute::Position);
    CORRADE_COMPARE(b.attributeFormat(0), VertexFormat::Vector2);
    CORRADE_COMPARE(b.attributeOffset(0), 0);
    CORRADE_COMPARE(b.attributeStride(0), sizeof(Vector2));
    CORRADE_COMPARE(b.attribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
    CORRADE_COMPARE(b.attribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));

    MeshData c{MeshPrimitive::LineLoop, 37};
    c = std::move(b);

    CORRADE_COMPARE(c.indexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(c.vertexDataFlags(), DataFlag::Owned|DataFlag::Mutable);
    CORRADE_COMPARE(c.primitive(), MeshPrimitive::Triangles);
    CORRADE_COMPARE(static_cast<const void*>(c.indexData()), indexView.data());
    CORRADE_COMPARE(static_cast<const void*>(c.vertexData()), vertexView.data());
    CORRADE_COMPARE(c.importerState(), &importerState);

    CORRADE_VERIFY(c.isIndexed());
    CORRADE_COMPARE(c.indexCount(), 3);
    CORRADE_COMPARE(c.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(c.indices<UnsignedShort>()[1], 1);
    CORRADE_COMPARE(c.indices<UnsignedShort>()[2], 0);

    CORRADE_COMPARE(c.vertexCount(), 2);
    CORRADE_COMPARE(c.attributeCount(), 1);
    CORRADE_COMPARE(c.attributeName(0), MeshAttribute::Position);
    CORRADE_COMPARE(c.attributeFormat(0), VertexFormat::Vector2);
    CORRADE_COMPARE(c.attributeOffset(0), 0);
    CORRADE_COMPARE(c.attributeStride(0), sizeof(Vector2));
    CORRADE_COMPARE(c.attribute<Vector2>(0)[0], (Vector2{0.1f, 0.2f}));
    CORRADE_COMPARE(c.attribute<Vector2>(0)[1], (Vector2{0.4f, 0.5f}));

    CORRADE_VERIFY(std::is_nothrow_move_constructible<MeshData>::value);
    CORRADE_VERIFY(std::is_nothrow_move_assignable<MeshData>::value);
}

template<class> struct NameTraits;
#define _c(format) template<> struct NameTraits<format> {                   \
        static const char* name() { return #format; }                       \
    };
_c(Vector2)
_c(Vector2h)
_c(Vector2ub)
_c(Vector2b)
_c(Vector2us)
_c(Vector2s)
_c(Vector3)
_c(Vector3h)
_c(Vector3ub)
_c(Vector3b)
_c(Vector3us)
_c(Vector3s)
_c(Color3)
_c(Color3h)
_c(Color3ub)
_c(Color3us)
_c(Color4)
_c(Color4h)
_c(Color4ub)
_c(Color4us)
#undef _c

template<class T> void MeshDataTest::indicesAsArray() {
    setTestCaseTemplateName(Math::TypeTraits<T>::name());

    Containers::Array<char> indexData{3*sizeof(T)};
    auto indexView = Containers::arrayCast<T>(indexData);
    indexView[0] = 75;
    indexView[1] = 131;
    indexView[2] = 240;

    MeshData data{MeshPrimitive::Points, std::move(indexData), MeshIndexData{indexView}};
    CORRADE_COMPARE_AS(data.indicesAsArray(),
        Containers::arrayView<UnsignedInt>({75, 131, 240}),
        TestSuite::Compare::Container);
}

void MeshDataTest::indicesIntoArrayInvalidSize() {
    Containers::Array<char> indexData{3*sizeof(UnsignedInt)};
    MeshData data{MeshPrimitive::Points, std::move(indexData), MeshIndexData{Containers::arrayCast<UnsignedInt>(indexData)}};

    std::ostringstream out;
    Error redirectError{&out};
    UnsignedInt destination[2];
    data.indicesInto(destination);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::indicesInto(): expected a view with 3 elements but got 2\n");
}

template<class T> void MeshDataTest::positions2DAsArray() {
    setTestCaseTemplateName(NameTraits<T>::name());
    typedef typename T::Type U;

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector2<U>{U(2.0f), U(1.0f)});
    positionsView[1] = T::pad(Math::Vector2<U>{U(0.0f), U(-1.0f)});
    positionsView[2] = T::pad(Math::Vector2<U>{U(-2.0f), U(3.0f)});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, positionsView}}};
    CORRADE_COMPARE_AS(data.positions2DAsArray(),
        Containers::arrayView<Vector2>({{2.0f, 1.0f}, {0.0f, -1.0f}, {-2.0f, 3.0f}}),
        TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions2DAsArrayPackedUnsigned() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector2<typename T::Type>{2, 1});
    positionsView[1] = T::pad(Math::Vector2<typename T::Type>{0, 15});
    positionsView[2] = T::pad(Math::Vector2<typename T::Type>{22, 3});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, positionsView}}};
    CORRADE_COMPARE_AS(data.positions2DAsArray(),
        Containers::arrayView<Vector2>({{2.0f, 1.0f}, {0.0f, 15.0f}, {22.0f, 3.0f}}),
        TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions2DAsArrayPackedSigned() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector2<typename T::Type>{2, 1});
    positionsView[1] = T::pad(Math::Vector2<typename T::Type>{0, -15});
    positionsView[2] = T::pad(Math::Vector2<typename T::Type>{-22, 3});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, positionsView}}};
    CORRADE_COMPARE_AS(data.positions2DAsArray(),
        Containers::arrayView<Vector2>({{2.0f, 1.0f}, {0.0f, -15.0f}, {-22.0f, 3.0f}}),
        TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions2DAsArrayPackedUnsignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector2<typename T::Type>{Math::pack<typename T::Type>(1.0f), 0});
    positionsView[1] = T::pad(Math::Vector2<typename T::Type>{0, Math::pack<typename T::Type>(1.0f)});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position,
        /* Assuming the normalized enum is always after the non-normalized */
        VertexFormat(UnsignedInt(Implementation::vertexFormatFor<T>()) + 1),
        positionsView}}};
    CORRADE_COMPARE_AS(data.positions2DAsArray(),
        Containers::arrayView<Vector2>({{1.0f, 0.0f}, {0.0f, 1.0f}}),
        TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions2DAsArrayPackedSignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector2<typename T::Type>{Math::pack<typename T::Type>(1.0f), 0});
    positionsView[1] = T::pad(Math::Vector2<typename T::Type>{0, Math::pack<typename T::Type>(-1.0f)});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position,
        /* Assuming the normalized enum is always after the non-normalized */
        VertexFormat(UnsignedInt(Implementation::vertexFormatFor<T>()) + 1),
        positionsView}}};
    CORRADE_COMPARE_AS(data.positions2DAsArray(),
        Containers::arrayView<Vector2>({{1.0f, 0.0f}, {0.0f, -1.0f}}),
        TestSuite::Compare::Container);
}

void MeshDataTest::positions2DIntoArrayInvalidSize() {
    Containers::Array<char> vertexData{3*sizeof(Vector2)};
    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, Containers::arrayCast<Vector2>(vertexData)}}};

    std::ostringstream out;
    Error redirectError{&out};
    Vector2 destination[2];
    data.positions2DInto(destination);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::positions2DInto(): expected a view with 3 elements but got 2\n");
}

template<class T> void MeshDataTest::positions3DAsArray() {
    setTestCaseTemplateName(NameTraits<T>::name());
    typedef typename T::Type U;

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    /* Needs to be sufficiently representable to have the test work also for
       half floats */
    positionsView[0] = T::pad(Math::Vector3<U>{U(2.0f), U(1.0f), U(0.75f)});
    positionsView[1] = T::pad(Math::Vector3<U>{U(0.0f), U(-1.0f), U(1.25f)});
    positionsView[2] = T::pad(Math::Vector3<U>{U(-2.0f), U(3.0f), U(2.5f)});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, positionsView}}};
    CORRADE_COMPARE_AS(data.positions3DAsArray(), Containers::arrayView<Vector3>({
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{2.0f, 1.0f, 0.75f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{0.0f, -1.0f, 1.25f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{-2.0f, 3.0f, 2.5f}))
    }), TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions3DAsArrayPackedUnsigned() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector3<typename T::Type>{2, 1, 135});
    positionsView[1] = T::pad(Math::Vector3<typename T::Type>{0, 15, 2});
    positionsView[2] = T::pad(Math::Vector3<typename T::Type>{22, 3, 192});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, positionsView}}};
    CORRADE_COMPARE_AS(data.positions3DAsArray(), Containers::arrayView<Vector3>({
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{2.0f, 1.0f, 135.0f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{0.0f, 15.0f, 2.0f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{22.0f, 3.0f, 192.0f}))
    }), TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions3DAsArrayPackedSigned() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector3<typename T::Type>{2, 1, -117});
    positionsView[1] = T::pad(Math::Vector3<typename T::Type>{0, -15, 2});
    positionsView[2] = T::pad(Math::Vector3<typename T::Type>{-22, 3, 86});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, positionsView}}};
    CORRADE_COMPARE_AS(data.positions3DAsArray(), Containers::arrayView<Vector3>({
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{2.0f, 1.0f, -117.0f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{0.0f, -15.0f, 2.0f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{-22.0f, 3.0f, 86.0f}))
    }), TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions3DAsArrayPackedUnsignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector3<typename T::Type>{Math::pack<typename T::Type>(1.0f), 0, Math::pack<typename T::Type>(1.0f)});
    positionsView[1] = T::pad(Math::Vector3<typename T::Type>{0, Math::pack<typename T::Type>(1.0f), 0});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position,
        /* Assuming the normalized enum is always after the non-normalized */
        VertexFormat(UnsignedInt(Implementation::vertexFormatFor<T>()) + 1),
        positionsView}}};
    CORRADE_COMPARE_AS(data.positions3DAsArray(), Containers::arrayView<Vector3>({
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{1.0f, 0.0f, 1.0f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{0.0f, 1.0f, 0.0f}))
    }), TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::positions3DAsArrayPackedSignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto positionsView = Containers::arrayCast<T>(vertexData);
    positionsView[0] = T::pad(Math::Vector3<typename T::Type>{Math::pack<typename T::Type>(1.0f), 0, Math::pack<typename T::Type>(1.0f)});
    positionsView[1] = T::pad(Math::Vector3<typename T::Type>{0, Math::pack<typename T::Type>(-1.0f), 0});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position,
        /* Assuming the normalized enum is always after the non-normalized */
        VertexFormat(UnsignedInt(Implementation::vertexFormatFor<T>()) + 1),
        positionsView}}};
    CORRADE_COMPARE_AS(data.positions3DAsArray(), Containers::arrayView<Vector3>({
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{1.0f, 0.0f, 1.0f})),
        Vector3::pad(Math::Vector<T::Size, Float>::pad(Vector3{0.0f, -1.0f, 0.0f}))
    }), TestSuite::Compare::Container);
}

void MeshDataTest::positions3DIntoArrayInvalidSize() {
    Containers::Array<char> vertexData{3*sizeof(Vector3)};
    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Position, Containers::arrayCast<Vector3>(vertexData)}}};

    std::ostringstream out;
    Error redirectError{&out};
    Vector3 destination[2];
    data.positions3DInto(destination);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::positions3DInto(): expected a view with 3 elements but got 2\n");
}

template<class T> void MeshDataTest::normalsAsArray() {
    setTestCaseTemplateName(NameTraits<T>::name());
    typedef typename T::Type U;

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto normalsView = Containers::arrayCast<T>(vertexData);
    /* Needs to be sufficiently representable to have the test work also for
       half floats */
    normalsView[0] = {U(2.0f), U(1.0f), U(0.75f)};
    normalsView[1] = {U(0.0f), U(-1.0f), U(1.25f)};
    normalsView[2] = {U(-2.0f), U(3.0f), U(2.5f)};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Normal, normalsView}}};
    CORRADE_COMPARE_AS(data.normalsAsArray(), Containers::arrayView<Vector3>({
        {2.0f, 1.0f, 0.75f}, {0.0f, -1.0f, 1.25f}, {-2.0f, 3.0f, 2.5f},
    }), TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::normalsAsArrayPackedSignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto normalsView = Containers::arrayCast<T>(vertexData);
    normalsView[0] = {Math::pack<typename T::Type>(1.0f), 0, Math::pack<typename T::Type>(1.0f)};
    normalsView[1] = {0, Math::pack<typename T::Type>(-1.0f), 0};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Normal,
        /* Assuming the normalized enum is always after the non-normalized */
        VertexFormat(UnsignedInt(Implementation::vertexFormatFor<T>()) + 1),
        normalsView}}};
    CORRADE_COMPARE_AS(data.normalsAsArray(), Containers::arrayView<Vector3>({
        {1.0f, 0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}
    }), TestSuite::Compare::Container);
}

void MeshDataTest::normalsIntoArrayInvalidSize() {
    Containers::Array<char> vertexData{3*sizeof(Vector3)};
    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Normal, Containers::arrayCast<Vector3>(vertexData)}}};

    std::ostringstream out;
    Error redirectError{&out};
    Vector3 destination[2];
    data.normalsInto(destination);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::normalsInto(): expected a view with 3 elements but got 2\n");
}

template<class T> void MeshDataTest::textureCoordinates2DAsArray() {
    setTestCaseTemplateName(NameTraits<T>::name());
    typedef typename T::Type U;

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto textureCoordinatesView = Containers::arrayCast<T>(vertexData);
    textureCoordinatesView[0] = {U(2.0f), U(1.0f)};
    textureCoordinatesView[1] = {U(0.0f), U(-1.0f)};
    textureCoordinatesView[2] = {U(-2.0f), U(3.0f)};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::TextureCoordinates, textureCoordinatesView}}};
    CORRADE_COMPARE_AS(data.textureCoordinates2DAsArray(), Containers::arrayView<Vector2>({
        {2.0f, 1.0f}, {0.0f, -1.0f}, {-2.0f, 3.0f},
    }), TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::textureCoordinates2DAsArrayPackedUnsigned() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto textureCoordinatesView = Containers::arrayCast<T>(vertexData);
    textureCoordinatesView[0] = {2, 1};
    textureCoordinatesView[1] = {0, 15};
    textureCoordinatesView[2] = {22, 3};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::TextureCoordinates, textureCoordinatesView}}};
    CORRADE_COMPARE_AS(data.textureCoordinates2DAsArray(),
        Containers::arrayView<Vector2>({{2.0f, 1.0f}, {0.0f, 15.0f}, {22.0f, 3.0f}}),
        TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::textureCoordinates2DAsArrayPackedSigned() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto textureCoordinatesView = Containers::arrayCast<T>(vertexData);
    textureCoordinatesView[0] = {2, 1};
    textureCoordinatesView[1] = {0, -15};
    textureCoordinatesView[2] = {-22, 3};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::TextureCoordinates, textureCoordinatesView}}};
    CORRADE_COMPARE_AS(data.textureCoordinates2DAsArray(),
        Containers::arrayView<Vector2>({{2.0f, 1.0f}, {0.0f, -15.0f}, {-22.0f, 3.0f}}),
        TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::textureCoordinates2DAsArrayPackedUnsignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto textureCoordinatesView = Containers::arrayCast<T>(vertexData);
    textureCoordinatesView[0] = {Math::pack<typename T::Type>(1.0f), 0};
    textureCoordinatesView[1] = {0, Math::pack<typename T::Type>(1.0f)};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::TextureCoordinates,
        /* Assuming the normalized enum is always after the non-normalized */
        VertexFormat(UnsignedInt(Implementation::vertexFormatFor<T>()) + 1),
        textureCoordinatesView}}};
    CORRADE_COMPARE_AS(data.textureCoordinates2DAsArray(),
        Containers::arrayView<Vector2>({{1.0f, 0.0f}, {0.0f, 1.0f}}),
        TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::textureCoordinates2DAsArrayPackedSignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto textureCoordinatesView = Containers::arrayCast<T>(vertexData);
    textureCoordinatesView[0] = {Math::pack<typename T::Type>(1.0f), 0};
    textureCoordinatesView[1] = {0, Math::pack<typename T::Type>(-1.0f)};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::TextureCoordinates,
        /* Assuming the normalized enum is always after the non-normalized */
        VertexFormat(UnsignedInt(Implementation::vertexFormatFor<T>()) + 1),
        textureCoordinatesView}}};
    CORRADE_COMPARE_AS(data.textureCoordinates2DAsArray(),
        Containers::arrayView<Vector2>({{1.0f, 0.0f}, {0.0f, -1.0f}}),
        TestSuite::Compare::Container);
}

void MeshDataTest::textureCoordinates2DIntoArrayInvalidSize() {
    Containers::Array<char> vertexData{3*sizeof(Vector2)};
    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::TextureCoordinates, Containers::arrayCast<Vector2>(vertexData)}}};

    std::ostringstream out;
    Error redirectError{&out};
    Vector2 destination[2];
    data.textureCoordinates2DInto(destination);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::textureCoordinates2DInto(): expected a view with 3 elements but got 2\n");
}

template<class T> void MeshDataTest::colorsAsArray() {
    setTestCaseTemplateName(NameTraits<T>::name());
    typedef typename T::Type U;

    Containers::Array<char> vertexData{3*sizeof(T)};
    auto colorsView = Containers::arrayCast<T>(vertexData);
    /* Can't use e.g. 0xff3366_rgbf because that's not representable in
       half-floats */
    colorsView[0] = {U(2.0f), U(1.0f), U(0.75f)};
    colorsView[1] = {U(0.0f), U(-1.0f), U(1.25f)};
    colorsView[2] = {U(-2.0f), U(3.0f), U(2.5f)};

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Color, colorsView}}};
    CORRADE_COMPARE_AS(data.colorsAsArray(), Containers::arrayView<Color4>({
        {2.0f, 1.0f, 0.75f}, {0.0f, -1.0f, 1.25f}, {-2.0f, 3.0f, 2.5f},
    }), TestSuite::Compare::Container);
}

template<class T> void MeshDataTest::colorsAsArrayPackedUnsignedNormalized() {
    setTestCaseTemplateName(NameTraits<T>::name());

    Containers::Array<char> vertexData{2*sizeof(T)};
    auto colorsView = Containers::arrayCast<T>(vertexData);
    colorsView[0] = T::pad(Math::Color4<typename T::Type>{Math::pack<typename T::Type>(1.0f), 0, Math::pack<typename T::Type>(1.0f), 0});
    colorsView[1] = T::pad(Math::Color4<typename T::Type>{0, Math::pack<typename T::Type>(1.0f), 0, Math::pack<typename T::Type>(1.0f)});

    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Color, colorsView}}};
    CORRADE_COMPARE_AS(data.colorsAsArray(), Containers::arrayView<Color4>({
        Color4::pad(Math::Vector<T::Size, Float>::pad(Vector4{1.0f, 0.0f, 1.0f, 0.0f}), 1.0f),
        Color4::pad(Math::Vector<T::Size, Float>::pad(Vector4{0.0f, 1.0f, 0.0f, 1.0f}), 1.0f)
    }), TestSuite::Compare::Container);
}

void MeshDataTest::colorsIntoArrayInvalidSize() {
    Containers::Array<char> vertexData{3*sizeof(Color4)};
    MeshData data{MeshPrimitive::Points, std::move(vertexData), {MeshAttributeData{MeshAttribute::Color, Containers::arrayCast<Color4>(vertexData)}}};

    std::ostringstream out;
    Error redirectError{&out};
    Color4 destination[2];
    data.colorsInto(destination);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::colorsInto(): expected a view with 3 elements but got 2\n");
}

void MeshDataTest::mutableAccessNotAllowed() {
    const UnsignedShort indexData[]{0, 1, 0};
    const Vector2 vertexData[]{{0.1f, 0.2f}, {0.4f, 0.5f}};

    MeshIndexData indices{indexData};
    MeshAttributeData positions{MeshAttribute::Position, Containers::arrayView(vertexData)};
    MeshData data{MeshPrimitive::Triangles, {}, indexData, indices, {}, vertexData, {positions}};
    CORRADE_COMPARE(data.indexDataFlags(), DataFlags{});
    CORRADE_COMPARE(data.vertexDataFlags(), DataFlags{});

    std::ostringstream out;
    Error redirectError{&out};
    data.mutableIndexData();
    data.mutableVertexData();
    data.mutableIndices();
    data.mutableIndices<UnsignedShort>();
    data.mutableAttribute(0);
    data.mutableAttribute<Vector2>(0);
    data.mutableAttribute(MeshAttribute::Position);
    data.mutableAttribute<Vector2>(MeshAttribute::Position);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::mutableIndexData(): index data not mutable\n"
        "Trade::MeshData::mutableVertexData(): vertex data not mutable\n"
        "Trade::MeshData::mutableIndices(): index data not mutable\n"
        "Trade::MeshData::mutableIndices(): index data not mutable\n"
        "Trade::MeshData::mutableAttribute(): vertex data not mutable\n"
        "Trade::MeshData::mutableAttribute(): vertex data not mutable\n"
        "Trade::MeshData::mutableAttribute(): vertex data not mutable\n"
        "Trade::MeshData::mutableAttribute(): vertex data not mutable\n");
}

void MeshDataTest::indicesNotIndexed() {
    MeshData data{MeshPrimitive::Triangles, 37};

    std::ostringstream out;
    Error redirectError{&out};
    data.indexCount();
    data.indexType();
    data.indexOffset();
    data.indices();
    data.indices<UnsignedInt>();
    data.indicesAsArray();
    UnsignedInt a[1];
    data.indicesInto(a);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::indexCount(): the mesh is not indexed\n"
        "Trade::MeshData::indexType(): the mesh is not indexed\n"
        "Trade::MeshData::indexOffset(): the mesh is not indexed\n"
        "Trade::MeshData::indices(): the mesh is not indexed\n"
        "Trade::MeshData::indices(): the mesh is not indexed\n"
        "Trade::MeshData::indicesAsArray(): the mesh is not indexed\n"
        "Trade::MeshData::indicesInto(): the mesh is not indexed\n");
}

void MeshDataTest::indicesWrongType() {
    Containers::Array<char> indexData{sizeof(UnsignedShort)};
    auto indexView = Containers::arrayCast<UnsignedShort>(indexData);
    indexView[0] = 57616;
    MeshData data{MeshPrimitive::Points, std::move(indexData), MeshIndexData{indexView}};

    std::ostringstream out;
    Error redirectError{&out};
    data.indices<UnsignedByte>();
    CORRADE_COMPARE(out.str(), "Trade::MeshData::indices(): improper type requested for MeshIndexType::UnsignedShort\n");
}

void MeshDataTest::attributeNotFound() {
    MeshAttributeData colors1{MeshAttribute::Color, VertexFormat::Vector3, nullptr};
    MeshAttributeData colors2{MeshAttribute::Color, VertexFormat::Vector4, nullptr};
    MeshData data{MeshPrimitive::Points, nullptr, {colors1, colors2}};

    std::ostringstream out;
    Error redirectError{&out};
    data.attributeName(2);
    data.attributeFormat(2);
    data.attributeOffset(2);
    data.attributeStride(2);
    data.attribute(2);
    data.attribute<Vector2>(2);
    data.attributeId(MeshAttribute::Position);
    data.attributeId(MeshAttribute::Color, 2);
    data.attributeFormat(MeshAttribute::Position);
    data.attributeFormat(MeshAttribute::Color, 2);
    data.attributeOffset(MeshAttribute::Position);
    data.attributeOffset(MeshAttribute::Color, 2);
    data.attributeStride(MeshAttribute::Position);
    data.attributeStride(MeshAttribute::Color, 2);
    data.attribute(MeshAttribute::Position);
    data.attribute(MeshAttribute::Color, 2);
    data.attribute<Vector2>(MeshAttribute::Position);
    data.attribute<Vector2>(MeshAttribute::Color, 2);
    data.positions2DAsArray();
    data.positions3DAsArray();
    data.normalsAsArray();
    data.textureCoordinates2DAsArray();
    data.colorsAsArray(2);
    CORRADE_COMPARE(out.str(),
        "Trade::MeshData::attributeName(): index 2 out of range for 2 attributes\n"
        "Trade::MeshData::attributeFormat(): index 2 out of range for 2 attributes\n"
        "Trade::MeshData::attributeOffset(): index 2 out of range for 2 attributes\n"
        "Trade::MeshData::attributeStride(): index 2 out of range for 2 attributes\n"
        "Trade::MeshData::attribute(): index 2 out of range for 2 attributes\n"
        "Trade::MeshData::attribute(): index 2 out of range for 2 attributes\n"
        "Trade::MeshData::attributeId(): index 0 out of range for 0 Trade::MeshAttribute::Position attributes\n"
        "Trade::MeshData::attributeId(): index 2 out of range for 2 Trade::MeshAttribute::Color attributes\n"
        "Trade::MeshData::attributeFormat(): index 0 out of range for 0 Trade::MeshAttribute::Position attributes\n"
        "Trade::MeshData::attributeFormat(): index 2 out of range for 2 Trade::MeshAttribute::Color attributes\n"
        "Trade::MeshData::attributeOffset(): index 0 out of range for 0 Trade::MeshAttribute::Position attributes\n"
        "Trade::MeshData::attributeOffset(): index 2 out of range for 2 Trade::MeshAttribute::Color attributes\n"
        "Trade::MeshData::attributeStride(): index 0 out of range for 0 Trade::MeshAttribute::Position attributes\n"
        "Trade::MeshData::attributeStride(): index 2 out of range for 2 Trade::MeshAttribute::Color attributes\n"
        "Trade::MeshData::attribute(): index 0 out of range for 0 Trade::MeshAttribute::Position attributes\n"
        "Trade::MeshData::attribute(): index 2 out of range for 2 Trade::MeshAttribute::Color attributes\n"
        "Trade::MeshData::attribute(): index 0 out of range for 0 Trade::MeshAttribute::Position attributes\n"
        "Trade::MeshData::attribute(): index 2 out of range for 2 Trade::MeshAttribute::Color attributes\n"
        "Trade::MeshData::positions2DInto(): index 0 out of range for 0 position attributes\n"
        "Trade::MeshData::positions3DInto(): index 0 out of range for 0 position attributes\n"
        "Trade::MeshData::normalsInto(): index 0 out of range for 0 normal attributes\n"
        "Trade::MeshData::textureCoordinates2DInto(): index 0 out of range for 0 texture coordinate attributes\n"
        "Trade::MeshData::colorsInto(): index 2 out of range for 2 color attributes\n");
}

void MeshDataTest::attributeWrongType() {
    MeshAttributeData positions{MeshAttribute::Position, VertexFormat::Vector3, nullptr};
    MeshData data{MeshPrimitive::Points, nullptr, {positions}};

    std::ostringstream out;
    Error redirectError{&out};
    data.attribute<Vector4>(MeshAttribute::Position);
    CORRADE_COMPARE(out.str(), "Trade::MeshData::attribute(): improper type requested for Trade::MeshAttribute::Position of format VertexFormat::Vector3\n");
}

void MeshDataTest::releaseIndexData() {
    Containers::Array<char> indexData{23};
    auto indexView = Containers::arrayCast<UnsignedShort>(indexData.slice(6, 12));

    MeshData data{MeshPrimitive::TriangleStrip, std::move(indexData), MeshIndexData{indexView}};
    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 3);
    CORRADE_COMPARE(data.indexOffset(), 6);

    Containers::Array<char> released = data.releaseIndexData();
    CORRADE_COMPARE(static_cast<void*>(released.data() + 6), indexView.data());
    /* This is not null as we still need the value for calculating offsets */
    CORRADE_COMPARE(static_cast<const void*>(data.indexData()), released.data());
    CORRADE_COMPARE(data.indexData().size(), 0);
    CORRADE_VERIFY(data.isIndexed());
    CORRADE_COMPARE(data.indexCount(), 0);
    CORRADE_COMPARE(data.indexType(), MeshIndexType::UnsignedShort);
    CORRADE_COMPARE(data.indexOffset(), 6);
}

void MeshDataTest::releaseAttributeData() {
    Containers::Array<char> vertexData{16};
    auto vertexView = Containers::arrayCast<Vector2>(vertexData);

    MeshAttributeData positions{MeshAttribute::Position, vertexView};
    MeshData data{MeshPrimitive::LineLoop, std::move(vertexData), {positions, positions}};
    CORRADE_COMPARE(data.attributeCount(), 2);

    Containers::Array<MeshAttributeData> released = data.releaseAttributeData();
    CORRADE_COMPARE(released.size(), 2);
    CORRADE_COMPARE(static_cast<const void*>(released[0].data().data()), vertexView.data());
    CORRADE_COMPARE(released[0].data().size(), 2);
    /* Unlike the other two, this is null as we don't need the value for
       calculating anything */
    CORRADE_COMPARE(static_cast<const void*>(data.attributeData()), nullptr);
    CORRADE_COMPARE(data.attributeCount(), 0);
    CORRADE_COMPARE(static_cast<const void*>(data.vertexData()), vertexView);
    CORRADE_COMPARE(data.vertexCount(), 2);
}

void MeshDataTest::releaseVertexData() {
    Containers::Array<char> vertexData{80};
    auto vertexView = Containers::arrayCast<Vector2>(vertexData.slice(48, 72));

    MeshAttributeData positions{MeshAttribute::Position, vertexView};
    MeshData data{MeshPrimitive::LineLoop, std::move(vertexData), {positions, positions}};
    CORRADE_COMPARE(data.attributeCount(), 2);
    CORRADE_COMPARE(data.vertexCount(), 3);
    CORRADE_COMPARE(data.attributeOffset(0), 48);

    Containers::Array<char> released = data.releaseVertexData();
    CORRADE_VERIFY(data.attributeData());
    CORRADE_COMPARE(data.attributeCount(), 2);
    CORRADE_COMPARE(static_cast<const void*>(static_cast<const char*>(data.attribute(0).data())), vertexView.data());
    CORRADE_COMPARE(static_cast<const void*>(static_cast<const char*>(data.mutableAttribute(0).data())), vertexView.data());
    /* Returned views should be patched to have zero size (but not the direct
       access, there it stays as it's an internal API really) */
    CORRADE_COMPARE(data.attribute(0).size()[0], 0);
    CORRADE_COMPARE(data.mutableAttribute(0).size()[0], 0);
    CORRADE_COMPARE(data.attributeData()[0].data().size(), 3);
    CORRADE_COMPARE(static_cast<void*>(released.data() + 48), vertexView.data());
    /* This is not null as we still need the value for calculating offsets */
    CORRADE_COMPARE(static_cast<const void*>(data.vertexData()), released.data());
    CORRADE_COMPARE(data.vertexCount(), 0);
    CORRADE_COMPARE(data.attributeOffset(0), 48);
}

}}}}

CORRADE_TEST_MAIN(Magnum::Trade::Test::MeshDataTest)
