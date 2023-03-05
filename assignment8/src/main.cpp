#include <stdio.h>
#include <assert.h>
#include <GL/glut.h>
using namespace std;

#include "arg_parser.h"
#include "glCanvas.h"
#include "spline_parser.h"
#include "curve.h"

// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) {

  glutInit(&argc, argv);

  // parse the command line arguments & input file
  ArgParser *args = new ArgParser(argc,argv);
  SplineParser* splines = new SplineParser(args->input_file);
  BezierCurve::initBasis();
  BSplineCurve::initBasis();

  // launch curve editor!
  if (args->gui) {
    GLCanvas glcanvas;
    glcanvas.initialize(args,splines);
  }

  // output as required
  splines->SaveBezier(args);
  splines->SaveBSpline(args);
  splines->SaveTriangles(args);

  // cleanup
  delete args;
  delete splines;
  return 0;
}

// ====================================================================
// ====================================================================



