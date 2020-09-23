from panda3d.core import GeomNode

from .SelectionMode import SelectionMode
from .SelectionType import SelectionType
from bsp.leveleditor.objectproperties.ObjectPropertiesWindow import ObjectPropertiesWindow
from bsp.leveleditor import LEGlobals
from bsp.leveleditor.menu.KeyBind import KeyBind
from bsp.leveleditor.mapobject.Entity import Entity
from bsp.leveleditor.mapobject.Solid import Solid
from bsp.leveleditor.actions.Reparent import Reparent
from bsp.leveleditor.actions.Delete import Delete
from bsp.leveleditor.actions.ActionGroup import ActionGroup
from bsp.leveleditor.actions.Create import Create
from bsp.leveleditor.actions.Select import Select

from PyQt5 import QtWidgets, QtCore, QtGui

_ObjectPropertiesWindow = None

# Object selection mode: selects individual MapObjects (Entity, Solid, etc), ignoring groups
class ObjectMode(SelectionMode):

    Type = SelectionType.Objects
    Key = "mapobject"
    Mask = LEGlobals.ObjectMask
    KeyBind = KeyBind.SelectObjects
    Icon = "resources/icons/editor-select-objects.png"
    Name = "Objects"
    Desc = "Select individual objects"
    ToolOnly = False

    def __init__(self, mgr):
        SelectionMode.__init__(self, mgr)
        self.objectProperties = ObjectMode.getObjectPropertiesWindow()

    def updateModeActions(self):
        SelectionMode.updateModeActions(self)
        if len(self.mgr.selectedObjects) > 0:
            base.menuMgr.enableAction(KeyBind.TieToWorld)
            base.menuMgr.enableAction(KeyBind.TieToEntity)
        else:
            base.menuMgr.disableAction(KeyBind.TieToWorld)
            base.menuMgr.disableAction(KeyBind.TieToEntity)

    def getObjectPropertiesTarget(self):
        return self.mgr.selectedObjects[len(self.mgr.selectedObjects) - 1]

    def updateObjProperties(self):
        self.objectProperties.setMgr(self.mgr)
        self.objectProperties.enable()

    def activate(self):
        base.menuMgr.connect(KeyBind.TieToWorld, self.__tieToWorld)
        base.menuMgr.connect(KeyBind.TieToEntity, self.__tieToEntity)

        SelectionMode.activate(self)
        self.updateObjProperties()

    def deactivate(self):
        base.menuMgr.disconnect(KeyBind.TieToWorld, self.__tieToWorld)
        base.menuMgr.disconnect(KeyBind.TieToEntity, self.__tieToEntity)

        self.objectProperties.disable()
        SelectionMode.deactivate(self)

    def cleanup(self):
        self.objectProperties = None
        SelectionMode.cleanup(self)

    def onSelectionsChanged(self):
        SelectionMode.onSelectionsChanged(self)
        self.objectProperties.updateForSelection()

    def __tieToWorld(self):
        ents = [e for e in self.mgr.selectedObjects if isinstance(e, Entity)]

        children = []
        for e in ents:
            children += list(e.children.values())

        actions = [Reparent(children, base.document.world),
                   Delete(ents)]
        base.actionMgr.performAction("Move to world", ActionGroup(actions))

    def __tieToEntity(self):
        ents = [e for e in self.mgr.selectedObjects if isinstance(e, Entity)]

        existing = None

        if (len(ents) == 1):
            dlg = QtWidgets.QMessageBox(parent = base.qtWindow, icon = QtWidgets.QMessageBox.NoIcon)
            dlg.setModal(True)
            dlg.setWindowTitle("Existing Entity in Selection")
            dlg.setText(("You have selected an existing entity (a '%s'), how would you like to proceed?\n"
                         "- Keep the existing entity and add the selected items to the entity\n"
                         "- Create a new entity and add the selected items to the new entity") % ents[0].getClassName())
            dlg.setStandardButtons(QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No | QtWidgets.QMessageBox.Cancel)
            yes = dlg.button(QtWidgets.QMessageBox.Yes)
            yes.setText("Keep Existing")
            no = dlg.button(QtWidgets.QMessageBox.No)
            no.setText("Create New")
            ret = dlg.exec_()

            if ret == QtWidgets.QMessageBox.Yes:
                existing = ents[0]

        elif (len(ents) > 1):
            dlg = QtWidgets.QDialog(base.qtWindow)
            dlg.setModal(True)
            dlg.setWindowTitle("Multiple Entities Selected")
            dlg.setLayout(QtWidgets.QVBoxLayout())
            dlg.layout().addWidget(QtWidgets.QLabel("You have selected multiple entities, which one would you like to keep?"))
            combo = QtWidgets.QComboBox()
            for ent in ents:
                combo.addItem(ent.getName())
            dlg.layout().addWidget(combo)
            btns = QtWidgets.QDialogButtonBox(QtWidgets.QDialogButtonBox.Ok | QtWidgets.QDialogButtonBox.Cancel, QtCore.Qt.Horizontal)
            dlg.layout().addWidget(btns)
            ret = dlg.exec_()
            if ret == QtWidgets.QDialogButtonBox.Ok:
                existing = ents[combo.currentIndex()]

        actions = []

        if not existing:
            # Tie to new entity
            existing = Entity(base.document.getNextID())
            existing.setClassname("func_detail") # TODO: default brush entity
            actions.append(Create(base.document.world.id, existing))
        else:
            # Move the new parent to the root, in case it is a descendant of a selected parent
            actions.append(Reparent(existing, base.document.world))
            # TODO: get rid of all the other entities

        objs = [o for o in self.mgr.selectedObjects if isinstance(o, Solid)]
        actions.append(Reparent(objs, existing))
        actions.append(Select([existing], False))

        base.actionMgr.performAction("Tie to entity", ActionGroup(actions))

    @staticmethod
    def getObjectPropertiesWindow():
        global _ObjectPropertiesWindow
        if not _ObjectPropertiesWindow:
            _ObjectPropertiesWindow = ObjectPropertiesWindow()
        return _ObjectPropertiesWindow
