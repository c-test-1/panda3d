#define _PYTHON_VERSION
#ifdef _PYTHON_VERSION

#include "py_bsploader.h"
#include "texturePool.h"
#include "sceneGraphReducer.h"

PyObject *Py_BSPLoader::
get_entity(int n) const {
  PyMutexHolder holder;

  PyObject *pent = _entities[n]->py_entity;
  Py_INCREF(pent);
  return pent;
}

CBaseEntity *Py_BSPLoader::
get_c_entity(const int entnum) const {
  for (size_t i = 0; i < _entities.size(); i++) {
    const EntityDef *def = _entities[i];
    if (!def->c_entity)
      continue;
    if (def->c_entity &&
        def->c_entity->get_bsp_entnum() == entnum) {
      return def->c_entity;
    }
  }

  return nullptr;
}

PyObject *Py_BSPLoader::
find_all_entities(const string &classname) {
  PyMutexHolder holder;

  PyObject *list = PyList_New(0);

  for (size_t i = 0; i < _entities.size(); i++) {
    const EntityDef *def = _entities[i];
    if (!def->c_entity)
      continue;
    std::string cls = def->c_entity->get_entity_value("classname");
    if (classname == cls) {
      PyList_Append(list, def->py_entity);
    }
  }

  Py_INCREF(list);

  return list;
}

void Py_BSPLoader::
get_entity_keyvalues(PyObject *list, const int entnum) {
  PyMutexHolder holder;

  entity_t *ent = get_level()->get_bspdata()->entities + entnum;
  for (epair_t *ep = ent->epairs; ep->next != nullptr; ep = ep->next) {
    PyObject *kv = PyTuple_New(2);
    PyTuple_SetItem(kv, 0, PyUnicode_FromString(ep->key));
    PyTuple_SetItem(kv, 1, PyUnicode_FromString(ep->value));
    PyList_Append(list, kv);
    Py_DECREF(kv);
  }
}

void Py_BSPLoader::
link_cent_to_pyent(int entnum, PyObject *pyent) {
  EntityDef *pdef = nullptr;
  bool found_pdef = false;

  entity_t *ent = get_level()->get_bspdata()->entities + entnum;
  std::string targetname = ValueForKey(ent, "targetname");

  pvector<EntityDef *> children;
  children.reserve(32);

  for (size_t i = 0; i < _entities.size(); i++) {
    EntityDef *def = _entities[i];
    if (!def->c_entity)
      continue;

    if (!found_pdef && def->c_entity->get_bsp_entnum() == entnum) {
      pdef = def;
      found_pdef = true;
    }

    if (def->c_entity->get_bsp_entnum() != entnum &&
        targetname.length() > 0u &&
        def->py_entity != nullptr) {
      std::string parentname = def->c_entity->get_entity_value("parent");
      if (!parentname.compare(targetname)) {
        // This entity is parented to the specified entity.
        children.push_back(def);
      }
    }
  }

  nassertv(pdef != nullptr);

  PyMutexHolder holder;

  Py_INCREF(pyent);
  pdef->py_entity = pyent;

  for (size_t i = 0; i < children.size(); i++) {
    EntityDef *def = children[i];
    PyObject_CallMethod(def->py_entity, (char *)"parentGenerated", NULL);
  }
}

PyObject *Py_BSPLoader::
get_py_entity_by_target_name(const string &targetname) const {
  for (size_t i = 0; i < _entities.size(); i++) {
    const EntityDef *def = _entities[i];
    if (!def->c_entity)
      continue;
    PyObject *pyent = def->py_entity;
    if (!pyent)
      continue;
    string tname = def->c_entity->get_entity_value("targetname");
    if (tname == targetname) {
      PyMutexHolder holder;
      Py_INCREF(pyent);
      return pyent;
    }
  }

  Py_RETURN_NONE;
}

/**
 * Manually remove a Python entity from the list.
 * Note: `unload()` will no longer be called on this entity when the level unloads.
 */
void Py_BSPLoader::
remove_py_entity(PyObject *obj) {
  for (size_t i = 0; i < _entities.size(); i++) {
    EntityDef *ent = _entities[i];
    if (ent->py_entity == obj) {
      PyMutexHolder holder;
      Py_DECREF(ent->py_entity);
      ent->py_entity = nullptr;
    }
  }
}

