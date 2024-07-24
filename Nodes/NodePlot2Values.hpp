// NodePlot2Values.hpp
#ifndef NODES_PLOT_2_VALUES_H // include guard
#define NODES_PLOT_2_VALUES_H

#include "../../ImNodeFlow/include/ImNodeFlow.h"

namespace Nodes
{
    // Based on https://github.com/epezent/implot/blob/master/implot_demo.cpp
    // utility structure for realtime plot
    struct ScrollingBuffer
    {
        int MaxSize;
        int Offset;
        ImVector<ImVec2> Data;
        float y_prev;
        ScrollingBuffer(int max_size = 24 * 60 * 600)
        {
            MaxSize = max_size;
            Offset = 0;
            Data.reserve(MaxSize);
            y_prev = 0.0;
        }
        void AddPoint(float x, float y)
        {
            if (y == y_prev && Data.size() > 1)
            {
                int32_t id = Data.size() - 1;
                Data[id] = ImVec2(x, y_prev);
            }
            else
            {
                if (Data.size() < MaxSize)
                    Data.push_back(ImVec2(x, y));
                else
                {
                    Data[Offset] = ImVec2(x, y);
                    Offset = (Offset + 1) % MaxSize;
                }
            }
            y_prev = y;
        }
        void Erase()
        {
            if (Data.size() > 0)
            {
                Data.shrink(0);
                Offset = 0;
            }
        }
    };

    // Based on https://github.com/epezent/implot/blob/master/implot_demo.cpp
    // utility structure for realtime plot
    struct RollingBuffer
    {
        float Span;
        ImVector<ImVec2> Data;
        RollingBuffer()
        {
            Span = 10.0f;
            Data.reserve(2000);
        }
        void AddPoint(float x, float y)
        {
            float xmod = fmodf(x, Span);
            if (!Data.empty() && xmod < Data.back().x)
                Data.shrink(0);
            Data.push_back(ImVec2(xmod, y));
        }
    };

    class NodePlot2Values : public ImFlow::BaseNode
    {
    public:
        NodePlot2Values();

        void draw() override;

    private:
    };

} // namespace Nodes
#endif // NODES_PLOT_2_VALUES_H