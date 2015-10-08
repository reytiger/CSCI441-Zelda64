#include <fstream>
#include "WorldObjects/FlagBanner.hpp"

using namespace std;

// A new class is needed for the Flag's due to there animation. Flags have a
// uniqie animation that should look like they are in the wind.

/*************************  DRAW FLAG BANNER  *********************************/
void FlagBanner::draw() const {
    pushMatrixAnd([&]() {
        glTranslatef(0, 2, 5);
        drawControlPoints();
        drawPole();
        drawFlag();
        m_WindsCurve.draw();
    });
}

void FlagBanner::drawControlPoints() const {
    pushMatrixAnd([&]() {

    });
}

void FlagBanner::drawFlag() const {
    pushMatrixAnd([&]() {

    });
}

void FlagBanner::drawPole() const {
    pushMatrixAnd([&]() {

    });
}

void FlagBanner::updateAnimation(double t, double dt) const {}

/*************************  SET UP FOR CURVES  ********************************/
////////////////////////////////////////////////////////////////////////////////
//  Load our control points from file and store them in a global variable.
bool FlagBanner::loadControlPoints(string filename) {
    string str;
    int numOfPoints = 0;
    ifstream file(filename.c_str());

    info("%s", filename);
    assert(file && "The file didn't load. :(");

    // get the number of points to build
    getline(file, str);
    numOfPoints = atoi(str.c_str());


    return true;
}

void FlagBanner::init() {
    m_FlagCPointsFile = "assets/world/flagBanner.csv";
    glChk();
    m_WindsCurve.loadFile(m_FlagCPointsFile);
    glChk();
}

Point FlagBanner::eval(double u, double v) {
    // firt, get the x pos.
    // std::vector<Point> tmp;
    // for (int i = 0; i < m_curvesCPoints.size(); ++i) {
    //     BezierCurve currentCurve = m_curvesCPoints.at(i);
    //     double _u = (u - currentCurve.getXmin())
    //                 / (currentCurve.getXmax() - currentCurve.getXmin());
    //     tmp.push_back(m_curvesCPoints.at(i).evalCubicPoint(_u));
    // }

    // // now get the z position.
    // BezierCurve uCurve = BezierCurve(tmp);
    // double _v = (v - uCurve.getZmin()) / (uCurve.getZmax() -
    // uCurve.getZmin());
    // return uCurve.evalCubicPoint(_v);
    Point tmp;
    return tmp;
}
