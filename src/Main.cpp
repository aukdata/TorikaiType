# include <Siv3D.hpp>
# include <HamFramework.hpp>

# include "GameInfo.hpp"
# include "Typing.hpp"
# include "CheckBox.hpp"
# include "WordFactory.hpp"
# include "KeyboardInput.hpp"


///////////////////////////////////////////////////////////////////////
//
//  間違えた時のエフェクト
//
struct TypoEffect : IEffect
{
    Vec2 pos;
    
    TypoEffect(const double x, const double y)
    : pos(Vec2(x, y)) {}
    
    bool update(double timeSec)
    {
        const double e = EaseOut(Easing::Quad, timeSec);
        FontAsset(U"SmallText")(U"Miss!").drawAt(pos.x, pos.y - 20 * e, ColorF(Palette::Red, 1 - timeSec));
        return timeSec < 1.0;
    }
};


///////////////////////////////////////////////////////////////////////
//
//  キーボードを描画
//
const Array<Array<char32>> qwerty =
{
    { U'1', U'2', U'3', U'4', U'5', U'6', U'7', U'8', U'9', U'0', U'-', U'^', U'¥' },
       { U'Q', U'W', U'E', U'R', U'T', U'Y', U'U', U'I', U'O', U'P', U'@', U'[' },
          { U'A', U'S', U'D', U'F', U'G', U'H', U'J', U'K', U'L', U';', U':', U']' },
             { U'Z', U'X', U'C', U'V', U'B', U'N', U'M', U',', U'.', U'/', U'_' }
};
void drawKeyboard(const double x, const double y, const Array<char32> &highlight)
{
    qwerty.each_index([&] (const int32 i, const auto& line) {
      line.each_index([&] (const int32 j, const char32 c) {
          const auto pressed = highlight.includes(c);
          const auto& color = pressed ? Palette::Lightpink : Palette::White;
          RoundRect(x + j * 80 + i * 40, y + i * 80, 70, 70, 5).draw(color);
          FontAsset(U"Text")(c).drawAt(x + j * 80 + i * 40 + 35, y + i * 80 + 35, GameInfo::BackgroundColor);
      });
    });
}
void drawFingers(const char32 c)
{
    const static std::array<String, 10> keysCorrespondingToEachFngers =
    {
        U"", U"5TGB4RFV", U"3EDC", U"2WSX", U"1QAZ",
        U"", U"6YHN7UJM", U"8IK,", U"9OL.", U"0P;/-@:_^[]¥",
    };
    constexpr static std::array<int32, 5> fingerLength = {30, 80, 100, 80, 50};
    const int32 offsetX = Scene::Center().x;
    
    for (const int32 i : step(5))
    {
        RoundRect(offsetX - 100 - i * 50, 720 -  fingerLength[i], 40,  fingerLength[i] + 20, 20)
        .draw(keysCorrespondingToEachFngers[i].includes(c) ? Palette::Lightpink : Palette::White);
        RoundRect(offsetX + 100 + i * 50, 720 -  fingerLength[i], 40,  fingerLength[i] + 20, 20)
        .draw(keysCorrespondingToEachFngers[i + 5].includes(c) ? Palette::Lightpink : Palette::White);
    }
}


///////////////////////////////////////////////////////////////////////
//
//  セーブ用スコアデータ
//
struct ScoreData
{
    int32 timeMillisec = 0;
    int32 wordCount = 0;
    int32 correctTypeCount = 0;
    int32 typoCount = 0;
    
    Date date = Date(1970, 1, 1);
    bool isRandom = false;
    bool isTimeAttack = false;
    
    double speed() const
    {
        return timeMillisec == 0 ? -1 : correctTypeCount * 1000.0 / timeMillisec;
    }
    int32 score() const
    {
        return (timeMillisec == 0 || typoCount + correctTypeCount == 0) ? -1 : speed() * 126 * pow((double) correctTypeCount / (typoCount + correctTypeCount), 2);
    }
    
    template <class Archive>
    void SIV3D_SERIALIZE(Archive& archive)
    {
        archive(timeMillisec, wordCount, correctTypeCount, typoCount, date, isRandom, isTimeAttack);
    }
};



