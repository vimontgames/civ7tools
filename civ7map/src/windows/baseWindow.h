#pragma once

class BaseWindow
{
public:
    BaseWindow(const string & _name) : m_name(_name) {};
    virtual ~BaseWindow() {};
    virtual bool Draw(const sf::RenderWindow & window) = 0;

    bool IsVisible() const { return m_visible; }
    void SetVisible(bool _visible) { m_visible = _visible; }

    const string Name() const { return m_name; }

protected:
    static const uint s_checkboxColumnWidth = 128;

    string m_name;
    bool m_visible = true;
};