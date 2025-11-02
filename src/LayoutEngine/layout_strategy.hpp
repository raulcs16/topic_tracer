#pragma once

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>

class LayoutStrategy {
public:
    virtual ~LayoutStrategy() = default;
    void apply(ogdf::GraphAttributes &GA) {
        if (m_layout)
            m_layout->call(GA);
    }

protected:
    ogdf::LayoutModule *m_layout = nullptr;
};