///////////////////////////////////////////////////////////////////////
//
//  セーブ用設定データ
//
struct ConfigData
{
    std::array<bool, 4> activeLines = {true, true, true, true};
    bool showRomaji = true;
    bool showKeyboard = true;
    bool showFingers = true;
    
    template <class Archive>
    void SIV3D_SERIALIZE(Archive& archive)
    {
        archive(activeLines, showRomaji, showKeyboard, showFingers);
    }
};



///////////////////////////////////////////////////////////////////////
//
//  デフォルトのハイスコア
//
const std::array<std::array<ScoreData, 5>, 5> defaultHighScores;

///////////////////////////////////////////////////////////////////////
//
//  全てのシーンからアクセスできるデータ
//
struct GameData
{
    WordType type = WordType::None;
    Array<char32> allowedChars;
    bool isTimeAttack = false;
    
    ScoreData lastScore;
    ConfigData config;
};



using MyApp = SceneManager<String, GameData>;

///////////////////////////////////////////////////////////////////////
//
//  タイトル画面
//
class Title : public MyApp::Scene
{
    Stopwatch m_effectBackgroundStopwatch{ StartImmediately::Yes };
    
    Stopwatch m_effectMenuItemStopwatch{ StartImmediately::Yes };
    
    const Array<RoundRect> m_menuRects =
    {
        RoundRect(Arg::center = Vec2(Scene::Center().x - 220, 300), 360, 100, 50),
        RoundRect(Arg::center = Vec2(Scene::Center().x - 220, 440), 360, 100, 50),

        RoundRect(Arg::center = Vec2(Scene::Center().x + 220, 300), 360, 100, 50),
        RoundRect(Arg::center = Vec2(Scene::Center().x + 220, 440), 360, 100, 50),
    };
    const Array<Circle> m_menuCircles = {
        Circle(486, 610, 60),
        Circle(640, 630, 60),
        Circle(794, 610, 60),
    };

    const Array<String> m_menuTexts =
    {
        U"スコア",
        U"終了",
        U"設定",
    };
    
public:
    
    Title(const InitData& init)
    : IScene(init)
    {
        if (FileSystem::Exists(GameInfo::ConfigFilePath))
        {
            try
            {
                Deserializer<BinaryReader>{GameInfo::ConfigFilePath}(getData().config);
            }
            catch (cereal::Exception e)
            {
                Serializer<BinaryWriter>{GameInfo::ConfigFilePath}(getData().config);
            }
        }
        else
        {
            Serializer<BinaryWriter>{GameInfo::ConfigFilePath}(getData().config);
        }
    }
    
    void update() override
    {
        bool handCursor = false;
        
        m_menuRects.each_index([&] (int i, const auto& shape) {
            handCursor |= shape.mouseOver();
            
            if (shape.leftClicked())
            {
                if (i == 0)
                {
                    getData().type = WordType::Random;
                    getData().isTimeAttack = true;
                    changeScene(U"Select");
                }
                else if (i == 1)
                {
                    getData().type = WordType::Text;
                    getData().isTimeAttack = true;
                    changeScene(U"Game");
                }
                else if (i == 2)
                {
                    getData().type = WordType::Random;
                    getData().isTimeAttack = false;
                    changeScene(U"Select");
                }
                else if (i == 3)
                {
                    getData().type = WordType::Text;
                    getData().isTimeAttack = false;
                    changeScene(U"Game");
                }
                return;
            }
        });
        m_menuCircles.each_index([&] (int i, const auto& shape) {
            handCursor |= shape.mouseOver();
            
            if (shape.leftClicked())
            {
                if (i == 0)
                {
                    changeScene(U"Score");
                }
                else if (i == 1)
                {
                    System::Exit();
                }
                else if (i == 2)
                {
                    changeScene(U"Config");
                }
                return;
            }
        });

        Cursor::RequestStyle(handCursor ? CursorStyle::Hand : CursorStyle::Default);
    }
    
