#pragma once

class BaseWindow
{
public:
    BaseWindow(const std::string & _name) : m_name(_name) {};
    virtual ~BaseWindow() {};
    virtual bool Draw(const RenderWindow & window) = 0;

    bool IsVisible() const { return m_visible; }
    void SetVisible(bool _visible) { m_visible = _visible; }

    const std::string Name() const { return m_name; }

protected:
    std::string m_name;
    bool m_visible = true;
};