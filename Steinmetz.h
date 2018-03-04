#ifndef STEINMETZ_H
#define STEINMETZ_H

#include <cmath>
#include "Utility.h"
#include "Geometry.h"
#include "Shape.h"

class Steinmetz : public Shape {
public:
    Steinmetz() {};
    ~Steinmetz() {};

protected:
    PVList getPoints(int segX, int segY) {
        if ((int)pvs.size() != 8 * (segX + 1) * (segY + 1)) {
            pvs.clear();
            PVList().swap(pvs); // free memory

            double r = DEFAULT_LENGTH; // radius
            PVList xySide;

            // one side first
            for (int y = 0; y < segY + 1; y++) {
                double xStart = -0.5 + y / (2.0f * segY);
                double xStep = 2 * std::abs(xStart) / segX;
                double yVal = sqrt(pow(r, 2) - pow(xStart, 2));

                for (int x = 0; x < segX + 1; x++) {
                    Point p(
                        xStart + x * xStep,
                        yVal,
                        std::abs(xStart)
                    );
                    Vector v(p[0], p[1], p[2]);
                    v.normalize();
                    PV pv = {p, v};
                    xySide.push_back(pv);
                }
            }

            // rotate for all 8 sides
            for (int h = 0; h < 4; h++) { // horizontal rota
                Matrix hmtx = rotY_mat(DEG_TO_RAD(h * 90)); // vertical rota
                for (int v = 0; v < 2; v++) {
                    Matrix vmtx = rotX_mat(DEG_TO_RAD(v * 180));
                    PVList sidepvs = rotatePVs(hmtx, xySide);
                    sidepvs = rotatePVs(vmtx, sidepvs);
                    pvs.insert(pvs.end(), sidepvs.begin(), sidepvs.end());
                }
            }
        }
        return pvs;
    }

    Surface getSurface(int segX, int segY) {
        if ((int)surface.size() != 8 * segX * (2 * (segX - 1) + 1)) {
            surface.clear();
            Surface().swap(surface); // free memory

            for (int s = 0; s < 8; s++) { // sides
                for (int y = 0; y < segY; y++) { // hline by hline
                    for (int x = 0; x < segX; x++) {
                        int li = y;
                        int ri = y + 1;

                        if (y == segY - 1) {
                            surface.push_back(toTriangle(
                                (s + 1) * (segX + 1) * (segY + 1) - 1,
                                toSideIndex(s, li, x, segY + 1, segX + 1),
                                toSideIndex(s, li, x + 1, segY + 1, segX + 1)
                            ));
                        } else {
                            surface.push_back(toTriangle(
                                toSideIndex(s, ri, x + 1, segY + 1, segX + 1),
                                toSideIndex(s, ri, x, segY + 1, segX + 1),
                                toSideIndex(s, li, x, segY + 1, segX + 1)
                            ));
                            surface.push_back(toTriangle(
                                toSideIndex(s, ri, x + 1, segY + 1, segX + 1),
                                toSideIndex(s, li, x, segY + 1, segX + 1),
                                toSideIndex(s, li, x + 1, segY + 1, segX + 1)
                            ));
                        }
                    }
                }
            }
            // for (int x = 0; x < segX + 1; x++) { // line by line
            //     for (int y = 0; y < segY + 1; y++) { // point per line
            //         int li = x; // left line index
            //         int ri = (x + 1) % segX; // right line index

            //         surface.push_back(toTriangle(
            //             toSideIndex(ri, y, segY + 1),
            //             toSideIndex(li, y, segY + 1),
            //             toSideIndex(ri, y + 1, segY + 1)
            //         ));
            //         surface.push_back(toTriangle(
            //             toSideIndex(ri, y + 1, segY + 1),
            //             toSideIndex(li, y, segY + 1),
            //             toSideIndex(li, y + 1, segY + 1)
            //         ));
            //     }
            // }

            // for (int i = 0; i < pvs.size(); i++) {
            //     for (int j = 0; j < pvs.size(); j++) {
            //         for (int k = 0; k < pvs.size(); k++) {
            //             surface.push_back(toTriangle(i, j, k));
            //         }
            //     }
            // }
        }
        return surface;
    }

private:

};

#endif