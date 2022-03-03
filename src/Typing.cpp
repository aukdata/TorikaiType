//
//  Typing.cpp
//  TorikaiType
//

#include "Typing.hpp"

const Array<std::pair<String, String>> Typing::romanize_table =
{
    {U"A", U"あ"}, {U"I", U"い"}, {U"U", U"う"}, {U"E", U"え"}, {U"O", U"お"},
    {U"KA", U"か"}, {U"CA", U"か"}, {U"KI", U"き"}, {U"KU", U"く"}, {U"CU", U"く"}, {U"KE", U"け"}, {U"KO", U"こ"}, {U"CO", U"こ"},
    {U"SA", U"さ"}, {U"SI", U"し"}, {U"SHI", U"し"}, {U"CI", U"し"}, {U"SU", U"す"}, {U"SE", U"せ"}, {U"CE", U"せ"}, {U"SO", U"そ"},
    {U"TA", U"た"}, {U"TI", U"ち"}, {U"CHI", U"ち"}, {U"TU", U"つ"}, {U"TSU", U"つ"}, {U"TE", U"て"}, {U"TO", U"と"},
    {U"NA", U"な"}, {U"NI", U"に"}, {U"NU", U"ぬ"}, {U"NE", U"ね"}, {U"NO", U"の"},
    {U"HA", U"は"}, {U"HI", U"ひ"}, {U"HU", U"ふ"}, {U"FU", U"ふ"}, {U"HE", U"へ"}, {U"HO", U"ほ"},
    {U"MA", U"ま"}, {U"MI", U"み"}, {U"MU", U"む"}, {U"ME", U"め"}, {U"MO", U"も"},
    {U"YA", U"や"}, {U"YU", U"ゆ"}, {U"YO", U"よ"},
    {U"RA", U"ら"}, {U"RI", U"り"}, {U"RU", U"る"}, {U"RE", U"れ"}, {U"RO", U"ろ"},
    {U"WA", U"わ"}, {U"WO", U"を"}, {U"N", U"ん"},
    
    {U"GA", U"が"}, {U"GI", U"ぎ"}, {U"GU", U"ぐ"}, {U"GE", U"げ"}, {U"GO", U"ご"},
    {U"ZA", U"ざ"}, {U"ZI", U"じ"}, {U"JI", U"じ"}, {U"ZU", U"ず"}, {U"ZE", U"ぜ"}, {U"ZO", U"ぞ"},
    {U"DA", U"だ"}, {U"DI", U"ぢ"}, {U"DU", U"づ"}, {U"DE", U"で"}, {U"DO", U"ど"},
    {U"BA", U"ば"}, {U"BI", U"び"}, {U"BU", U"ぶ"}, {U"BE", U"べ"}, {U"BO", U"ぼ"},
    {U"PA", U"ぱ"}, {U"PI", U"ぴ"}, {U"PU", U"ぷ"}, {U"PE", U"ぺ"}, {U"PO", U"ぽ"},
    
    {U"KYA", U"きゃ"}, {U"KYI", U"きぃ"}, {U"KYU", U"きゅ"}, {U"KYE", U"きぇ"}, {U"KYO", U"きょ"},
    {U"KIXYA", U"きゃ"}, {U"KIXI", U"きぃ"}, {U"KIXYU", U"きゅ"}, {U"KIXE", U"きぇ"}, {U"KIXYO", U"きょ"},
    {U"KILYA", U"きゃ"}, {U"KILI", U"きぃ"}, {U"KILYU", U"きゅ"}, {U"KILE", U"きぇ"}, {U"KILYO", U"きょ"},
    
    {U"SHA", U"しゃ"}, {U"SHU", U"しゅ"}, {U"SHE", U"しぇ"}, {U"SHO", U"しょ"},
    {U"SYA", U"しゃ"}, {U"SYI", U"しぃ"}, {U"SYU", U"しゅ"}, {U"SYE", U"しぇ"}, {U"SYO", U"しょ"},
    {U"SIXYA", U"しゃ"}, {U"SIXI", U"しぃ"}, {U"SIXYU", U"しゅ"}, {U"SIXE", U"しぇ"}, {U"SIXYO", U"しょ"},
    {U"SILYA", U"しゃ"}, {U"SILI", U"しぃ"}, {U"SILYU", U"しゅ"}, {U"SILE", U"しぇ"}, {U"SILYO", U"しょ"},
    {U"SHIXYA", U"しゃ"}, {U"SHIXI", U"しぃ"}, {U"SHIXYU", U"しゅ"}, {U"SHIXE", U"しぇ"}, {U"SHIXYO", U"しょ"},
    {U"SHILYA", U"しゃ"}, {U"SHILI", U"しぃ"}, {U"SHILYU", U"しゅ"}, {U"SHILE", U"しぇ"}, {U"SHILYO", U"しょ"},
    {U"CIXYA", U"しゃ"}, {U"CIXI", U"しぃ"}, {U"CIXYU", U"しゅ"}, {U"CIXE", U"しぇ"}, {U"CIXYO", U"しょ"},
    {U"CILYA", U"しゃ"}, {U"CILI", U"しぃ"}, {U"CILYU", U"しゅ"}, {U"CILE", U"しぇ"}, {U"CILYO", U"しょ"},

    {U"CHA", U"ちゃ"}, {U"CHU", U"ちゅ"}, {U"CHE", U"ちぇ"}, {U"CHO", U"ちょ"},
    {U"CYA", U"ちゃ"}, {U"CYI", U"ちぃ"}, {U"CYU", U"ちゅ"}, {U"CYE", U"ちぇ"}, {U"CYO", U"ちょ"},
    {U"TYA", U"ちゃ"}, {U"TYI", U"ちぃ"}, {U"TYU", U"ちゅ"}, {U"TYE", U"ちぇ"}, {U"TYO", U"ちょ"},
    {U"TIXYA", U"ちゃ"}, {U"TIXI", U"ちぃ"}, {U"TIXYU", U"ちゅ"}, {U"TIXE", U"ちぇ"}, {U"TIXYO", U"ちょ"},
    {U"TILYA", U"ちゃ"}, {U"TILI", U"ちぃ"}, {U"TILYU", U"ちゅ"}, {U"TILE", U"ちぇ"}, {U"TILYO", U"ちょ"},
    {U"CHIXYA", U"ちゃ"}, {U"CHIXI", U"ちぃ"}, {U"CHIXYU", U"ちゅ"}, {U"CHIXE", U"ちぇ"}, {U"CHIXYO", U"ちょ"},
    {U"CHILYA", U"ちゃ"}, {U"CHILI", U"ちぃ"}, {U"CHILYU", U"ちゅ"}, {U"CHILE", U"ちぇ"}, {U"CHILYO", U"ちょ"},

    {U"NYA", U"にゃ"}, {U"NYI", U"にぃ"}, {U"NYU", U"にゅ"}, {U"NYE", U"にぇ"}, {U"NYO", U"にょ"},
    {U"NIXYA", U"にゃ"}, {U"NIXI", U"にぃ"}, {U"NIXYU", U"にゅ"}, {U"NIXE", U"にぇ"}, {U"NIXYO", U"にょ"},
    {U"NILYA", U"にゃ"}, {U"NILI", U"にぃ"}, {U"NILYU", U"にゅ"}, {U"NILE", U"にぇ"}, {U"NILYO", U"にょ"},
    
    {U"THA", U"てゃ"}, {U"THI", U"てぃ"}, {U"THU", U"てゅ"}, {U"THE", U"てぇ"}, {U"THO", U"てょ"},
    {U"TEXYA", U"てゃ"}, {U"TEXI", U"てぃ"}, {U"TEXYU", U"てゅ"}, {U"TEXE", U"てぇ"}, {U"TEXYO", U"てょ"},
    {U"TELYA", U"てゃ"}, {U"TELI", U"てぃ"}, {U"TELYU", U"てゅ"}, {U"TELE", U"てぇ"}, {U"TELYO", U"てょ"},

    {U"FA", U"ふぁ"}, {U"FI", U"ふぃ"}, {U"FE", U"ふぇ"}, {U"FO", U"ふぉ"},
    {U"HUXYA", U"ふゃ"}, {U"HUXI", U"ふぃ"}, {U"HUXE", U"ふぇ"}, {U"HUXYO", U"ふょ"},
    {U"HULYA", U"ふゃ"}, {U"HULI", U"ふぃ"}, {U"HULE", U"ふぇ"}, {U"HULYO", U"ふょ"},
    {U"FUXYA", U"ふゃ"}, {U"FUXI", U"ふぃ"}, {U"FUXE", U"ふぇ"}, {U"FUXYO", U"ふょ"},
    {U"FULYA", U"ふゃ"}, {U"FULI", U"ふぃ"}, {U"FULE", U"ふぇ"}, {U"FULYO", U"ふょ"},
    
    {U"HYA", U"ひゃ"}, {U"HYI", U"ひぃ"}, {U"HYU", U"ひゅ"}, {U"HYE", U"ひぇ"}, {U"HYO", U"ひょ"},
    {U"HIXYA", U"ひゃ"}, {U"HIXI", U"ひぃ"}, {U"HIXYU", U"ひゅ"}, {U"HIXE", U"ひぇ"}, {U"HIXYO", U"ひょ"},
    {U"HILYA", U"ひゃ"}, {U"HILI", U"ひぃ"}, {U"HILYU", U"ひゅ"}, {U"HILE", U"ひぇ"}, {U"HILYO", U"ひょ"},
    
    {U"MYA", U"みゃ"}, {U"MYI", U"みぃ"}, {U"MYU", U"みゅ"}, {U"MYE", U"みぇ"}, {U"MYO", U"みょ"},
    {U"MIXYA", U"みゃ"}, {U"MIXI", U"みぃ"}, {U"MIXYU", U"みゅ"}, {U"MIXE", U"みぇ"}, {U"MIXYO", U"みょ"},
    {U"MILYA", U"みゃ"}, {U"MILI", U"みぃ"}, {U"MILYU", U"みゅ"}, {U"MILE", U"みぇ"}, {U"MILYO", U"みょ"},
    
    {U"RYA", U"りゃ"}, {U"RYI", U"りぃ"}, {U"RYU", U"りゅ"}, {U"RYE", U"りぇ"}, {U"RYO", U"りょ"},
    {U"RIXYA", U"りゃ"}, {U"RIXI", U"りぃ"}, {U"RIXYU", U"りゅ"}, {U"RIXE", U"りぇ"}, {U"RIXYO", U"りょ"},
    {U"RILYA", U"りゃ"}, {U"RILI", U"りぃ"}, {U"RILYU", U"りゅ"}, {U"RILE", U"りぇ"}, {U"RILYO", U"りょ"},
    
    {U"GYA", U"ぎゃ"}, {U"GYI", U"ぎぃ"}, {U"GYU", U"ぎゅ"}, {U"GYE", U"ぎぇ"}, {U"GYO", U"ぎょ"},
    {U"GIXYA", U"ぎゃ"}, {U"GIXI", U"ぎぃ"}, {U"GIXYU", U"ぎゅ"}, {U"GIXE", U"ぎぇ"}, {U"GIXYO", U"ぎょ"},
    {U"GILYA", U"ぎゃ"}, {U"GILI", U"ぎぃ"}, {U"GILYU", U"ぎゅ"}, {U"GILE", U"ぎぇ"}, {U"GILYO", U"ぎょ"},
    
    {U"ZYA", U"じゃ"}, {U"ZYI", U"じぃ"}, {U"ZYU", U"じゅ"}, {U"ZYE", U"じぇ"}, {U"ZYO", U"じょ"},
    {U"JA", U"じゃ"}, {U"JU", U"じゅ"}, {U"JE", U"じぇ"}, {U"JO", U"じょ"},
    {U"ZIXYA", U"じゃ"}, {U"ZIXI", U"じぃ"}, {U"ZIXYU", U"じゅ"}, {U"ZIXE", U"じぇ"}, {U"ZIXYO", U"じょ"},
    {U"ZILYA", U"じゃ"}, {U"ZILI", U"じぃ"}, {U"ZILYU", U"じゅ"}, {U"ZILE", U"じぇ"}, {U"ZILYO", U"じょ"},
    {U"JIXYA", U"じゃ"}, {U"JIXI", U"じぃ"}, {U"JIXYU", U"じゅ"}, {U"JIXE", U"じぇ"}, {U"JIXYO", U"じょ"},
    {U"JILYA", U"じゃ"}, {U"JILI", U"じぃ"}, {U"JILYU", U"じゅ"}, {U"JILE", U"じぇ"}, {U"JILYO", U"じょ"},

    {U"DYA", U"ぢゃ"}, {U"DYI", U"ぢぃ"}, {U"DYU", U"ぢゅ"}, {U"DYE", U"ぢぇ"}, {U"DYO", U"ぢょ"},
    {U"DIXYA", U"ぢゃ"}, {U"DIXI", U"ぢぃ"}, {U"DIXYU", U"ぢゅ"}, {U"DIXE", U"ぢぇ"}, {U"DIXYO", U"ぢょ"},
    {U"DILYA", U"ぢゃ"}, {U"DILI", U"ぢぃ"}, {U"DILYU", U"ぢゅ"}, {U"DILE", U"ぢぇ"}, {U"DILYO", U"ぢょ"},
    
    {U"DHA", U"でゃ"}, {U"DHI", U"でぃ"}, {U"DHU", U"でゅ"}, {U"HDE", U"でぇ"}, {U"DHO", U"でょ"},
    {U"DEXYA", U"でゃ"}, {U"DEXI", U"でぃ"}, {U"DEXYU", U"でゅ"}, {U"DEXE", U"でぇ"}, {U"DEXYO", U"でょ"},
    {U"DELYA", U"でゃ"}, {U"DELI", U"でぃ"}, {U"DELYU", U"でゅ"}, {U"DELE", U"でぇ"}, {U"DELYO", U"でょ"},

    {U"BYA", U"びゃ"}, {U"BYI", U"びぃ"}, {U"BYU", U"びゅ"}, {U"BYE", U"びぇ"}, {U"BYO", U"びょ"},
    {U"BIXYA", U"びゃ"}, {U"BIXI", U"びぃ"}, {U"BIXYU", U"びゅ"}, {U"BIXE", U"びぇ"}, {U"BIXYO", U"びょ"},
    {U"BILYA", U"びゃ"}, {U"BILI", U"びぃ"}, {U"BILYU", U"びゅ"}, {U"BILE", U"びぇ"}, {U"BILYO", U"びょ"},
    
    {U"PYA", U"ぴゃ"}, {U"PYI", U"ぴぃ"}, {U"PYU", U"ぴゅ"}, {U"PYE", U"ぴぇ"}, {U"PYO", U"ぴょ"},
    {U"PIXA", U"ぴゃ"}, {U"PIXI", U"ぴぃ"}, {U"PIXU", U"ぴゅ"}, {U"PIXE", U"ぴぇ"}, {U"PIXO", U"ぴょ"},
    {U"PILA", U"ぴゃ"}, {U"PILI", U"ぴぃ"}, {U"PILU", U"ぴゅ"}, {U"PILE", U"ぴぇ"}, {U"PILO", U"ぴょ"},
    
    {U"XA", U"ぁ"}, {U"XI", U"ぃ"}, {U"XU", U"ぅ"}, {U"XE", U"ぇ"}, {U"XO", U"ぉ"}, {U"XWA", U"ゎ"},
    {U"LA", U"ぁ"}, {U"LI", U"ぃ"}, {U"LU", U"ぅ"}, {U"LE", U"ぇ"}, {U"LO", U"ぉ"}, {U"LWA", U"ゎ"},
    {U"VA", U"ゔぁ"}, {U"-", U"ー"}, {U".", U"。"}, {U",", U"、"}, {U"/", U"・"},
    
    {U"A", U"A"}, {U"B", U"B"}, {U"C", U"C"}, {U"D", U"D"}, {U"E", U"E"},
    {U"F", U"F"}, {U"G", U"G"}, {U"H", U"H"}, {U"I", U"I"}, {U"J", U"J"},
    {U"K", U"K"}, {U"L", U"L"}, {U"M", U"M"}, {U"N", U"N"}, {U"O", U"O"},
    {U"P", U"P"}, {U"Q", U"Q"}, {U"R", U"R"}, {U"S", U"S"}, {U"T", U"T"},
    {U"U", U"U"}, {U"V", U"V"}, {U"W", U"W"}, {U"X", U"X"}, {U"Y", U"Y"}, {U"Z", U"Z"},
    
    {U"1", U"1"}, {U"2", U"2"}, {U"3", U"3"}, {U"4", U"4"}, {U"5", U"5"},
    {U"6", U"6"}, {U"7", U"7"}, {U"8", U"8"}, {U"9", U"9"}, {U"0", U"0"},

    {U"-", U"-"}, {U"@", U"@"}, {U";", U";"}, {U":", U":"},
    {U",", U","}, {U".", U"."}, {U"/", U"/"}, {U"[", U"["}, {U"]", U"]"},
};

