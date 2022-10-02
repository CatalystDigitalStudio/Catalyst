#include "pch.h"
#include "unicode.h"

namespace Catalyst
{
    namespace utf8
    {
        size_t nextCodepointLength(codeunit* str)
        {
            auto get_length = [&]() {
                size_t i = sizeof(codeunit);
                while (CATALYST_BIT_CHK(*str, i, 1)) //Looks for first 0 from left
                {
                    i--;
                }
                return (sizeof(codeunit) - i);
            };

            size_t length = get_length();

            if (length == 0)
            {
                return 1;
            }
            else if (length == 1)
            {
                Engine::raiseError({ Level::Warning, "CatalystResult::Utf8_Codepoint_Found_Unexpected_Byte", CatalystResult::Utf8_Codepoint_Found_Unexpected_Byte, __FUNCTION__ });
                return 0;
            }
            else if (length == 2)
            {
                return 2;
            }
            else if (length == 3)
            {
                return 3;
            }
            else if (length == 4)
            {
                return 4;
            }
            else if (length == 5)
            {
                Engine::raiseError({ Level::Warning, "CatalystResult::Utf8_Codepoint_Malformed", CatalystResult::Utf8_Codepoint_Malformed, __FUNCTION__ });
                return -1;
            }
            else
            {
                Engine::raiseError({ Level::Warning, "CatalystResult::Utf8_Codepoint_Malformed", CatalystResult::Utf8_Codepoint_Malformed, __FUNCTION__ });
                return -1;
            }
        }


        static CatalystResult internal_encodeCodepoint(UChar cp, UChar* buffer)
        {
            if (cp <= CATALYST_UTF_MAX_ASCII)
            {
                codeunit u8 = as<codeunit>(cp);
                buffer[0] = u8;
                return CatalystResult::Success;
            }
            else if (cp <= CATALYST_UTF_MAX_UTF16_UNENCODED)
            {
                char16_t u16 = as<char16_t>(cp);
                *buffer = { UChar
                    ((CATALYST_UTF_CONTINUATION_FLAG ^ (CATALYST_UTF_CONTINUATION_MASK & codeunit(u16     ))) ^      // Add flag
                    ((CATALYST_UTF_HEADER_16         ^ (CATALYST_UTF_HEADER_16_MASK    & codeunit(u16 << 6))) << 8)) // Add header & shift data
                };
            }
            else if (cp <= 0xFFFF)
            {
                *buffer = { UChar
                    ((CATALYST_UTF_CONTINUATION_FLAG ^ (CATALYST_UTF_CONTINUATION_MASK & codeunit(cp     )))       ^ // Add flag
                    ((CATALYST_UTF_CONTINUATION_FLAG ^ (CATALYST_UTF_CONTINUATION_MASK & codeunit(cp >> 6))) << 8) ^ // Add flag and shift data
                    ((CATALYST_UTF_HEADER_24         ^ (CATALYST_UTF_HEADER_24_MASK    & codeunit(cp >> 12))) << 16)  // Add header and shift data
               ) };
            }
            else if (cp <= CATALYST_UTF_MAX_PLANE)
            {
                *buffer = { UChar
                    ((CATALYST_UTF_CONTINUATION_FLAG ^ (CATALYST_UTF_CONTINUATION_MASK & codeunit(cp      )))        ^ // Add flag
                    ((CATALYST_UTF_CONTINUATION_FLAG ^ (CATALYST_UTF_CONTINUATION_MASK & codeunit(cp >> 6 ))) << 8)  ^ // Add flag and shift data
                    ((CATALYST_UTF_CONTINUATION_FLAG ^ (CATALYST_UTF_CONTINUATION_MASK & codeunit(cp >> 12))) << 16) ^ // Add flag and shift data
                    ((CATALYST_UTF_HEADER_32         ^ (CATALYST_UTF_HEADER_32_MASK    & codeunit(cp >> 18))) << 24)   // Add header and shift data
               ) };
            }
            else
            {
                (char32_t&)(*buffer) = { CATALYST_UTF_ERROR_DECODE };
                return CatalystResult::Utf8_Codepoint_Found_Unexpected_Byte;
            }

            return CatalystResult::Success;
        }

        static CatalystResult internal_decodeCodepoint(UChar cp, UChar* buffer, size_t length)
        {
            if (length == 1)
            {
                buffer[0] = cp;
                return CatalystResult::Success;
            }
            else if (length == 2)
            {
                *buffer = { (UChar)
                    ((CATALYST_UTF_CONTINUATION_MASK & codeunit(cp)) ^    // Remove continuation flag
                    ((CATALYST_UTF_HEADER_16_MASK    & codeunit(cp >> 8)) << 6))   // Remove header and shift data
                };

                return CatalystResult::Success;
            }
            else if (length == 3)
            {
                *buffer = { (UChar)
                    ((CATALYST_UTF_CONTINUATION_MASK & codeunit(cp))            ^    // Remove continuation flag
                    ((CATALYST_UTF_CONTINUATION_MASK & codeunit(cp >> 8)) << 6) ^    // Remove continuation flag and shift data
                    ((CATALYST_UTF_HEADER_24_MASK    & codeunit(cp >> 16)) << 12))   // Remove header and shift data
                };

                return CatalystResult::Success;
            }
            else if (length == 4)
            {
                *buffer = { (UChar)
                    ((CATALYST_UTF_CONTINUATION_MASK & codeunit(cp))              ^  // Remove continuation flag
                    ((CATALYST_UTF_CONTINUATION_MASK & codeunit(cp >> 8 )) << 6)  ^  // Remove continuation flag and shift data
                    ((CATALYST_UTF_CONTINUATION_MASK & codeunit(cp >> 16)) << 12) ^  // Remove continuation flag and shift data
                    ((CATALYST_UTF_HEADER_32_MASK    & codeunit(cp >> 24)) << 18))   // Remove header and shift data
                };

                return CatalystResult::Success;
            }
            else
            {
                (char32_t&)(*buffer) = { CATALYST_UTF_ERROR_DECODE };
                return CatalystResult::Utf8_Codepoint_Malformed;
            }
        }

        CatalystResult encodeCodepoint(UChar cp, UChar* buffer)
        {
            return internal_encodeCodepoint(cp, buffer);
        }
        CatalystResult decodeCodepoint(UChar cp, UChar* buffer)
        {
            return internal_decodeCodepoint(cp, buffer, nextCodepointLength((codeunit*) &cp));
        }
        unicode::Node createNodeList(const UChar* str)
        {
            static auto getLength = [&]()
            {
                const auto* ptr = str;
                size_t size = 0;
                while (*ptr)
                {
                    ++size;
                    ++ptr;
                }
                return size;
            };

            size_t length = getLength();

            unicode::Node nodes = {};

            nodes.m_Data = std::make_shared<unicode::Node[]>(length);
            unicode::Node* index = (unicode::Node*) nodes.m_Data.get();

            while (str && index < (unicode::Node*)nodes.m_Data.get() + length)
            {
                unicode::Node node = {};

                node.m_Parent   = &nodes;
                node.m_Data     = std::make_shared<UChar>(*str);

                if (index - 1 >= nodes.m_Data.get())
                {
                    node.m_Previous = index - 1;
                    node.m_Previous->m_Next = index;
                }

                *index = node;

                ++index;
                ++str;
            }

            return nodes;
        }
}
}