bool Py_BSPLoader::
read(const Filename &filename, bool is_transition) {
  if (!BSPLoader::read(filename, is_transition)) {
    return false;
  }

  spawn_entities();

  return true;
}

void Py_BSPLoader::
spawn_entities() {



  // Now load all of the entities at the application level.
  for (size_t i = 0; i < _entities.size(); i++) {
    EntityDef *ent = _entities[i];
    if (ent->py_entity && !ent->dynamic && !ent->preserved) {
      // This is a newly loaded (not preserved from previous level) entity
      // that is from the BSP file.
      PyMutexHolder holder;

      //PyTypeObject *tp = Py_TYPE(ent->py_entity);
      //std::cout << ent->c_entity->get_classname() << " : " << ent->py_entity << " : " << _PyType_Lookup(tp, PyUnicode_FromString("load")) << std::endl;

      PyObject *meth = PyObject_GetAttrString(ent->py_entity, (char *)"load");
      if (meth) {
        PyObject_CallObject(meth, NULL);
      } else {
        PyErr_PrintEx(1);
      }
      //PyObject_CallMethod(ent->py_entity, (char *)"load", NULL);
    }
  }
}

//============================================================================================

class Py_CL_BSPLevel : public BSPLevel {
public:
  Py_CL_BSPLevel(BSPLoader *loader);

protected:
  virtual void load_geometry();
};

Py_CL_BSPLevel::
Py_CL_BSPLevel(BSPLoader *loader) :
  BSPLevel(loader) {
}

void Py_CL_BSPLevel::
load_geometry() {
  load_cubemaps();

  LightmapPalettizer lmp(this);
  _lightmap_dir = lmp.palettize_lightmaps();

  make_faces();
  SceneGraphReducer gr;
  gr.apply_attribs(_result.node());

  _result.set_attrib(BSPFaceAttrib::make_default(), 1);

  _decal_mgr.init();
}

PT(BSPLevel) Py_CL_BSPLoader::
make_level() {
  return new Py_CL_BSPLevel(this);
}

PyObject *Py_CL_BSPLoader::
make_pyent(PyObject *py_ent, const string &classname) {
  if (_entity_to_class.find(classname) != _entity_to_class.end()) {

    PyMutexHolder holder;

    Py_INCREF(py_ent);

    // A python class was linked to this entity!
    PyObject *obj = PyObject_CallObject(_entity_to_class[classname], NULL);
    if (obj == nullptr)
      PyErr_PrintEx(1);
    Py_INCREF(obj);
    // Give the python entity a handle to the c entity.
    PyObject_SetAttrString(obj, (char *)"cEntity", py_ent);
    // Precache all resources the entity will use.
    PyObject_CallMethod(obj, (char *)"precache", NULL);

    // Don't call load just yet, we need to have all of the entities created first, because some
    // entities rely on others.
    PyTypeObject *tp = Py_TYPE(obj);
    std::cout << classname << " : " << obj << " : " << obj->ob_refcnt << " : " << tp->tp_dict << std::endl;
    return obj;
  }
  return nullptr;
}

void Py_CL_BSPLoader::
link_entity_to_class(const string &entname, PyObject *type) {
  Py_INCREF(type);
  _entity_to_class[entname] = type;
}

void Py_CL_BSPLoader::
cleanup_entities(bool is_transition) {
  // Unload any client-side only entities.
  // Assume the server will take care of unloading networked entities.
  //
  // UNDONE: Client-side only entities are an obsolete feature.
  //         All entities should eventually be networked and client-side
  //         entity functionality should be removed.

  PyMutexHolder holder;

  for (auto itr = _entities.begin(); itr != _entities.end(); itr++) {
    EntityDef *def = *itr;
    if (def->py_entity) {
      if (_entity_to_class.find(def->c_entity->get_classname()) != _entity_to_class.end()) {
        // This is a client-sided entity. Unload it.
        PyObject_CallMethod(def->py_entity, (char *)"unload", NULL);
      }

      Py_DECREF(def->py_entity);
      def->py_entity = nullptr;
    }
  }

  _entities.clear();
}