    void draw() const override
    {
        FontAsset(U"Title")(GameInfo::Title).drawAt(Scene::Center().x, 80);
        
        FontAsset(U"MenuLarge")(U"20単語").drawAt(Scene::Center().x - 220, 200);
        FontAsset(U"MenuLarge")(U"2分間").drawAt(Scene::Center().x + 220, 200);
        
        m_menuRects.each_index([&] (const int32 i, const auto& shape) {
            shape.draw();
            FontAsset(U"MenuLarge")(i % 2 == 0 ? U"ポジション練習" : U"文章・単語練習").drawAt(shape.center(), Palette::Black);
        });
        m_menuCircles.each_index([&] (const int32 i, const auto& shape) {
            shape.draw();
            FontAsset(U"MenuSmall")(m_menuTexts[i]).drawAt(shape.center, Palette::Black);
        });

        const Size versionSize = FontAsset(U"Version")(GameInfo::Version).region().size.asPoint();
        FontAsset(U"Version")(GameInfo::Version).drawAt(Scene::Size().moveBy(-versionSize));
    }
};



///////////////////////////////////////////////////////////////////////
//
//  設定画面
//
class Config : public MyApp::Scene
{
    const Circle m_back = Circle(Scene::Center().x, 630, 60);
    
    CheckBox m_romaji = CheckBox(100, 150, true, U"ローマ字を表示する", FontAsset(U"MenuSmall"));
    CheckBox m_keyboard = CheckBox(100, 250, true, U"キーボードを表示する", FontAsset(U"MenuSmall"));
    CheckBox m_fingers = CheckBox(150, 350, true, U"指を表示する", FontAsset(U"MenuSmall"));

    void saveConfig()
    {
        getData().config.showRomaji = m_romaji;
        getData().config.showKeyboard = m_keyboard;
        getData().config.showFingers = m_fingers;
        Serializer<BinaryWriter>{GameInfo::ConfigFilePath}(getData().config);
    }
    
    void drawGameScreen() const
    {
        Transformer2D t2(Mat3x2::Translate(600, 100));
        Transformer2D t1(Mat3x2::Scale(0.5));
        
        Rect(0, 0, 1280, 720).drawFrame();

        if (m_keyboard)
        {
            drawKeyboard(115, m_fingers ? 280 : 340, { U'B' });
            
            if (m_fingers)
            {
                drawFingers(U'B');
                Rect(0, 720, 1280, 740).draw(GameInfo::BackgroundColor);
            }
        }
        
        const int32 strY = m_fingers ? 140 : 180;
        FontAsset(U"Text")(U"ぶんしょう").draw(115, strY);
        if (m_romaji)
            FontAsset(U"SmallText")(U"  BU     N       SHO        U").draw(115, strY + 60);
        
        FontAsset(U"GameScore")(U"単語数: 0 / 0").draw(115, 40);
        FontAsset(U"GameScore")(U"時間: 00'00").draw(500, 40);
        
        if (!m_keyboard)
            TextureAsset(U"Tori").draw(850, 400);
    }
    
public:
    
    Config(const InitData& init)
    : IScene(init)
    {
        m_romaji.setChecked(getData().config.showRomaji);
        
        m_keyboard.setChecked(getData().config.showKeyboard);
        const auto&& updateCheck = [&] (const bool checked) {
            m_fingers.setChecked(checked);
            m_fingers.setEnabled(checked);
        };
        m_keyboard.setOnChangeHandler(updateCheck);
        updateCheck(m_keyboard);

        m_fingers.setChecked(getData().config.showFingers);
    }
    
    void update()
    {
        m_romaji.update();
        m_keyboard.update();
        m_fingers.update();
        
        if (m_back.leftClicked())
        {
            saveConfig();
            changeScene(U"Title");
        }
        
        if (m_back.mouseOver())
            Cursor::RequestStyle(CursorStyle::Hand);
    }
    
    void draw() const
    {
        drawGameScreen();
        
        m_romaji.draw();
        m_keyboard.draw();
        m_fingers.draw();

        m_back.draw();
        FontAsset(U"MenuSmall")(U"戻る").drawAt(m_back.center, Palette::Black);
    }
};




///////////////////////////////////////////////////////////////////////
//
//  選択画面
//
class Select : public MyApp::Scene
{
    bool m_advanced = false;
    Array<char32> m_allowedChars;

