#pragma once


//https: //github.com/schulzch/qml-ogdf/blob/master/ogdfplugin/graphlayout.h


#include <QObject>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/LayoutModule.h>

class GraphLayout : public QObject {
    Q_OBJECT
    Q_ENUMS(Algorithm)
    Q_PROPERTY(
        Algorithm algorithm READ algorithm WRITE setAlgorithm NOTIFY algorithmChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
public:
    enum Algorithm {
        BalloonLayout,
        CircularLayout,
        ComponentSplitterLayout,
        DavidsonHarelLayout,
        DominanceLayout,
        FMMMLayout,
        FPPLayout,
        FastMultipoleEmbedder,
        FastMultipoleMultilevelEmbedder,
        GEMLayout,
        MMMExampleFastLayout,
        MMMExampleNiceLayout,
        MMMExampleNoTwistLayout,
        MixedForceLayout,
        MixedModelLayout,
        ModularMultilevelMixer,
        MultilevelLayout,
        PlanarDrawLayout,
        PlanarStraightLayout,
        PlanarizationGridLayout,
        PlanarizationLayout,
        PreprocessorLayout,
        RadialTreeLayout,
        ScalingLayout,
        SchnyderLayout,
        SpringEmbedderFR,
        SpringEmbedderFRExact,
        SpringEmbedderKK,
        StressMinimization,
        SugiyamaLayout,
        TreeLayout,
        TutteLayout,
        UpwardPlanarizationLayout,
        VisibilityLayout
    };

    GraphLayout(ogdf::GraphAttributes &attributes, QObject *parent = 0);

    Algorithm algorithm() const;
    void setAlgorithm(Algorithm algorithm);

    bool valid() const;

    Q_INVOKABLE void call();

signals:
    void algorithmChanged();
    void validChanged();

public slots:
    void invalidate();

private:
    QScopedPointer<ogdf::LayoutModule> m_layout;
    ogdf::GraphAttributes *m_attributes;
    Algorithm m_algorithm;
    bool m_valid;
};