#pragma once

template <typename T> class Array2D
{
public:
    Array2D()
    {

    }

    Array2D(u32 _width, u32 _height, const T * _data = nullptr) :
        m_width(_width),
        m_height(_height)
    {
        resize();

        if (_data)
        {
            for (u32 j = 0; j < _height; ++j)
            {
                for (u32 i = 0; i < _width; ++i)
                {
                    const u32 offset = j * _width + i;
                    m_data[offset] = _data[offset];
                }
            }
        }
    }

    const T & get(u32 _x, u32 _y)
    {
        return m_data[_x + _y * m_width];
    }

    void set(u32 _x, u32 _y, const T & _val)
    {
        m_data[_x + _y * m_width] = _val;
    }

    const T * data() const
    {
        return m_data.data();
    }

    u32 rotateX(u32 _x)
    {
        return u32((int)_x % (int)m_width);
    }

    u32 rotateY(u32 _y)
    {
        return u32((int)_y % (int)m_height);
    }

    void translate(i32 _x, i32 _y)
    {
        Array2D<T> temp(m_width, m_height);
        temp.resize();

        for (u32 j = 0; j < m_height; ++j)
        {
            for (u32 i = 0; i < m_width; ++i)
            {
                const T val = this->get(rotateX((i32)i+_x), rotateY((i32)j+_y));
                temp.set(i, j, val);
            }
        }

        m_data = std::move(temp.m_data);
    }

    void SetSize(u32 width, u32 height)
    {
        m_width = width;
        m_height = height;
        resize();
    }

    u32 Width() const { return m_width; }
    u32 Height() const { return m_height; }

protected:
    void resize()
    {
        m_data.resize(m_width*m_height);
    }

private:
    u32            m_width = 0;
    u32            m_height = 0;
    std::vector<T> m_data;
};