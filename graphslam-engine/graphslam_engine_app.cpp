/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2016, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */


#include <mrpt/obs/CRawlog.h>
#include <mrpt/graphslam.h>
#include <mrpt/graphs.h>
#include <mrpt/gui.h>
#include <mrpt/opengl.h>
#include <mrpt/utils.h>
#include <mrpt/system/filesystem.h>
#include <mrpt/system/os.h>
#include <mrpt/utils/CLoadableOptions.h>
#include <mrpt/opengl/CPlanarLaserScan.h> 
#include <mrpt/poses/CPoses2DSequence.h>
#include <mrpt/poses/CPosePDF.h>
#include <mrpt/graphs/CNetworkOfPoses.h>

#include <mrpt/otherlibs/tclap/CmdLine.h>

#include <string>
#include <cerrno>

#include "GraphSlamEngine.h"

using namespace mrpt::utils;
using namespace mrpt::poses;
using namespace mrpt::obs;
using namespace mrpt::system;
using namespace mrpt::graphs;
using namespace mrpt::math;
using namespace mrpt::opengl;
using namespace mrpt::utils;

using namespace std;

#define VERBOSE_COUT  if (verbose) std::cout << "[graphslam_engine] "

/** 
 * Forward declaration
 * display_graph function template provided in display_graph.cpp
 */
template <class GRAPHTYPE> void display_graph(const GRAPHTYPE & g);


/* 
 * Command line options initialization
 */

TCLAP::CmdLine cmd(/*output message = */ "graphslam_engine", 
    /* delimeter = */ ' ', /* version = */ MRPT_getVersion().c_str());
TCLAP::ValueArg<string> arg_ini_file(/*flag = */ "i", /*name = */ "ini_file", 
    /*desc = */ ".ini configuration file", /* required = */ false,
    /* default value = */ "", /*typeDesc = */ "config.ini", /*parser = */ cmd);
TCLAP::SwitchArg arg_do_demo(/*flag = */ "d", /*name = */ "demo",
    /*desc = */ "use default file for demosntratio purposes", 
    /* parser = */ cmd, /* default = */ false);



/**
 * main
 */
int main(int argc, char **argv)
{
  
  try {

    /** 
     * Command line arguments parsing
     */
    // validation
    if (!cmd.parse( argc, argv ) || argc == 1) {
      THROW_EXCEPTION("Neither .ini file or demo option was specified");
    }
    if (arg_do_demo.isSet() && arg_ini_file.isSet()) {
      THROW_EXCEPTION("Either the demo option or the .ini file must be specified."
          << "Exiting..")
    }
    // fetching the .ini file
    string config_fname;
    if ( arg_do_demo.isSet() ) {
      config_fname = "../default_config.ini";
      VERBOSE_COUT << "Using the demo file: " << config_fname << endl;
    }
    else {
      config_fname = arg_ini_file.getValue();
      VERBOSE_COUT << "Rawlog file: " << config_fname << endl;
    }

    /**
     * Objects initialization
     */

    // Initialize the visualization objects
    CDisplayWindow3D	win("Graphslam building procedure",800, 600);
    win.setCameraElevationDeg(75);


    // Initialize the class
    GraphSlamEngine_t<CNetworkOfPoses2DInf> g_engine(config_fname, &win);
    
    g_engine.parseRawlogFile();

    // saving the graph to external file
    g_engine.saveGraph();
    //g_engine.saveGraph("kalimera.txt");

    // TODO add "keylogger" function to know when to exit..

    while (win.isOpen()) {
      ;;
    }


  }
  catch (exception& e) {
    setConsoleColor(CONCOL_RED, true);
    cerr << "Program finished with an exception!" << endl;
    setConsoleColor(CONCOL_NORMAL, true);

    cerr << e.what() << endl;

    mrpt::system::pause();
    return -1;
  }
  catch (...) {
    setConsoleColor(CONCOL_RED, true);
    cerr << "Program finished for an untyped exception!!" << endl;
    setConsoleColor(CONCOL_NORMAL, true);

    mrpt::system::pause();
    return -1;
  }

  return 0;
}