void Py_CL_BSPLoader::
load_entities() {
  BSPLevel *level = get_level();
  bspdata_t *bspdata = level->get_bspdata();

  for (int entnum = 0; entnum < bspdata->numentities; entnum++) {
    entity_t *ent = &bspdata->entities[entnum];

    string classname = ValueForKey(ent, "classname");
    const char *psz_classname = classname.c_str();
    string id = ValueForKey(ent, "id");

    vec_t origin[3];
    GetVectorDForKey(ent, "origin", origin);

    vec_t angles[3];
    GetVectorDForKey(ent, "angles", angles);

    string targetname = ValueForKey(ent, "targetname");

    if (!strncmp(classname.c_str(), "trigger_", 8) ||
        !strncmp(classname.c_str(), "func_water", 10)) {
      // This is a bounds entity. We do not actually care about the geometry,
      // but the mins and maxs of the model. We will use that to create
      // a BoundingBox to check if the avatar is inside of it.
      int modelnum = BSPLevel::extract_modelnum_s(ent);
      if (modelnum != -1) {
        level->remove_model(modelnum);
        brush_model_data_t &model_data = level->get_brush_model_data(modelnum);
        model_data.model_root = level->get_model(0);
        model_data.merged_modelnum = 0;
        NodePath(model_data.decal_rbc).remove_node();
        model_data.decal_rbc = nullptr;

        dmodel_t *mdl = &bspdata->dmodels[modelnum];

        PT(CBoundsEntity) entity = new CBoundsEntity;
        entity->set_data(entnum, ent, level, mdl);

        PyObject *py_ent = DTool_CreatePyInstance<CBoundsEntity>(entity, false);
        PyObject *linked = make_pyent(py_ent, classname);

        _entities.push_back(new EntityDef(entity, linked));
      }
    } else if (!strncmp(classname.c_str(), "func_", 5)) {
      // Brush entites begin with func_, handle those accordingly.
      int modelnum = BSPLevel::extract_modelnum_s(ent);
      if (modelnum != -1) {
        // Brush model
        NodePath modelroot = level->get_model(modelnum);
        // render all faces of brush model in a single batch
        BSPLevel::clear_model_nodes_below(modelroot);
        modelroot.flatten_strong();

        if (!strncmp(classname.c_str(), "func_wall", 9) ||
            !strncmp(classname.c_str(), "func_detail", 11) ||
            !strncmp(classname.c_str(), "func_illusionary", 17)) {
          // func_walls and func_details aren't really entities,
          // they are just hints to the compiler. we can treat
          // them as regular brushes part of worldspawn.
          modelroot.wrt_reparent_to(level->get_model(0));
          BSPLevel::flatten_node(modelroot);
          NodePathCollection npc = modelroot.get_children();
          for (int n = 0; n < npc.get_num_paths(); n++) {
            npc[n].wrt_reparent_to(level->get_model(0));
          }
          level->remove_model(modelnum);
          brush_model_data_t &world_model = level->get_brush_model_data(0);
          brush_model_data_t &model_data = level->get_brush_model_data(modelnum);
          model_data.model_root = world_model.model_root;
          NodePath(model_data.decal_rbc).remove_node();
          model_data.decal_rbc = world_model.decal_rbc;
          model_data.merged_modelnum = 0;
          continue;
        }

        PT(CBrushEntity) entity = new CBrushEntity;
        entity->set_data(entnum, ent, level, &bspdata->dmodels[modelnum], modelroot);

        PyObject *py_ent = DTool_CreatePyInstance<CBrushEntity>(entity, false);
        PyObject *linked = make_pyent(py_ent, classname);

        _entities.push_back(new EntityDef(entity, linked));
      }
    } else if (!strncmp(classname.c_str(), "infodecal", 9)) {
      const char *mat = ValueForKey(ent, "texture");
      const BSPMaterial *bspmat = BSPMaterial::get_from_file(mat);
      Texture *tex = TexturePool::load_texture(bspmat->get_keyvalue("$basetexture"));
      LPoint3 vpos(origin[0], origin[1], origin[2]);
      level->trace_decal(mat, LPoint2(tex->get_orig_file_x_size() / 16.0, tex->get_orig_file_y_size() / 16.0),
                             0.0, vpos, vpos, LColorf(1.0), DECALFLAGS_STATIC);
    } else {
      if (classname == "light_environment")
        level->set_light_environment(ent);

      // We don't know what this entity is exactly, maybe they linked it to a python class.
      // It didn't start with func_, so we can assume it's just a point entity.
      PT(CPointEntity) entity = new CPointEntity;
      entity->set_data(entnum, ent, level);

      PyObject *py_ent = DTool_CreatePyInstance<CPointEntity>(entity, false);
      PyObject *linked = make_pyent(py_ent, classname);

      _entities.push_back(new EntityDef(entity, linked));
    }
  }
}

