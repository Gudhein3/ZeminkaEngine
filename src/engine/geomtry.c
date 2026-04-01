#include <zeminka/engine.h>

bool ZEGeomIsPointInBBox(ZEVec3 p, ZEGeomBBox b) {
    return (p.x >= b.center.x - b.dimensions.x && p.x <= b.center.x + b.dimensions.x &&
            p.y >= b.center.y - b.dimensions.y && p.y <= b.center.y + b.dimensions.y &&
            p.z >= b.center.z - b.dimensions.z && p.z <= b.center.z + b.dimensions.z);
}

// Thanks Aitor Lopera Toledo for https://github.com/atlotl/CubeIntersectionCalculator.
bool ZEGeomAreBBoxIntersecting(ZEGeomBBox a, ZEGeomBBox b) {
    return !(a.center.x + a.dimensions.x / 2 < b.center.x - b.dimensions.x / 2 ||
             a.center.x - a.dimensions.x / 2 > b.center.x + b.dimensions.x / 2 ||
             a.center.y + a.dimensions.y / 2 < b.center.y - b.dimensions.y / 2 ||
             a.center.y - a.dimensions.y / 2 > b.center.y + b.dimensions.y / 2 ||
             a.center.z + a.dimensions.z / 2 < b.center.z - b.dimensions.z / 2 ||
             a.center.z - a.dimensions.z / 2 > b.center.z + b.dimensions.z / 2);
}
