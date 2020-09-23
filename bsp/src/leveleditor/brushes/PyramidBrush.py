from .BaseBrush import BaseBrush

from panda3d.core import Point3

from bsp.leveleditor import LEUtils

class PyramidBrush(BaseBrush):

    Name = "Pyramid"

    def create(self, generator, mins, maxs, material, roundDecimals, temp = False):
        # The lower Z plane will be base
        center = (mins + maxs) / 2
        c1 = LEUtils.roundVector(Point3(mins.x, mins.y, mins.z), roundDecimals)
        c2 = LEUtils.roundVector(Point3(maxs.x, mins.y, mins.z), roundDecimals)
        c3 = LEUtils.roundVector(Point3(maxs.x, maxs.y, mins.z), roundDecimals)
        c4 = LEUtils.roundVector(Point3(mins.x, maxs.y, mins.z), roundDecimals)
        c5 = LEUtils.roundVector(Point3(center.x, center.y, maxs.z), roundDecimals)

        faces = [
            [ c1, c2, c3, c4 ],
            [ c2, c1, c5 ],
            [ c3, c2, c5 ],
            [ c4, c3, c5 ],
            [ c1, c4, c5 ]
        ]

        return [self.makeSolid(generator, faces, material, temp)]