//============================================================================================

class Py_AI_BSPLevel : public BSPLevel {
public:
  Py_AI_BSPLevel(BSPLoader *loader);

protected:
  virtual void load_geometry();
};

Py_AI_BSPLevel::
Py_AI_BSPLevel(BSPLoader *loader) :
  BSPLevel(loader) {
}

void Py_AI_BSPLevel::
load_geometry() {
  make_faces_ai();
}

PT(BSPLevel) Py_AI_BSPLoader::
make_level() {
  return new Py_AI_BSPLevel(this);
}

void Py_AI_BSPLoader::
add_dynamic_entity(PyObject *pyent) {
  PyMutexHolder holder;

  Py_INCREF(pyent);

  _entities.push_back(new EntityDef(nullptr, pyent, true));
}

void Py_AI_BSPLoader::
remove_dynamic_entity(PyObject *pyent) {
  auto itr = _entities.end();
  for (size_t i = 0; i < _entities.size(); i++) {
    const EntityDef *def = _entities[i];
    if (def->py_entity == pyent) {
      itr = _entities.begin() + i;
      break;
    }
  }

  nassertv(itr != _entities.end());

  PyMutexHolder holder;
  Py_DECREF(pyent);
  _entities.erase(itr);
}

void Py_AI_BSPLoader::
mark_entity_preserved(int n, bool preserved) {
  _entities[n]->preserved = preserved;
}

void Py_AI_BSPLoader::
set_server_entity_dispatcher(PyObject *dispatch) {
  _sv_ent_dispatch = dispatch;
}

void Py_AI_BSPLoader::
link_server_entity_to_class(const string &name, PyObject *type) {
  Py_INCREF(type);
  _svent_to_class[name] = type;
}

void Py_AI_BSPLoader::
load_entities() {

  BSPLevel *level = get_level();
  bspdata_t *bspdata = level->get_bspdata();

  PyMutexHolder holder;

  for (int entnum = 0; entnum < bspdata->numentities; entnum++) {
    entity_t *ent = &bspdata->entities[entnum];

    string classname = ValueForKey(ent, "classname");
    const char *psz_classname = classname.c_str();
    string id = ValueForKey(ent, "id");

    vec_t origin[3];
    GetVectorDForKey(ent, "origin", origin);

    vec_t angles[3];
    GetVectorDForKey(ent, "angles", angles);

    string targetname = ValueForKey(ent, "targetname");

    if (_svent_to_class.find(classname) != _svent_to_class.end()) {
      if (_sv_ent_dispatch != nullptr) {
        PyObject *ret = PyObject_CallMethod(_sv_ent_dispatch, (char *)"createServerEntity",
                                            (char *)"Oi", _svent_to_class[classname], entnum);
        if (!ret) {
          PyErr_PrintEx(1);
        } else {

          PT(CBaseEntity) entity;
          if (!strncmp(psz_classname, "func_", 5) ||
              entnum == 0) {
            int modelnum;
            if (entnum == 0)
              modelnum = 0;
            else
              modelnum = level->extract_modelnum(entnum);
            entity = new CBrushEntity;
            DCAST(CBrushEntity, entity)->set_data(entnum, ent, level,
                                                  bspdata->dmodels + modelnum, level->get_model(modelnum));
          } else if (!strncmp(classname.c_str(), "trigger_", 8)) {
            int modelnum = BSPLevel::extract_modelnum_s(ent);
            entity = new CBoundsEntity;
            DCAST(CBoundsEntity, entity)->set_data(entnum, ent, level,
                                                   &bspdata->dmodels[modelnum]);
          } else {
            entity = new CPointEntity;
            DCAST(CPointEntity, entity)->set_data(entnum, ent, level);
          }

          Py_INCREF(ret);
          _entities.push_back(new EntityDef(entity, ret));
        }
      }
    }

  }
}