    Array<CheckBox> m_lineCheckboxes =
    {
        CheckBox(80, 195, true),
        CheckBox(80, 275, true),
        CheckBox(80, 355, true),
        CheckBox(80, 435, true),
    };
    Circle m_start = Circle(Scene::Center().x + 70, 630, 60);
    Circle m_back = Circle(Scene::Center().x - 70, 630, 60);

    void saveLineSelection()
    {
        m_lineCheckboxes.each_index([&] (const int32 i, const auto& checkbox) { getData().config.activeLines[i] = checkbox; });
        Serializer<BinaryWriter>{GameInfo::ConfigFilePath}(getData().config);
    }
    
public:
    
    Select(const InitData& init)
    : IScene(init)
    {
        const auto onChange = [&] (const bool _) {
            m_allowedChars.clear();
            m_lineCheckboxes.each_index([&] (const int32 i, const auto& _checkbox) {
                if (_checkbox)
                    m_allowedChars.append(qwerty[i]);
            });
        };
        
        m_lineCheckboxes.each_index([&] (const int32 i, auto& checkbox) {
            checkbox.setChecked(getData().config.activeLines[i]);
            checkbox.setOnChangeHandler(onChange);
        });
        
        onChange(true);
    }
    
    void update()
    {
        m_lineCheckboxes.each([&] (auto& checkbox) { checkbox.update(); });
        
        if (m_start.leftClicked())
        {
            getData().allowedChars = m_allowedChars;
            
            saveLineSelection();
            changeScene(U"Game", 2000);
        }
        if (m_back.leftClicked())
        {
            saveLineSelection();
            changeScene(U"Title");
        }
        
        if (m_start.mouseOver() || m_back.mouseOver())
            Cursor::RequestStyle(CursorStyle::Hand);
    }
    
    void draw() const
    {
        FontAsset(U"Menu")(U"チェックが入っている列を練習します").drawAt(Scene::Center().x, 80);

        m_lineCheckboxes.each([&] (const auto& checkbox) { checkbox.draw(); });

        drawKeyboard(160, 190, m_allowedChars);
        
        m_start.draw(Palette::White);
        FontAsset(U"MenuSmall")(U"始める").drawAt(m_start.center, Palette::Black);
        m_back.draw(Palette::White);
        FontAsset(U"MenuSmall")(U"戻る").drawAt(m_back.center, Palette::Black);
    }
};


///////////////////////////////////////////////////////////////////////
//
//  ゲーム
//
class Game : public MyApp::Scene
{
    Typing m_typing;
    WordFactory m_factory;
    
    bool m_timeattack;
    int32 m_wordCount = 0, m_correctTypeCount = 0, m_typoCount = 0;
    
    Stopwatch m_countDownTimer;
    Stopwatch m_gameTimer;

    Effect m_effect;
    
    bool onCountDown() const
    {
        return m_countDownTimer.isRunning() && m_countDownTimer < 4000ms;
    }
    
    bool onGame() const
    {
        return m_gameTimer.isRunning();
    }
    
public:
    
    Game(const InitData& init)
    : IScene(init),
    m_factory(getData().type, U"words.txt", getData().allowedChars),
    m_timeattack(getData().isTimeAttack)
    {
        getData().lastScore = ScoreData();
        m_typing.setCorrectWordHandler([&] { m_wordCount++; });
        m_typing.setNextWordHandler([&] { return m_timeattack ? (m_wordCount < GameInfo::GameNumberOfWords ? m_factory.nextWord() : U" ") : m_factory.nextWord(); });
        
        m_factory.loadWords();
    }
    
