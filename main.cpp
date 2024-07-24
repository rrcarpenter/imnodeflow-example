#include "../imgui/imgui_internal.h"
#include "imgui_stdlib.h"
#include "AppImGuiCRTP.hpp"
#include "../ImNodeFlow/include/ImNodeFlow.h"
#include <fstream>
#include <iostream>
#include <sstream> // std::stringstream
#include <vector>
#include <tuple>
#include <chrono>
#include <iomanip>
#include "Nodes/NodePlot2Values.hpp"
#include "Nodes/NodeSetFloat.hpp"
#include "Nodes/NodeSetInt.hpp"

class App : public AppImGuiCRTP<App>
{
public:
    App() = default;
    ~App() = default;

    void StartUp()
    {
        printf("Startup\n");
    }

    void Update()
    {

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        ImGui::SetNextWindowSize(ImVec2(1002,674));
        ImGui::SetNextWindowPos(ImVec2(60, 60));
        ImGui::Begin("Nodes");
        static bool init_nodes = true;
        static std::shared_ptr<Nodes::NodePlot2Values> ptNodePlot;
        static std::shared_ptr<Nodes::NodeSetFloat> ptNodeTemperature;
        static std::shared_ptr<Nodes::NodeSetFloat> ptNodeSetpoint;
        if (init_nodes)
        {
            ptNodeTemperature = myNode.placeNodeAt<Nodes::NodeSetFloat>(ImVec2(30, 20), "Temperature", 25.1);
            ptNodeSetpoint = myNode.placeNodeAt<Nodes::NodeSetFloat>(ImVec2(30, 210), "Setpoint", 30.0);
            ptNodePlot = myNode.placeNodeAt<Nodes::NodePlot2Values>(ImVec2(580, 170));

            ptNodeTemperature->outPin("OUT1")->createLink(ptNodePlot->inPin("IN_VAL1"));
            ptNodeSetpoint->outPin("OUT2")->createLink(ptNodePlot->inPin("IN_VAL2"));

            init_nodes = false;
        }

        myNode.rightClickPopUpContent([this](ImFlow::BaseNode *node)
                                      {
            if (node != nullptr)
            {
                ImGui::Text("%lu", node->getUID());
                ImGui::Text("%s", node->getName().c_str());
                if (ImGui::Button("Ok"))
                {
                    ImGui::CloseCurrentPopup();
                }
                if (ImGui::Button("Delete Node"))
                {
                    node->destroy();
                    ImGui::CloseCurrentPopup();
                }
            }
            else
            {
                if (ImGui::Button("Add Node"))
                {
                    ImVec2 pos = ImGui::GetMousePos();
                    myNode.placeNodeAt<Nodes::NodeSetInt>(pos, "Test", 1);
                    ImGui::CloseCurrentPopup();
                }
            } });

        myNode.droppedLinkPopUpContent([this](ImFlow::Pin *dragged)
                                       { dragged->deleteLink(); });

        std::vector<std::weak_ptr<ImFlow::Link>> myLinks = myNode.getLinks();

        ImGui::Text("Links: %lu", myLinks.size());
        ImGui::SameLine();
        ImGui::Text("Nodes: %u", myNode.getNodesCount());

        for (auto wp : myLinks)
        {
            auto p = wp.lock();
            if (p == nullptr)
                continue;
            if (p->isHovered())
            {
                // ImGui::Text("Hovered");
            }
            if (p->isSelected() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                // ImGui::Text("Selected and Right Mouse click");
                ImFlow::Pin *right = p->right();
                ImFlow::Pin *left = p->left();
                right->deleteLink();
                left->deleteLink();
            }
        }

        myNode.update();
        ImGui::End();
    }

    void CloseEvent()
    {
        std::cout << std::endl
                  << "Name: " << myNode.getName() << std::endl;
        ImVec2 pos = myNode.getPos();
        std::cout << "pos x/y: " << pos.x << "/" << pos.y << std::endl;
        std::cout << "Node count: " << myNode.getNodesCount() << std::endl;

        for (const auto &it : myNode.getNodes())
        {
            std::cout << std::endl;
            uint64_t nodeId = it.first;
            ImFlow::BaseNode *ptNode = it.second.get();
            std::cout << "NodeId:" << nodeId << std::endl;
            std::cout << "Node Name: " << ptNode->getName() << std::endl;
            pos = ptNode->getPos();
            std::cout << "Pos x/y:" << pos.x << "/" << pos.y << std::endl;
            Nodes::NodePlot2Values *ptPlot = dynamic_cast<Nodes::NodePlot2Values *>(ptNode);
            Nodes::NodeSetFloat *ptSetFloat = dynamic_cast<Nodes::NodeSetFloat *>(ptNode);
            Nodes::NodeSetInt *ptSetInt = dynamic_cast<Nodes::NodeSetInt *>(ptNode);
            if (ptPlot != nullptr)
                std::cout << "Type is NodePlot2Values" << std::endl;
            if (ptSetInt != nullptr)
                std::cout << "Type is NodeSetInt" << std::endl;
            if (ptSetFloat != nullptr)
                std::cout << "Type is NodeSetFloat" << std::endl;
        }
    }

    void CleanUp()
    {
        printf("CleanUp\n");
    }

private:
    bool show_demo_window = true;
    bool show_another_window = false;
    ImFlow::ImNodeFlow myNode;
};

int main(int, char **)
{
    App app;
    app.Run();

    return 0;
}