const HashTable<char32, char32> Typing::hiraganaize_table =
{
    {U'ア', U'あ'}, {U'イ', U'い'}, {U'ウ', U'う'}, {U'エ', U'え'}, {U'オ', U'お'},
    {U'カ', U'か'}, {U'キ', U'き'}, {U'ク', U'く'}, {U'ケ', U'け'}, {U'コ', U'こ'},
    {U'サ', U'さ'}, {U'シ', U'し'}, {U'ス', U'す'}, {U'セ', U'せ'}, {U'ソ', U'そ'},
    {U'タ', U'た'}, {U'チ', U'ち'}, {U'ツ', U'つ'}, {U'テ', U'て'}, {U'ト', U'と'},
    {U'ナ', U'な'}, {U'ニ', U'に'}, {U'ヌ', U'ぬ'}, {U'ネ', U'ね'}, {U'ノ', U'の'},
    
    {U'ハ', U'は'}, {U'ヒ', U'ひ'}, {U'フ', U'ふ'}, {U'ヘ', U'へ'}, {U'ホ', U'ほ'},
    {U'マ', U'ま'}, {U'ミ', U'み'}, {U'ム', U'む'}, {U'メ', U'め'}, {U'モ', U'も'},
    {U'ヤ', U'や'}, {U'ユ', U'ゆ'}, {U'ヨ', U'よ'},
    {U'ラ', U'ら'}, {U'リ', U'り'}, {U'ル', U'る'}, {U'レ', U'れ'}, {U'ロ', U'ろ'},
    {U'ワ', U'わ'}, {U'ヲ', U'を'}, {U'ン', U'ん'},
    
    {U'ガ', U'が'}, {U'ギ', U'ぎ'}, {U'グ', U'ぐ'}, {U'ゲ', U'げ'}, {U'ゴ', U'ご'},
    {U'ザ', U'ざ'}, {U'ジ', U'じ'}, {U'ズ', U'ず'}, {U'ゼ', U'ぜ'}, {U'ゾ', U'ぞ'},
    {U'ダ', U'だ'}, {U'ヂ', U'ぢ'}, {U'ヅ', U'づ'}, {U'デ', U'で'}, {U'ド', U'ど'},
    {U'バ', U'ば'}, {U'ビ', U'び'}, {U'ブ', U'ぶ'}, {U'ベ', U'べ'}, {U'ボ', U'ぼ'},
    {U'パ', U'ぱ'}, {U'ピ', U'ぴ'}, {U'プ', U'ぷ'}, {U'ペ', U'ぺ'}, {U'ポ', U'ぽ'},
    
    {U'ァ', U'ぁ'}, {U'ィ', U'ぃ'}, {U'ゥ', U'ぅ'}, {U'ェ', U'ぇ'}, {U'ォ', U'ぉ'},
    {U'ッ', U'っ'}, {U'ャ', U'ゃ'}, {U'ュ', U'ゅ'}, {U'ョ', U'ょ'}, {U'ヮ', U'ゎ'},
    
    {U'a', U'A'}, {U'b', U'B'}, {U'c', U'C'}, {U'd', U'D'}, {U'e', U'E'},
    {U'f', U'F'}, {U'g', U'G'}, {U'h', U'H'}, {U'i', U'I'}, {U'j', U'J'},
    {U'k', U'K'}, {U'l', U'L'}, {U'm', U'M'}, {U'n', U'N'}, {U'o', U'O'},
    {U'p', U'P'}, {U'q', U'Q'}, {U'r', U'R'}, {U's', U'S'}, {U't', U'T'},
    {U'u', U'U'}, {U'v', U'V'}, {U'w', U'W'}, {U'x', U'X'}, {U'y', U'Y'}, {U'z', U'Z'},
};


