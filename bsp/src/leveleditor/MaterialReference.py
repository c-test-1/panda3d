from panda3d.bsp import BSPMaterial
from panda3d.core import LVector2i, PNMImage, VirtualFileSystem, getModelPath, Filename

from PyQt5 import QtGui, QtCore

# Reference to a material loaded from disk that can be applied to brush faces.
# Materials with the same filename are unified to the same MaterialReference object.
# Stores the $basetexture Texture and the dimensions of it.
class MaterialReference:

    def __init__(self, filename):
        vfs = VirtualFileSystem.getGlobalPtr()
        self.material = BSPMaterial.getFromFile(filename)
        self.filename = filename
        if self.material.hasKeyvalue("$basetexture"):
            baseTexturePath = Filename(self.material.getKeyvalue("$basetexture"))

            if vfs.resolveFilename(baseTexturePath, getModelPath().getValue()):
                imageData = bytes(VirtualFileSystem.getGlobalPtr().readFile(baseTexturePath, True))
                byteArray = QtCore.QByteArray.fromRawData(imageData)
                image = QtGui.QImage.fromData(byteArray)
                self.pixmap = QtGui.QPixmap.fromImage(image)
                self.icon = QtGui.QIcon(self.pixmap)
                self.size = LVector2i(image.width(), image.height())
            else:
                self.texture = None
                self.size = LVector2i(64, 64)
                self.icon = None
                self.pixmap = None
        else:
            self.texture = None
            self.size = LVector2i(64, 64)
            self.icon = None
            self.pixmap = None
