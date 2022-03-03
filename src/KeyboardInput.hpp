//
//  KeyboardInput.hpp
//  TorikaiType
//

#ifndef KeyboardInput_hpp
#define KeyboardInput_hpp

# include <stdio.h>
# include <Siv3D.hpp>

class KeyboardInput
{
    inline static Array<char32> m_input_characters;
    inline static int32 m_frame = 0;
    
    constexpr inline static std::array<char32, 13> special_characters = {U'-', U'^', U'¥', U'@', U'[', U';', U':', U']', U',', U'.', U'/', U'_', U' '};
    constexpr inline static std::array<int32, 13> special_keycodes = {0xBD, 0xDE, 0x00, 0xDB, 0xDD, 0xBB, 0xBA, 0xDC, 0xBC, 0xBE, 0xBF, 0xE2, 0x20};
    
    [[nodiscard]] static int32 convert_character_to_keycode(const char32 c)
    {
        const auto it = std::find(special_characters.begin(), special_characters.end(), c);
        return it == special_characters.end() ? c : *it;
    }
    [[nodiscard]] constexpr static char32 convert_keycode_to_character(const int32 v)
    {
        for (const auto i : step(special_keycodes.size()))
        {
            if (special_keycodes[i] == v)
            {
                return special_characters[i];
            }
        }
        return v;
    }

    static void updateInputChars()
    {
        m_input_characters.clear();
        
        constexpr auto keycodes = std::array<char32, 49> {U'A', U'B', U'C', U'D', U'E', U'F', U'G', U'H', U'I', U'J', U'K', U'L', U'M', U'N', U'O', U'P', U'Q', U'R', U'S', U'T', U'U', U'V', U'W', U'X', U'Y', U'Z', U'1', U'2', U'3', U'4', U'5', U'6', U'7', U'8', U'9', U'0', 0xBD, 0xDE, 0x00, 0xDB, 0xDD, 0xBB, 0xBA, 0xDC, 0xBC, 0xBE, 0xBF, 0xE2, 0x20};
        for (int32 s : keycodes)
        {
            if (Input(InputDeviceType::Keyboard, s).down())
            {
                m_input_characters.push_back(convert_keycode_to_character(s));
            }
        }
    }
    
public:
    
    [[nodiscard]] static const Array<char32>& getInputChars()
    {
        auto frame = s3d::Scene::FrameCount();
        if (frame > m_frame)
        {
            updateInputChars();
        }
        m_frame = frame;
        
        return m_input_characters;
    }
};

#endif /* KeyboardInput_hpp */