    void update() override
    {
        if (KeyEscape.down())
        {
            changeScene(U"Title", 2000);
        }
        
        if (!onGame() && !m_countDownTimer.isRunning())
        {
            m_countDownTimer.start();
        }
        
        if (!onGame() && m_countDownTimer >= 3000ms)
        {
            m_gameTimer.start();
        }
        
        if (!onGame())
        {
            return;
        }
        
        if ((m_timeattack && m_wordCount >= GameInfo::GameNumberOfWords) || (!m_timeattack && m_gameTimer.ms() >= GameInfo::GameTimeMillisec))
        {
            getData().lastScore = ScoreData{ m_gameTimer.ms(), m_wordCount, m_correctTypeCount, m_typoCount, Date::Today(), getData().type == WordType::Random, m_timeattack };
            changeScene(U"Result", 2000);
        }
        
        
        Array<char32> m_typedChars;
        m_typedChars = KeyboardInput::getInputChars();
        
        m_typing.update();
        
        char32 lastTypo = 0;
        for (const auto s : m_typedChars)
        {
            if (m_typing.onCharInput(s))
            {
                m_correctTypeCount++;
            }
            else
            {
                lastTypo = s;
                m_typoCount++;
            }
        }
        
        qwerty.each_index([&] (const int32 i, const auto& line)
        {
            line.each_index([&] (const int32 j, const char32 c)
            {
                if (c == lastTypo)
                {
                    int32 x, y;

                    if (getData().config.showFingers)
                    {
                        x = 115 + 35 + j * 80 + i * 40;
                        y = 280 + 35 + i * 80;
                    }
                    else if (getData().config.showKeyboard)
                    {
                        x = 115 + 35 + j * 80 + i * 40;
                        y = 340 + 35 + i * 80;
                    }
                    else
                    {
                        auto [offsetx, offsety] = m_typing.getNextCharPos();
                        x = 115 + offsetx;
                        y = 200 + offsety;
                    }
                    
                    m_effect.add<TypoEffect>(x, y);
                }
            });
        });
        lastTypo = 0;
    }
    
    void draw() const override
    {
        if (onCountDown())
        {
            const int32 timeMillisec = Max((3999 - m_countDownTimer.ms()), 0);
            const int32 countDown = timeMillisec / 1000;
            const double e = EaseIn(Easing::Expo, (timeMillisec % 1000) / 1000.0);
            
            if (countDown > 0)
            {
                Transformer2D t(Mat3x2::Scale(1.0 + e * 2, Scene::Center()));
                FontAsset(U"CountDown")(countDown).drawAt(Scene::Center());
            }
            else
            {
                Transformer2D t(Mat3x2::Scale(1.0 + (1.0 - e) * 2, Scene::Center()));
                FontAsset(U"CountDown")(U"スタート").drawAt(Scene::Center(), AlphaF(e));
            }
        }
        
        if (!onGame())
        {
            return;
        }
        
        const char32 nextChar = m_typing.getNextChar();
        if (getData().config.showKeyboard)
        {
            drawKeyboard(115, getData().config.showFingers ? 280 : 340, { nextChar });
            
            if (getData().config.showFingers)
                drawFingers(nextChar);
        }
        
        const int32 strY = getData().config.showFingers ? 170 : 200;
        m_typing.draw(115, strY, true, getData().config.showRomaji);
        
        if (m_timeattack)
        {
            const int32 timeMillisec = m_gameTimer.ms();
            FontAsset(U"GameScore")(U"単語数: {} / {}"_fmt(m_wordCount, GameInfo::GameNumberOfWords)).draw(115, 40);
            FontAsset(U"GameScore")(U"時間: {:0>2}'{:0>2}"_fmt(timeMillisec / 1000, timeMillisec % 1000 / 10)).draw(500, 40);
        }
        else
        {
            const int32 timeLeftMillisec = Max(GameInfo::GameTimeMillisec - m_gameTimer.ms(), 0);
            FontAsset(U"GameScore")(U"単語数: {}"_fmt(m_wordCount)).draw(115, 40);
            FontAsset(U"GameScore")(U"時間: {:0>2}'{:0>2}"_fmt(timeLeftMillisec / 1000, timeLeftMillisec % 1000 / 10)).draw(500, 40);
        }
        
        const Size versionSize = FontAsset(U"Version")(U"escキーでタイトルへ").region().size.asPoint();
        FontAsset(U"Version")(U"escキーでタイトルへ").drawAt(Scene::Size().moveBy(-versionSize));

        
        if (!getData().config.showKeyboard)
            TextureAsset(U"Tori").draw(850, 400);
        
        m_effect.update();
    }
};

///////////////////////////////////////////////////////////////////////
//
//  結果画面
//
class Result : public MyApp::Scene
{
private:
    
    std::array<std::array<ScoreData, 5>, 5> m_highScores = defaultHighScores;
    
    const Circle titleButton = Circle(Scene::Center().x, 630, 60);
    
public:
    
