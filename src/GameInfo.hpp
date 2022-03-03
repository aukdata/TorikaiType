# include <Siv3D.hpp>
# include <HamFramework.hpp>


///////////////////////////////////////////////////////////////////////
//
//  ゲームの基本情報
//
namespace GameInfo
{
    // ゲームのタイトル
    constexpr auto Title = U"TorikaiType";
    
    // ゲームのバージョン（空の文字列も OK）
    constexpr auto Version = U"Version 1.0";
    
    // ゲームの背景色
    constexpr ColorF BackgroundColor = ColorF(0.0, 0.7, 0.7);
    
    // シーン切り替え時のフェードイン・アウトの色
    constexpr ColorF FadeInColor = ColorF(1.0, 1.0, 1.0);
    
    // タイトルのフォントサイズ
    constexpr int32 TitleFontSize = 80;
    
    // メニューのフォントサイズ
    constexpr int32 MenuFontSize = 30;
    
    // 時間
    constexpr int32 GameTimeMillisec = 2 * 60 * 1000;
    
    // 問題数
    constexpr int32 GameNumberOfWords = 20;
    
    // セーブデータの保存場所
    const FilePath SaveFilePath = U"Save/Score.dat";
    
    // 設定データの保存場所
    const FilePath ConfigFilePath = U"Save/Config.dat";
    
    // スタッフロールのクレジット（項目は増減できる）
    const Array<std::pair<String, Array<String>>> Credits
    {
        { U"制作", { U"私" } },
        { U"ライブラリ", { U"OpenSiv3D" } },
        { U"Special Thanks", { U"You!" } },
    };
}
