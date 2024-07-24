#include "NodeSetFloat.hpp"

namespace Nodes
{
    NodeSetFloat::NodeSetFloat(const std::string name, float initValue)
    {
        if (name == "")
        {
            setTitle("Set float");
        }
        else
        {
            setTitle(name);
        }

        value1 = initValue;
        value2 = initValue;

        setStyle(ImFlow::NodeStyle::green());
        addOUT<float>("OUT1")
            ->behaviour([this]()
                        { return value1; });
        addOUT<float>("OUT2")
            ->behaviour([this]()
                        { return value2; });
    }

    void NodeSetFloat::draw()
    {
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("##Val1", &value1, 0.1, 1.0, "%.1f");
        ImGui::SetNextItemWidth(100.f);
        ImGui::InputFloat("##Val2", &value2, 0.1, 1.0, "%.1f");
    }
}