    Result(const InitData& init)
    : IScene(init)
    {
        if (FileSystem::Exists(GameInfo::SaveFilePath))
        {
            try {
                Deserializer<BinaryReader>{GameInfo::SaveFilePath}(m_highScores);
            } catch (cereal::Exception e) {
                Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
            }
        }
        else
        {
            Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
        }
        
        bool modified = false;
        
        const auto updateScores = [&] (std::array<ScoreData, 5>& scores) {
            if (scores.back().score() <= getData().lastScore.score())
            {
                scores.back() = getData().lastScore;
                
                std::sort(scores.begin(), scores.end(), [] (const ScoreData& a, const ScoreData& b) {
                    return a.score() > b.score() ? true : a.score() == b.score() ? a.date > b.date : false;
                });
                
                modified = true;
            }
        };
        updateScores(m_highScores[0]);
        
        const int32 type_index = getData().type == WordType::Random ? 0 : 1;
        const int32 timeattack_index = getData().isTimeAttack ? 0 : 2;
        updateScores(m_highScores[1 + type_index + timeattack_index]);

        if (modified)
            Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
    }
    
    void update() override
    {
        if (titleButton.leftClicked() || KeyEscape.down())
        {
            changeScene(U"Title");
        }
        
        if (titleButton.mouseOver())
        {
            Cursor::RequestStyle(CursorStyle::Hand);
        }
    }
    
    void draw() const override
    {
        FontAsset(U"Result")(U"結果").drawAt(Scene::Center().x, 80);
        
        const auto& score = getData().lastScore;
        FontAsset(U"GameScore")(U"単語数:").draw(Arg::topRight = Vec2(Scene::Center().x - 20, 180));
        FontAsset(U"GameScore")(score.wordCount, U"語").draw(Scene::Center().x + 20, 180);
        FontAsset(U"GameScore")(U"時間:").draw(Arg::topRight = Vec2(Scene::Center().x - 20, 240));
        FontAsset(U"GameScore")(U"{:0>2}'{:0>2}"_fmt(score.timeMillisec / 1000, score.timeMillisec % 1000 / 10)).draw(Scene::Center().x + 20, 240);
        FontAsset(U"GameScore")(U"正しいタイプ数:").draw(Arg::topRight = Vec2(Scene::Center().x - 20, 300));
        FontAsset(U"GameScore")(score.correctTypeCount,U" type",
                                score.correctTypeCount == 1 ? U"" : U"s").draw(Scene::Center().x + 20, 300);
        FontAsset(U"GameScore")(U"ミスタイプ数:").draw(Arg::topRight = Vec2(Scene::Center().x - 20, 360));
        FontAsset(U"GameScore")(score.typoCount, U" type",
                                score.typoCount == 1 ? U"" : U"s").draw(Scene::Center().x + 20, 360);
        FontAsset(U"GameScore")(U"タイプの速さ:").draw(Arg::topRight = Vec2(Scene::Center().x - 20, 420));
        FontAsset(U"GameScore")(U"{:.3g}"_fmt(score.speed()), U" types/s").draw(Scene::Center().x + 20, 420);
        FontAsset(U"GameScore")(U"スコア:").draw(Arg::topRight = Vec2(Scene::Center().x - 20, 480));
        FontAsset(U"GameScore")(score.score()).draw(Scene::Center().x + 20, 480);

        titleButton.draw();
        FontAsset(U"MenuSmall")(U"戻る").drawAt(titleButton.center, Palette::Black);
    }
};

///////////////////////////////////////////////////////////////////////
//
//  ハイスコア一覧
//
class Score : public MyApp::Scene
{
    const Array<String> tab_texts =
    {
        U"総合",
        U"20問ポジション",
        U"20問文章・単語",
        U"2分ポジション",
        U"2分文章・単語",
    };
    Array<RoundRect> tabs;
    int32 m_page = 0;

    std::array<std::array<ScoreData, 5>, 5> m_highScores = defaultHighScores;

    const Circle titleButton = Circle(Scene::Center().x, 630, 60);
    