bool Py_AI_BSPLoader::
read(const Filename &filename, bool is_transition) {
  if (!Py_BSPLoader::read(filename, is_transition)) {
    return false;
  }

  if (is_transition) {
    // Find the destination landmark
    EntityDef *dest_landmark = nullptr;
    for (size_t i = 0; i < _entities.size(); i++) {
      EntityDef *def = _entities[i];
      if (!def->c_entity)
        continue;
      if (def->c_entity->get_classname() == "info_landmark" &&
          def->c_entity->get_targetname() == _transition_source_landmark.get_name()) {
        dest_landmark = def;
        break;
      }
    }

    if (dest_landmark) {
#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure();
#endif

      CPointEntity *dest_ent = DCAST(CPointEntity, dest_landmark->c_entity);
      NodePath dest_landmark_np("destination_landmark");
      dest_landmark_np.set_pos(dest_ent->get_origin());
      dest_landmark_np.set_hpr(dest_ent->get_angles());
      PyObject *py_dest_landmark_np =
        DTool_CreatePyInstance(&dest_landmark_np, *(Dtool_PyTypedObject *)dest_landmark_np.get_class_type().get_python_type(), true, true);
      Py_INCREF(py_dest_landmark_np);

      for (size_t i = 0; i < _entities.size(); i++) {
        EntityDef *def = _entities[i];
        if (def->preserved && def->py_entity) {
          LMatrix4f mat = def->landmark_relative_transform;
          PyObject *py_mat = DTool_CreatePyInstance(&mat, *(Dtool_PyTypedObject *)mat.get_class_type().get_python_type(), true, true);
          Py_INCREF(py_mat);

          PyObject *meth = PyObject_GetAttrString(def->py_entity, (char*)"transitionXform");
          if (meth) {
            PyObject *args = PyTuple_Pack(2, py_dest_landmark_np, py_mat);
            Py_INCREF(args);
            PyObject_CallObject(meth, args);
            Py_DECREF(args);

          } else {
            PyErr_PrintEx(1);
          }

          Py_DECREF(py_mat);
        }
      }

      Py_DECREF(py_dest_landmark_np);
      dest_landmark_np.remove_node();

#if defined(HAVE_THREADS) && !defined(SIMPLE_THREADS)
      PyGILState_Release(gstate);
#endif
    }

    clear_transition_landmark();
  }

  return true;
}

void Py_AI_BSPLoader::
cleanup_entities(bool is_transition) {

  PyMutexHolder holder;

  // If we are in a transition to another level, unload any entities
  // that aren't being perserved. Or if we are not in a transition,
  // unload all entities.
  for (auto itr = _entities.begin(); itr != _entities.end(); ) {
    EntityDef *def = *itr;
    if ((is_transition && !def->preserved) || !is_transition) {
      PyObject *py_ent = def->py_entity;
      if (py_ent) {
        PyObject_CallMethod(py_ent, (char *)"unload", NULL);
        Py_DECREF(py_ent);
        def->py_entity = nullptr;
      }
      itr = _entities.erase(itr);
      continue;
    } else if (def->c_entity) {
      // This entity is being preserved.
      // The entnum is now invalid since the BSP file is changing.
      // This avoids conflicts with future entities from the new BSP file.
      def->c_entity->_bsp_entnum = -1;
    }
    itr++;
  }

  if (!is_transition) {
    _entities.clear();

  } else if (!_transition_source_landmark.is_empty()) {
    // We are in a transition to another level.
    // Store all entity transforms relative to the source landmark.
    for (size_t i = 0; i < _entities.size(); i++) {
      EntityDef *ent = _entities[i];
      if (ent->py_entity && DtoolInstance_Check(ent->py_entity)) {
        NodePath *pyent_np = (NodePath *)
          DtoolInstance_VOID_PTR(ent->py_entity);
        if (pyent_np) {
          ent->landmark_relative_transform =
            pyent_np->get_mat(_transition_source_landmark);
        }
      }
    }
  }
}

#endif // #ifdef _PYTHON_VERSION