std::pair<int32, int32> Typing::getNextCharPos() const
{
    const auto& titleFont = FontAsset(U"Text");
    int32 offsetx = 0;
    m_parsed.take(m_pos).each([&] (const auto& s) { offsetx += titleFont(s).region().w; });
    
    return {offsetx + titleFont(m_parsed[m_pos]).region().w / 2, 0};
}


bool Typing::onCharInput(const char32 s)
{
    if (m_pos == -1)
        return false;
    
    const auto newBuffer = m_buffer + s;
    Array<String> cand;
    const bool sokuon = m_hiraganized[m_pos].starts_with(U'っ');
    
    // 正解の文字になるものを候補として抽出
    const auto& filtered_kana_table = romanize_table.filter([&] (const auto& pair) {
        return (!sokuon && pair.second == m_hiraganized[m_pos]) || (sokuon && U"っ" + pair.second == m_hiraganized[m_pos]);
    });
    
    for (const auto& pair : filtered_kana_table)
    {
        Array<String> suffixes;
        if (sokuon)
            suffixes = {String(1, pair.first[0]).c_str(), U"XTU", U"XTSU", U"LTU", U"LTSU"};
        else
            suffixes = {U""};
        
        for (const auto& suffix :suffixes)
        {
            const auto& roma = suffix + pair.first;
            
            // 既に一致しているならOK
            if (roma == newBuffer)
            {
                m_buffer = U"";
                m_pos++;
                m_acceptSecondN = roma == U"N";
                return true;
            }
            else if (roma.starts_with(newBuffer))
            {
                cand.push_back(roma);
            }
        }
    }
    
    // 候補がないならミスタイプと判定
    if (cand.size() <= 0)
    {
        // 前の文字が「ん」でNがミスタイプ判定されたとき、一度だけ見逃す
        if (s == U'N' && (m_acceptSecondN || (m_pos > 0 && m_hiraganized[m_pos - 1] == U"ん" && m_romanized[m_pos - 1] == U"N" && m_buffer.empty())))
        {
            if (m_pos > 0) m_romanized[m_pos - 1] += U"N";
            m_acceptSecondN = false;
            return true;
        }
        
        m_acceptSecondN = false;
        return false;
    }
    else
    {
        m_buffer = newBuffer;
        m_romanized[m_pos] = cand[0];
        m_acceptSecondN = false;
        return true;
    }
}

