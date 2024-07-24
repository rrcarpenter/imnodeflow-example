// NodeSetFloat.hpp
#ifndef NODES_SET_FLOAT_H // include guard
#define NODES_SET_FLOAT_H

#include "../../ImNodeFlow/include/ImNodeFlow.h"

namespace Nodes
{
    class NodeSetFloat : public ImFlow::BaseNode
    {
    public:
        explicit NodeSetFloat(const std::string name, float initValue);

        void draw() override;

    private:
        float value1 = 0.0;
        float value2 = 0.0;
    };

} // namespace Nodes
#endif // NODES_SET_FLOAT_H