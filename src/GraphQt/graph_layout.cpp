/*
 * Copyright (c) 2013 Christoph Schulz
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details: http://www.gnu.org/copyleft/lesser
 */
#include "graph_layout.hpp"
#include <QQmlInfo>
#include <ogdf/basic/PreprocessorLayout.h>
#include <ogdf/energybased/DavidsonHarelLayout.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/energybased/FastMultipoleEmbedder.h>
#include <ogdf/energybased/GEMLayout.h>
#include <ogdf/energybased/MultilevelLayout.h>
// #include <ogdf/energybased/SpringEmbedderFR.h>
#include <ogdf/energybased/SpringEmbedderFRExact.h>
#include <ogdf/energybased/SpringEmbedderKK.h>
#include <ogdf/energybased/StressMinimization.h>
#include <ogdf/energybased/TutteLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MMMExampleFastLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MMMExampleNiceLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MMMExampleNoTwistLayout.h>
// #include <ogdf/energybased/multilevel_mixer/MixedForceLayout.h>
#include <ogdf/energybased/multilevel_mixer/ModularMultilevelMixer.h>
#include <ogdf/energybased/multilevel_mixer/ScalingLayout.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/misclayout/BalloonLayout.h>
#include <ogdf/misclayout/CircularLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>
#include <ogdf/planarity/PlanarizationGridLayout.h>
#include <ogdf/planarity/PlanarizationLayout.h>
#include <ogdf/planarlayout/FPPLayout.h>
#include <ogdf/planarlayout/MixedModelLayout.h>
#include <ogdf/planarlayout/PlanarDrawLayout.h>
#include <ogdf/planarlayout/PlanarStraightLayout.h>
#include <ogdf/planarlayout/SchnyderLayout.h>
#include <ogdf/tree/RadialTreeLayout.h>
#include <ogdf/tree/TreeLayout.h>
#include <ogdf/upward/DominanceLayout.h>
#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include <ogdf/upward/VisibilityLayout.h>

#define CREATE_MODULE(name)                                                              \
    case name: layout = new ogdf::name(); break

GraphLayout::GraphLayout(ogdf::GraphAttributes &attributes, QObject *parent)
    : QObject(parent), m_attributes(&attributes), m_layout(new ogdf::FMMMLayout()),
      m_algorithm(GraphLayout::FMMMLayout), m_valid(false) {}

GraphLayout::Algorithm GraphLayout::algorithm() const { return m_algorithm; }

void GraphLayout::setAlgorithm(Algorithm algorithm) {
    if (algorithm == m_algorithm) {
        return;
    }
    // Create layout module by name.
    ogdf::LayoutModule *layout = 0;
    switch (algorithm) {
        CREATE_MODULE(BalloonLayout);
        CREATE_MODULE(CircularLayout);
        CREATE_MODULE(ComponentSplitterLayout);
        CREATE_MODULE(DavidsonHarelLayout);
        CREATE_MODULE(DominanceLayout);
        CREATE_MODULE(FMMMLayout);
        CREATE_MODULE(FPPLayout);
        CREATE_MODULE(FastMultipoleEmbedder);
        CREATE_MODULE(FastMultipoleMultilevelEmbedder);
        CREATE_MODULE(GEMLayout);
        // CREATE_MODULE(MMMExampleFastLayout);
        // CREATE_MODULE(MMMExampleNiceLayout);
        // CREATE_MODULE(MMMExampleNoTwistLayout);
        // CREATE_MODULE(MixedForceLayout);
        CREATE_MODULE(MixedModelLayout);
        CREATE_MODULE(ModularMultilevelMixer);
        CREATE_MODULE(MultilevelLayout);
        CREATE_MODULE(PlanarDrawLayout);
        CREATE_MODULE(PlanarStraightLayout);
        CREATE_MODULE(PlanarizationGridLayout);
        CREATE_MODULE(PlanarizationLayout);
        CREATE_MODULE(PreprocessorLayout);
        CREATE_MODULE(RadialTreeLayout);
        CREATE_MODULE(ScalingLayout);
        CREATE_MODULE(SchnyderLayout);
        // CREATE_MODULE(SpringEmbedderFR);
        CREATE_MODULE(SpringEmbedderFRExact);
        CREATE_MODULE(SpringEmbedderKK);
        CREATE_MODULE(StressMinimization);
        CREATE_MODULE(SugiyamaLayout);
        CREATE_MODULE(TreeLayout);
        CREATE_MODULE(TutteLayout);
        CREATE_MODULE(UpwardPlanarizationLayout);
        CREATE_MODULE(VisibilityLayout);
    }
    if (layout) {
        m_layout.reset(layout);
        m_algorithm = algorithm;
        invalidate();
        emit algorithmChanged();
    }
}

bool GraphLayout::valid() const { return m_valid; }

void GraphLayout::call() {
    if (m_valid) {
        return;
    }
    try {
        m_layout->call(*m_attributes);
        m_valid = true;
        emit validChanged();
    } catch (ogdf::AlgorithmFailureException &e) {
        QString reason =
            QString("of an unknown reason (%1)").arg(static_cast<int>(e.exceptionCode()));
        switch (e.exceptionCode()) {
        case ogdf::AlgorithmFailureCode::Unknown:
            // Do nothing.
            break;
        case ogdf::AlgorithmFailureCode::IllegalParameter:
            reason = "of an illegal parameter";
            break;
        case ogdf::AlgorithmFailureCode::NoFlow:
            reason = "min-cost flow solver could not find a legal flow";
            break;
        case ogdf::AlgorithmFailureCode::Sort: reason = "sequence is not sorted"; break;
        case ogdf::AlgorithmFailureCode::Label: reason = "labelling failed"; break;
        case ogdf::AlgorithmFailureCode::ExternalFace:
            reason = "external face is not correct";
            break;
        case ogdf::AlgorithmFailureCode::ForbiddenCrossing:
            reason = "crossing were forbidden";
            break;
        case ogdf::AlgorithmFailureCode::TimelimitExceeded:
            reason = "timelimit exceeded";
            break;
        case ogdf::AlgorithmFailureCode::NoSolution:
            reason = "it could not find a solution";
            break;
        case ogdf::AlgorithmFailureCode::STOP:
            // Do nothing.
            break;
        }
        qmlInfo(this) << "Layout algorithm failed, because " << reason;
    } catch (ogdf::Exception &) {
        qmlInfo(this) << "OGDF exception caught";
    } catch (...) {
        qmlInfo(this) << "Unknown exception caught";
    }
}

void GraphLayout::invalidate() {
    if (m_valid) {
        m_valid = false;
        emit validChanged();
    }
}