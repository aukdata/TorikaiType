//
//  Typing.hpp
//  TorikaiType
//

# ifndef Typing_hpp
# define Typing_hpp

# include <Siv3D.hpp>
# include <functional>


class Typing
{
    static const Array<std::pair<String, String>> romanize_table;
    static const HashTable<char32, char32> hiraganaize_table;
    
    Array<String> m_parsed, m_hiraganized, m_romanized;
    int32 m_pos = -1;
    String m_buffer = U"";
    bool m_acceptSecondN = false;
    
    [[nodiscard]] bool isSmallChar(char32 c) const
    {
        constexpr std::array<char32, 16> smallChs =
        {
            U'ぁ', U'ぃ', U'ぅ', U'ぇ', U'ぉ', U'ゃ', U'ゅ', U'ょ',
            U'ァ', U'ィ', U'ゥ', U'ェ', U'ォ', U'ャ', U'ュ', U'ョ'
        };
        return std::find(smallChs.begin(), smallChs.end(), c) != smallChs.end();
    }
    [[nodiscard]] constexpr bool isAlphabet(char32 c) const
    {
        return U'A' <= c && c <= U'z';
    }
    
    void parseText(String text) {
        String buf = U"";
        bool sokuon = false;
        
        m_parsed.clear();
        
        for (auto c : text)
        {
            if (c == U'っ' || c == U'ッ')
            {
                if (!sokuon)
                {
                    m_parsed.push_back(std::move(buf));
                    buf = U"";
                }
                buf += c;
                sokuon = true;
            }
            else
            {
                if (isSmallChar(c))
                {
                    m_parsed.push_back(buf + c);
                    buf = U"";
                    sokuon = true;
                }
                else
                {
                    if (!sokuon)
                    {
                        m_parsed.push_back(std::move(buf));
                        buf = U"";
                    }
                    buf += c;
                    sokuon = false;
                }
            }
        }
        
        if (buf != U"")
        {
            m_parsed.push_back(std::move(buf));
        }
        
        m_parsed.pop_front();
    }

    void setText(String text)
    {
        parseText(text);
        m_hiraganized = m_parsed.map([&] (const auto& s) {
            String hiragana = U"";
            s.each([&] (const auto c) { hiragana += hiraganaize_table.count(c) > 0 ? hiraganaize_table.at(c) : c; });
            return std::move(hiragana);
        });
        m_romanized = m_hiraganized.map([&] (const auto& kana) {
            for (const auto& pair : romanize_table)
            {
                if (pair.second == kana)
                {
                    return pair.first;
                }
                else if (U"っ" + pair.second == kana)
                {
                    return pair.first[0] + pair.first;
                }
            }
            return String(U" ");
        });
        
        m_pos = 0;
    }
    
    std::function<void()> m_onCorrectWordHandler;
    std::function<String()> m_nextWordHandler;

public:
    
    Typing() {};
    
    std::pair<int32, int32> getNextCharPos() const;
    
    void setCorrectWordHandler(std::function<void()> handler) { m_onCorrectWordHandler = handler; }
    void setNextWordHandler(std::function<String()> handler) { m_nextWordHandler = handler; }

    bool onCharInput(const char32 s);
    [[nodiscard]] char32 getNextChar() const;
    
    void update();
    void draw(const int32 x, const int32 y, const bool showHiragana = true, const bool showRomaji = true, const bool showTypedRomaji = true) const;
};

# endif /* Typing_hpp */
