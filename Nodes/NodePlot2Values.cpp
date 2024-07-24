#include "NodePlot2Values.hpp"
#include "../../implot/implot.h"
#include "../../implot/implot_internal.h"

namespace Nodes
{

    NodePlot2Values::NodePlot2Values()
    {
        setTitle("Plot Node");
        setStyle(ImFlow::NodeStyle::green());
        addIN<float>("IN_VAL1", 0.0, ImFlow::ConnectionFilter::SameType());
        addIN<float>("IN_VAL2", 0.0, ImFlow::ConnectionFilter::SameType());
    }

    void NodePlot2Values::draw()
    {
        float inVal1 = getInVal<float>("IN_VAL1");
        float inVal2 = getInVal<float>("IN_VAL2");
        ImGui::Text("In %.2f/%.2f", inVal1, inVal2);

        // Based on https://github.com/epezent/implot/blob/master/implot_demo.cpp
        static ScrollingBuffer sdata1, sdata2;
        static RollingBuffer rdata1, rdata2;
        static float t = 0;

        t += ImGui::GetIO().DeltaTime;
        sdata1.AddPoint(t, inVal1);
        rdata1.AddPoint(t, inVal1);
        sdata2.AddPoint(t, inVal2);
        rdata2.AddPoint(t, inVal2);

        static float history = 30.0f;
        static int stride = 4;
        // ImGui::SliderFloat("History", &history, 1, 30, "%.1f s");
        // ImGui::SliderInt("Stride", &stride, 4, 128, "%d");
        rdata1.Span = history;
        rdata2.Span = history;

        static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
        if (ImPlot::BeginSubplots("Plot2Values", 2, 1, ImVec2(300, 300)))
        {
            if (ImPlot::BeginPlot("##Scrolling", ImVec2(0, 0)))
            {
                ImPlot::SetupAxes(NULL, NULL, flags, flags);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, t, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100);
                ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
                ImPlot::PlotLine("IN_VAL1", &sdata1.Data[0].x, &sdata1.Data[0].y, sdata1.Data.size(), 0, sdata1.Offset, sizeof(sdata1.Data[0]));
                ImPlot::PlotLine("IN_VAL2", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, sizeof(sdata2.Data[0]));
                ImPlot::EndPlot();
            }
            if (ImPlot::BeginPlot("##Rolling", ImVec2(0, 0)))
            {
                ImPlot::SetupAxes(NULL, NULL, flags, flags);
                ImPlot::SetupAxisLimits(ImAxis_X1, 0, history, ImGuiCond_Always);
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 100);
                ImPlot::PlotLine("IN_VAL1", &rdata1.Data[0].x, &rdata1.Data[0].y, rdata1.Data.size(), 0, 0, sizeof(rdata1.Data[0]));
                ImPlot::PlotLine("IN_VAL2", &rdata2.Data[0].x, &rdata2.Data[0].y, rdata2.Data.size(), 0, 0, sizeof(rdata2.Data[0]));
                ImPlot::EndPlot();
            }
            ImPlot::EndSubplots();
        }
    }
}