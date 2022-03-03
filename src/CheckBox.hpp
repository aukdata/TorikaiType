//
//  CheckBox.hpp
//  TorikaiType
//

# ifndef CheckBox_hpp
# define CheckBox_hpp

# include <Siv3D.hpp>
# include <functional>

class CheckBox
{
    bool m_checked = false;
    bool m_enabled = true;
    String m_label = U"";
    Font m_font;
    int32 m_height;

    RoundRect m_box;
    inline static const Polygon m_checkmark {
        {25, 35}, {5, 20}, {25, 45}, {55, 10}
    };
    
    std::function<void(bool checked)> m_onChangeHandler;
    
public:
    
    CheckBox(const double x, const double y, const bool checked = false, const String& label = U"", const Font& font = Font()) :
    m_checked(checked),
    m_label(label),
    m_font(font),
    m_height(m_font(m_label).region().h),
    m_box(RoundRect(x, y, 60, 60, 5)) {}
    
    void setOnChangeHandler(const std::function<void(bool checked)> &handler)
    {
        m_onChangeHandler = handler;
    }
    
    [[nodiscard]] bool isChecked() const { return m_checked; }
    void setChecked(const bool check) { m_checked = check; }
    
    [[nodiscard]] bool isEnabled() const { return m_enabled; }
    void setEnabled(const bool enabled) { m_enabled = enabled; }
    
    [[nodiscard]] operator bool() const
    {
        return isChecked();
    }
    
    void update()
    {
        if (m_enabled)
        {
            const auto rect = m_font(m_label).region(m_box.x + 70, m_box.y + 30 - m_height / 2);
            
            if (m_box.mouseOver() || rect.mouseOver())
            {
                Cursor::RequestStyle(CursorStyle::Hand);
            }
            
            if (m_box.leftClicked() || rect.leftClicked())
            {
                m_checked = !m_checked;
                if (m_onChangeHandler) m_onChangeHandler(m_checked);
            }
        }
    }
    
    void draw() const
    {
        m_box.draw(m_enabled ? Palette::White : Palette::Lightgrey);
        if (m_label.size() > 0)
        {
            m_font(m_label).draw(m_box.x + 70, m_box.y + 30 - m_height / 2, m_enabled ? Palette::White : Palette::Lightgrey);
        }
        
        if (m_checked)
        {
            m_checkmark.draw(m_box.x, m_box.y, m_enabled ? Palette::Green : Palette::Darkgray);
        }
    }
};

# endif /* CheckBox_hpp */