    static String getOrdinal(const int32 n)
    {
        switch (n) {
            case 1: return U"1st ";
            case 2: return U"2nd ";
            case 3: return U"3rd ";
            default: return U"{}th "_fmt(n);
        }
    }
    static const Color& getOrdinalColor(const int32 n)
    {
        switch (n) {
            case 1: return Palette::Gold;
            case 2: return Palette::Silver;
            case 3: return Palette::Brown;
            default: return Palette::White;
        }
    }

public:
    
    Score(const InitData& init)
    : IScene(init)
    {
        if (FileSystem::Exists(GameInfo::SaveFilePath))
        {
            try {
                Deserializer<BinaryReader>{GameInfo::SaveFilePath}(m_highScores);
            } catch (cereal::Exception e) {
                Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
            }
        }
        else
        {
            Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
        }
        
        tabs.resize(5);
        for (const auto i : step(5))
        {
            tabs[i] = RoundRect(20 + i * 210, 20, 200, 60, 30);
        }
    }
    
    void update() override
    {
        if (KeyEscape.down() || titleButton.leftClicked())
        {
            changeScene(U"Title");
        }

        bool hover = false;
        tabs.each_index([&] (const int32 i, const auto& shape) {
            hover |= shape.mouseOver();
            
            if (shape.leftClicked())
            {
                m_page = static_cast<int32>(i);
                return;
            }
        });
        
        if (titleButton.mouseOver())
        {
            hover = true;
        }
        
        if (hover)
        {
            Cursor::RequestStyle(CursorStyle::Hand);
        }
    }
    
    void draw() const override
    {
        const int32 h = FontAsset(U"ScoreList").height();
        const int32 dh = FontAsset(U"ScoreListDetail").height();

        tabs.each_index([&] (const int32 i, const auto& shape) {
            const bool selected = i == m_page;
            
            if (selected)
            {
                shape.draw(Palette::White);
            }
            else
            {
                shape.stretched(2).draw(Palette::White);
                shape.draw(GameInfo::BackgroundColor);
            }
            
            FontAsset(U"MenuSmall")(tab_texts[i]).drawAt(shape.center(), Palette::Black);
        });
        
        const auto& scores = m_highScores[m_page];
        for (int32 i = 0, n = static_cast<int32>(scores.size()) ; i < n ; i++) {
            const auto& score = scores[i];
            const Rect rect = Rect(1000, 80).setCenter(Scene::Center().x, static_cast<int32>(145 + i * 90));
            rect.draw(ColorF(1.0, 0.2));
            
            FontAsset(U"ScoreList")(getOrdinal(static_cast<int32>(i + 1))).draw(rect.pos + Point(20, (rect.h - h) / 2), getOrdinalColor(i + 1));
            
            if (score.score() >= 0)
            {
                FontAsset(U"ScoreList")(score.score()).draw(rect.pos + Point(140, (rect.h - h) / 2));
                
                FontAsset(U"ScoreListDetail")(score.isRandom ? U"ポジション" : U"文章・単語")
                .draw(rect.pos + Point(260, rect.h / 4 - dh / 2));
                FontAsset(U"ScoreListDetail")(U"{:.3g}"_fmt(score.speed()), U" types/s")
                .draw(rect.pos + Point(260, rect.h * 3 / 4 - dh / 2));

                FontAsset(U"ScoreListDetail")(U"単語数: ", score.wordCount, U"語")
                .draw(rect.pos + Point(420, rect.h / 4 - dh / 2));
                FontAsset(U"ScoreListDetail")(U"時間: {:0>2}'{:0>2}"_fmt(score.timeMillisec / 1000, score.timeMillisec % 1000 / 10))
                .draw(rect.pos + Point(420, rect.h * 3 / 4 - dh / 2));
                
                FontAsset(U"ScoreListDetail")(U"正しいタイプ数: ", score.correctTypeCount, U" type", score.correctTypeCount == 1 ? U"" : U"s")
                .draw(rect.pos + Point(580, rect.h / 4 - dh / 2));
                FontAsset(U"ScoreListDetail")(U"ミスタイプ数: ", score.typoCount, U" type", score.typoCount == 1 ? U"" : U"s")
                .draw(rect.pos + Point(580, rect.h * 3 / 4 - dh / 2));
                
                const Size& dateSize = FontAsset(U"ScoreListDetail")(scores[i].date).region().size.asPoint();
                FontAsset(U"ScoreListDetail")(score.date)
                .draw(rect.br().x - dateSize.x - 40, rect.center().y - dateSize.y / 2);
            }
            else
            {
                FontAsset(U"ScoreList")(U"-").draw(rect.pos + Point(140, (rect.h - h) / 2));
            }
        }
        
        titleButton.draw();
        FontAsset(U"MenuSmall")(U"戻る").drawAt(titleButton.center, Palette::Black);
    }
};

