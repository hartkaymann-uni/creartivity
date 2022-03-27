import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "../../../libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: "../../.."

    ofApp {
        name: { return FileInfo.baseName(sourceDirectory) }

        files: [
            'src/Controls.txt',
            'src/ccReceiver.cpp',
            'src/ccReceiver.h',
            'src/ccUser.cpp',
            'src/ccUser.h',
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
            'src/scenes/ccScene.cpp',
            'src/scenes/ccScene.h',
            'src/scenes/contourlines/ContourLinesScene.cpp',
            'src/scenes/contourlines/ContourLinesScene.h',
            'src/scenes/contourlines/shader/contour.frag',
            'src/scenes/contourlines/shader/contour.vert',
            'src/scenes/contourlines/shader/contourLines.frag',
            'src/scenes/contourlines/shader/contourLines.vert',
            'src/scenes/cuboid/CuboidScene.cpp',
            'src/scenes/cuboid/CuboidScene.h',
            'src/scenes/cuboid/shader/cuboids.frag',
            'src/scenes/cuboid/shader/cuboids.vert',
            'src/scenes/gameoflife/GameOfLifeScene.cpp',
            'src/scenes/gameoflife/GameOfLifeScene.h',
            'src/scenes/gameoflife/shader/gol.frag',
            'src/scenes/gameoflife/shader/outline.frag',
            'src/scenes/gameoflife/shader/passthru.vert',
            'src/scenes/gameoflife/shader/renderInstanced.frag',
            'src/scenes/gameoflife/shader/renderInstanced.vert',
            'src/scenes/particles/ParticleScene.cpp',
            'src/scenes/particles/ParticleScene.h',
            'src/scenes/particles/shader/instancedParticles.frag',
            'src/scenes/particles/shader/instancedParticles.vert',
            'src/scenes/particles/shader/particle.comp',
            'src/scenes/scenes.h',
            'src/scenes/spiral/SpiralScene.cpp',
            'src/scenes/spiral/SpiralScene.h',
            'src/scenes/swarm/SwarmScene.cpp',
            'src/scenes/swarm/SwarmScene.h',
            'src/scenes/swarm/shader/colorSplash.comp',
            'src/scenes/swarm/shader/swarm.comp',
            'src/scenes/swarm/shader/swarm.frag',
            'src/scenes/swarm/shader/swarm.vert',
            'src/scenes/swarm/shader/swarmUserEnter.comp',
        ]

        of.addons: [
            'ofxAppUtils',
            'ofxGui',
            'ofxOsc',
            'ofxUbo',
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project
        of.staticLibraries: []  // static libraries
        of.dynamicLibraries: [] // dynamic libraries

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    property bool makeOF: true  // use makfiles to compile the OF library
                                // will compile OF only once for all your projects
                                // otherwise compiled per project with qbs
    

    property bool precompileOfMain: false  // precompile ofMain.h
                                           // faster to recompile when including ofMain.h 
                                           // but might use a lot of space per project

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
