#pragma once

namespace Quasar
{
    
    class Timestep
    {
    private:
        float m_Time; // time is in seconds

    public:
        Timestep(float time)
            : m_Time(time)
        {
        }

        operator float() const { return m_Time; }

        float getSeconds() const { return m_Time; }
        float getMilliseconds() const { return m_Time * 1000.0f; }
    };

} // namespace Quasar