///////////////////////////////////////////////////////////////////////
//
//  スタッフクレジット
//
class Credit : public MyApp::Scene
{
    Array<std::tuple<String, double, bool>> m_credits;
    
    double m_height = 0.0;
    
    Stopwatch m_stopwatch{ StartImmediately::Yes };
    
    double yOffset() const
    {
        return Scene::Height() + 60 - m_stopwatch.ms() / 20.0;
    }
    
public:
    
    Credit(const InitData& init)
    : IScene(init)
    {
        double y = 0;
        
        for (const auto& credit : GameInfo::Credits)
        {
            m_credits.emplace_back(credit.first, y, true);
            y += 70;
            
            for (const auto& name : credit.second)
            {
                m_credits.emplace_back(name, y, false);
                y += 60;
            }
            
            y += 60;
        }
        
        m_height = y;
    }
    
    void update() override
    {
        if ((MouseL | KeyEscape).down())
        {
            changeScene(U"Title");
        }
        
        if ((m_height + yOffset()) < 0)
        {
            m_stopwatch.restart();
        }
    }
    
    void draw() const override
    {
        const double offset = yOffset();
        
        for (const auto& credit : m_credits)
        {
            FontAsset(std::get<bool>(credit) ? U"CreditBig" : U"CreditSmall")(std::get<String>(credit))
            .drawAt(Scene::Center().x, std::get<double>(credit) + offset);
        }
    }
};

///////////////////////////////////////////////////////////////////////
//
//  メイン関数
//
void Main()
{
    Window::Resize(1280, 720);
    Window::SetTitle(GameInfo::Title);
    Scene::SetBackground(GameInfo::BackgroundColor);
    
    //////////////////////////////////////////////////////////////
    //
    //  使用するシーン
    //
    MyApp manager;
    manager.setFadeColor(GameInfo::FadeInColor);
    manager.add<Title>(U"Title");
    manager.add<Select>(U"Select");
    manager.add<Config>(U"Config");
    manager.add<Game>(U"Game");
    manager.add<Result>(U"Result");
    manager.add<Score>(U"Score");
    manager.add<Credit>(U"Credit");
    
    //////////////////////////////////////////////////////////////
    //
    //  使用するフォント
    //
    FontAsset::Register(U"Title", GameInfo::TitleFontSize, Typeface::Heavy);
    FontAsset::Register(U"Menu", 30, Typeface::Bold);
    FontAsset::Register(U"MenuLarge", 40, Typeface::Bold);
    FontAsset::Register(U"MenuSmall", 24, Typeface::Bold);
    FontAsset::Register(U"Version", 14, Typeface::Regular);
    
    FontAsset::Register(U"CountDown", 72, Typeface::Bold);
    FontAsset::Register(U"Text", 50, Typeface::Heavy);
    FontAsset::Register(U"SmallText", 20, Typeface::Heavy);
    
    FontAsset::Register(U"Result", 80, Typeface::Bold);
    FontAsset::Register(U"ResultButton", 32, Typeface::Regular);
    
    FontAsset::Register(U"GameScore", 40, Typeface::Light);
    FontAsset::Register(U"ScoreList", 50, Typeface::Heavy);
    FontAsset::Register(U"ScoreListDetail", 20, Typeface::Regular, FontStyle::Italic);
    
    FontAsset::Register(U"CreditBig", 32, Typeface::Bold);
    FontAsset::Register(U"CreditSmall", 28, Typeface::Regular);
    
    //////////////////////////////////////////////////////////////
    //
    //  使用するテクスチャ
    //
    TextureAsset::Register(U"Tori", Resource(U"textures/tori.png"));
    
    //////////////////////////////////////////////////////////////
    //
    //  メインループ
    //
    while (System::Update())
    {
        if (!manager.update())
        {
            break;
        }
    }
}
