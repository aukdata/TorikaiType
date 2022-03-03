//
//  WordFactory.hpp
//  TorikaiType
//

# ifndef WordFactory_hpp
# define WordFactory_hpp

# include <Siv3D.hpp>

// 出題する単語の種類
enum class WordType : uint8
{
    None = 0, Random = 1, Text = 2
};


class WordFactory
{
    bool m_loaded = false;
    WordType m_type;
    
    String m_filepath;
    Array<String> m_words;
    
    Array<char32> m_allowedChars;
    
    String generateRandomText()
    {
        static String excluded = U"^¥_";
        
        String output = U"";
        for ([[maybe_unused]] const auto i : step(Random(5, 10)))
        {
            char32 c;
            
            do
            {
                c = m_allowedChars[Random(m_allowedChars.size() - 1)];
            }
            while (excluded.includes(c));
            
            output += c;
        }
        return output;
    }
    String getWord()
    {
        const size_t pos = Random(m_words.size() - 1);
        const String output = m_words[pos];
        m_words.remove_at(pos);
        return output;
    }
    
public:
    
    WordFactory(const WordType type, const String& filepath, const Array<char32>& allowedChars) :
    m_type(type),
    m_filepath(filepath),
    m_allowedChars(allowedChars) {}
    
    void loadWords()
    {
        if (m_type == WordType::Text)
        {
            TextReader reader(Resource(m_filepath));
            String line;
            
            m_words.clear();
            while (reader.readLine(line))
            {
                if (line.size() > 0)
                    m_words.push_back(line);
            }
        }
    }
    
    [[nodiscard]] String nextWord()
    {
        switch (m_type)
        {
            case WordType::Random:
                return generateRandomText();
            case WordType::Text:
                return getWord();
            default:
                return U"error";
        }
    }
};


# endif /* WordFactory_hpp */