[[nodiscard]] char32 Typing::getNextChar() const
{
    if (m_romanized.size() > m_pos && m_romanized[m_pos].size() > m_buffer.size())
        return m_romanized[m_pos][m_buffer.size()];
    return U'\0';
}

void Typing::update()
{
    if (m_pos == -1)
    {
        setText(m_nextWordHandler ? m_nextWordHandler() : U"データなし");
    }
    if (m_pos >= m_hiraganized.size())
    {
        if (m_onCorrectWordHandler) m_onCorrectWordHandler();
        setText(m_nextWordHandler ? m_nextWordHandler() : U"データなし");
    }

    if (KeyBackspace.down())
    {
        m_buffer.pop_back();
    }
}

void Typing::draw(const int32 x, const int32 y, const bool showHiragana, const bool showRomaji, const bool showTypedRomaji) const
{
    if (m_pos == -1)
        return;
    
    int32 offsetx = x;
    m_parsed.each_index([&] (const int32 i, const auto& s) {
        const auto& titleDrawer = FontAsset(U"Text")(s);
        const int titleWidth = titleDrawer.region().w;
        
        const auto& subtitleDrawer = FontAsset(U"SmallText")(m_romanized[i]);
        
        const auto& color = i >= m_pos ? Palette::White : Palette::Lightpink;
        
        if (showHiragana)
            titleDrawer.drawAt(offsetx + titleWidth / 2, y, color);
        
        const bool m_areAlphabets = m_parsed[m_pos].all([&] (const char32 c) { return isAlphabet(c); });
        if (m_parsed[i] != m_romanized[i] && !m_areAlphabets)
        {
            if ((i < m_pos && showTypedRomaji) || showRomaji)
                subtitleDrawer.drawAt(offsetx + titleWidth / 2, y + 40, color);
            
            if (i == m_pos && showTypedRomaji)
            {
                const int subtitleWidth = subtitleDrawer.region().w;
                const int subtitleHeight = subtitleDrawer.region().h;
                FontAsset(U"SmallText")(m_buffer).draw(offsetx + titleWidth / 2 - subtitleWidth / 2, y + 40 - subtitleHeight / 2, Palette::Lightpink);
            }
        }
        
        offsetx += titleWidth;
    });
}
