#pragma once

#include "internal.h"

#ifndef CATALYST_UNICODE_HEADER
#define CATALYST_UNICODE_HEADER

#define CATALYST_UTF_MAX_PLANE       0x0010FFFF

#define CATALYST_UTF_PLANE_0         0x00000000
#define CATALYST_UTF_PLANE_0_MAX     0x0000FFFF
#define CATALYST_UTF_PLANE_1         0x00010000
#define CATALYST_UTF_PLANE_1_MAX     0x0001FFFF
#define CATALYST_UTF_PLANE_2         0x00020000
#define CATALYST_UTF_PLANE_2_MAX     0x0002FFFF
#define CATALYST_UTF_PLANE_3         0x00030000
#define CATALYST_UTF_PLANE_3_MAX     0x0003FFFF
#define CATALYST_UTF_PLANE_4         0x00040000
#define CATALYST_UTF_PLANE_4_MAX     0x0004FFFF
#define CATALYST_UTF_PLANE_12        0x000C0000
#define CATALYST_UTF_PLANE_12_MAX    0x000CFFFF
#define CATALYST_UTF_PLANE_13        0x000D0000
#define CATALYST_UTF_PLANE_13_MAX    0x000DFFFF
#define CATALYST_UTF_PLANE_14        0x000E0000
#define CATALYST_UTF_PLANE_14_MAX    0x000EFFFF
#define CATALYST_UTF_PLANE_15        0x000F0000
#define CATALYST_UTF_PLANE_15_MAX    0x000FFFFF
#define CATALYST_UTF_PLANE_16        0x00100000
#define CATALYST_UTF_PLANE_16_MAX    0x0010FFFF
#define CATALYST_UTF_PLANE_RSV       0x00040000
#define CATALYST_UTF_PLANE_RSV_MAX   0x000DFFFF
#define CATALYST_UTF_PLANE_BAN       0x0000D800
#define CATALYST_UTF_PLANE_BAN_MAX   0x000DFFFF

#define CATALYST_UTF_MAX_ASCII           0x007F
#define CATALYST_UTF_MAX_UTF16_UNENCODED 0x00FF
#define CATALYST_UTF_BYTE_ORDER_MARK     0xFEFF
#define CATALYST_UTF_BOM_ENCODE        0xEFBBBF
#define CATALYST_UTF_ERROR_ENCODE      0xEFBFBD
#define CATALYST_UTF_ERROR_DECODE        0xFDFF

#define CATALYST_UTF_HEADER_16             0xC0
#define CATALYST_UTF_HEADER_16_MASK        0x1F

#define CATALYST_UTF_HEADER_24             0xE0
#define CATALYST_UTF_HEADER_24_MASK        0x0F

#define CATALYST_UTF_HEADER_32             0xF0
#define CATALYST_UTF_HEADER_32_MASK        0x07

#define CATALYST_UTF_CONTINUATION_FLAG     0x80
#define CATALYST_UTF_CONTINUATION_MASK     0x3F

#define CATALYST_UTF_TAIL                0x80BF

#define CATALYST_BIT_CHK(BYTES, INDEX, VALUE) (VALUE & (BYTES >> INDEX))

namespace Catalyst
{
    typedef char32_t UChar;

    namespace unicode
    {

        template<unsigned long lower, unsigned long upper>
        struct UtfAllocationSection
        {
            static constexpr unsigned int begin = lower;
            static constexpr unsigned int end = upper;

            template<typename T>
            inline static constexpr bool isInRange(T value)
            {
                return ((begin <= begin) && (value <= end));
            }
        };

        /* Basic Multilingual Plane */
        using UTF_BMP    = UtfAllocationSection<CATALYST_UTF_PLANE_0,  CATALYST_UTF_PLANE_0_MAX >;
        /* Supplementary Multilingual Plane */
        using UTF_SMP    = UtfAllocationSection<CATALYST_UTF_PLANE_1,  CATALYST_UTF_PLANE_1_MAX >;
        /* Supplementary Ideographic Plane */
        using UTF_SIP    = UtfAllocationSection<CATALYST_UTF_PLANE_2,  CATALYST_UTF_PLANE_2_MAX >;
        /* Tertiary Ideographic Plane */
        using UTF_TIP    = UtfAllocationSection<CATALYST_UTF_PLANE_3,  CATALYST_UTF_PLANE_3_MAX >;
        /* Unassigned Ideographic Plane */
        using UTF_UAP    = UtfAllocationSection<CATALYST_UTF_PLANE_4,  CATALYST_UTF_PLANE_13_MAX >;
        /* Supplementart Special-purpose Plane */
        using UTF_SSP    = UtfAllocationSection<CATALYST_UTF_PLANE_14, CATALYST_UTF_PLANE_14_MAX>;
        /* Supplementart Private-Use Area A */
        using UTF_SPUA_A = UtfAllocationSection<CATALYST_UTF_PLANE_15, CATALYST_UTF_PLANE_15_MAX>;
        /* Supplementart Private-Use Area B */
        using UTF_SPUA_B = UtfAllocationSection<CATALYST_UTF_PLANE_16, CATALYST_UTF_PLANE_16_MAX>;


        struct Node
        {
            Node* m_Parent;

            Node* m_Previous;
            Node* m_Next;

            std::shared_ptr<void> m_Data;
        };
    }

    namespace utf8
    {

        typedef char8_t codeunit;

        template<typename Unit, typename Code>
        static constexpr Unit as(Code code)
        {
            return { (Unit)code };
        }

        struct alignas(8) Utf8Byte
        {
            bool bit0 : 1;
            bool bit1 : 1;
            bool bit2 : 1;
            bool bit3 : 1;
            bool bit4 : 1;
            bool bit5 : 1;
            bool bit6 : 1;
            bool bit7 : 1;
        };


        enum class Direction
        {
            DFLT,
            LR,
            RL,
            TB,
            BT
        };


        extern size_t nextCodepointLength(codeunit* cp);

        extern CatalystResult encodeCodepoint(UChar cp, UChar* buffer);
        extern CatalystResult decodeCodepoint(UChar cp, UChar* buffer);

        extern unicode::Node createNodeList(const UChar* str);

    }
}

#endif //CATALYST_UNICODE_HEADER