#include "scene.h"
#include "json.h"
#include "util.h"

#include <boost\property_tree\json_parser.hpp>
#include <cstdlib>
#include <sstream>

// TODO: I made a mess of this, clean it up.

json_object_s *getChildByName(json_object_element_s *root, std::string name) {
  struct json_object_element_s *a = root;

  do {
    struct json_string_s *a_name = a->name;
    if (a_name->string == name) {
      return (struct json_object_s *)a->value->payload;
    }
    a = a->next;
  } while (a != nullptr);
  return nullptr;
}

json_object_element_s *getElementByName(json_object_element_s *root,
                                        std::string name) {
  struct json_object_element_s *a = root;

  do {
    struct json_string_s *a_name = a->name;
    if (a_name->string == name) {
      return a;
    }
    a = a->next;
  } while (a != nullptr);
  return nullptr;
}

json_array_s *getArrayByName(json_object_element_s *root, std::string name) {
  struct json_object_element_s *a = root;

  do {
    struct json_string_s *a_name = a->name;
    if (a_name->string == name) {
      return (json_array_s *)a->value->payload;
    }
    a = a->next;
  } while (a != nullptr);
  return nullptr;
}

json_value_s *getValueByName(json_object_element_s *root, std::string name) {
  struct json_object_element_s *a = root;

  do {
    struct json_string_s *a_name = a->name;
    if (a_name->string == name) {
      return a->value;
    }
    a = a->next;
  } while (a != nullptr);
  return nullptr;
}

// TODO: Float might actually not preserve decimal points, verify this.
template <typename T>
T getNumberByName(json_object_element_s *root, std::string name) {
  struct json_object_element_s *a = root;

  do {
    struct json_string_s *a_name = a->name;
    if (a_name->string == name) {
      if (std::is_same<T, int>::value) {
        return atoi(json_value_as_number(a->value)->number);
      } else if (std::is_same<T, float>::value) {
        return atof(json_value_as_number(a->value)->number);
      } else {
        return 0;
      }
    }
    a = a->next;
  } while (a != nullptr);
  return 0;
}

// Takes a branch of a property tree and reads in a vec3
glm::vec3 readVec3(const boost::property_tree::ptree &pt) {
  glm::vec3 v;
  boost::property_tree::ptree::const_iterator iter = pt.begin();
  v.x = (iter++)->second.get_value<float>();
  v.y = (iter++)->second.get_value<float>();
  v.z = (iter++)->second.get_value<float>();
  return v;
}

// Takes a branch of a property tree and reads in a vec4
glm::vec4 readVec4(const boost::property_tree::ptree &pt) {
  glm::vec4 v;
  boost::property_tree::ptree::const_iterator iter = pt.begin();
  v.x = (iter++)->second.get_value<float>();
  v.y = (iter++)->second.get_value<float>();
  v.z = (iter++)->second.get_value<float>();
  v.w = (iter++)->second.get_value<float>();
  return v;
}

// Converts a json_array_s to a vec4
glm::vec4 readVec4(json_array_s *array) {

  struct json_array_element_s *element_x = array->start;
  struct json_array_element_s *element_y = element_x->next;
  struct json_array_element_s *element_z = element_y->next;
  struct json_array_element_s *element_w = element_z->next;

  glm::vec4 v;
  v.x = atof(json_value_as_number(element_x->value)->number);
  v.y = atof(json_value_as_number(element_y->value)->number);
  v.z = atof(json_value_as_number(element_z->value)->number);
  v.w = atof(json_value_as_number(element_w->value)->number);
  return v;
}

// Reads in geometry from the property tree
void readGeometry(scene_data *scene, json_object_s *node) {
  json_object_element_s *a_node = node->start;
  // For Each branch of the tree
  do {
    struct json_string_s *a_name = a_node->name;
    std::string name = a_name->string;
    struct json_object_s *object =
        (struct json_object_s *)a_node->value->payload;
    json_object_element_s *shape_node =
        getElementByName(object->start, "shape");
    if (shape_node != nullptr) {
      std::string shape =
          ((json_string_s *)getValueByName(object->start, "shape")->payload)
              ->string;
      if (std::is_same<int, int>::value) {
      }
      geometry *geom;

      // TODO: Convert types to use integral consts rather than strings
      // Depending on the shape of the geometry, use the relevant geometry
      // creation method
      if (shape == "cube")
        geom = createBox();
      else if (shape == "tetrahedron")
        geom = createTetrahedron();
      else if (shape == "pyramid")
        geom = createPyramid();
      else if (shape == "disk") {
        int slices = getNumberByName<int>(object->start, "slices");
        geom = createDisk(slices);
      } else if (shape == "cylinder") {
        int slices = getNumberByName<int>(object->start, "slices");
        int stacks = getNumberByName<int>(object->start, "stacks");
        geom = createCylinder(stacks, slices);
      } else if (shape == "sphere") {
        int slices = getNumberByName<int>(object->start, "slices");
        int stacks = getNumberByName<int>(object->start, "stacks");
        geom = createSphere(stacks, slices);
      } else if (shape == "torus") {
        int radius = getNumberByName<float>(object->start, "radius");
        int slices = getNumberByName<int>(object->start, "slices");
        int stacks = getNumberByName<int>(object->start, "stacks");
        std::cout << "radius : " << radius << std::endl;
        geom = createTorus(radius, stacks, slices);
      } else if (shape == "plane") {
        int width = getNumberByName<int>(object->start, "width");
        int depth = getNumberByName<int>(object->start, "depth");
        geom = createPlane(width, depth);
      } else if (shape == "sierpinski") {
        int divisions = getNumberByName<int>(object->start, "divisions");
        geom = createSierpinski(divisions);
      } else if (shape == "terrain") {
        std::string heightmap =
            ((json_string_s *)getValueByName(object->start, "heightmap")
                 ->payload)
                ->string;
        geom = createTerrain(scene->textures[heightmap]);
      } else {
        std::cerr << "Error - Geometry type not recognised: " // << name
                  << std::endl;
        exit(EXIT_FAILURE);
      }
      scene->geometry[name] = geom;
    }

    a_node = a_node->next;
  } while (a_node != nullptr);
}

// Reads in textures from the scene file
void readTextures(scene_data *scene, json_object_s *node) {

  json_object_element_s *a_node = node->start;
  // For Each branch of the tree
  do {
    struct json_string_s *a_name = a_node->name;
    std::string name = a_name->string;
    struct json_string_s *file_name =
        (struct json_string_s *)a_node->value->payload;
    std::string filename = file_name->string;

    // Create the texture from the filename
    texture *tex = new texture("assets/images/" + filename);
    if (!tex->create())
      printf("Error loading texture: %s\n", filename.c_str());
    else
      scene->textures[name] = tex;

    a_node = a_node->next;
  } while (a_node != nullptr);
}

// Read in material data from the scene file
void readMaterials(scene_data *scene, const boost::property_tree::ptree &pt,
                   json_object_s *node) {
  json_object_element_s *a_node = node->start;
  do {
    struct json_string_s *a_name = a_node->name;
    std::string name = a_name->string;
    // std::cout << "name : " << name << std::endl;
    struct json_object_s *object =
        (struct json_object_s *)a_node->value->payload;
    json_array_s *emissive = getArrayByName(object->start, "emmisive");
    json_array_s *ambient = getArrayByName(object->start, "ambient");
    json_array_s *diffuse = getArrayByName(object->start, "diffuse");
    json_array_s *specular = getArrayByName(object->start, "specular");

    material *mat = new material();

    mat->data.emissive = readVec4(emissive);
    mat->data.ambient = readVec4(ambient);
    mat->data.diffuse = readVec4(diffuse);
    mat->data.specular = readVec4(specular);

    mat->data.shininess = getNumberByName<float>(object->start, "shininess");

    std::string texture =
        ((json_string_s *)getValueByName(object->start, "texture")->payload)
            ->string;

    // Try and find texture, and set material accordingly.  If the not found,
    // set to nullptr
    if (scene->textures.find(texture) != scene->textures.end()) {
      mat->texture = scene->textures[texture];
    } else {
      mat->texture = nullptr;
    }

    // Create the material, and add to the table
    mat->create();
    scene->material[name] = mat;

    a_node = a_node->next;
  } while (a_node != nullptr);

  //   boost::property_tree::ptree::const_iterator iter = pt.begin();
  //   // Iterate through all the sub branches, and read in the relevant data
  //   for (; iter != pt.end(); ++iter) {
  //     material *mat = new material();
  //     std::string name = iter->first;
  //     mat->data.emissive = readVec4(iter->second.get_child("emmisive"));
  //     mat->data.ambient = readVec4(iter->second.get_child("ambient"));
  //     mat->data.diffuse = readVec4(iter->second.get_child("diffuse"));
  //     mat->data.specular = readVec4(iter->second.get_child("specular"));
  //     mat->data.shininess =
  //         iter->second.get_child("shininess").get_value<float>();
  //     std::string texture =
  //         iter->second.get_child("texture").get_value<std::string>();

  //     // Try and find texture, and set material accordingly.  If the not
  //     found,
  //     // set to nullptr
  //     if (scene->textures.find(texture) != scene->textures.end())
  //       mat->texture = scene->textures[texture];
  //     else
  //       mat->texture = nullptr;

  //     // Create the material, and add to the table
  //     mat->create();
  //     scene->material[name] = mat;
  //   }
}

// Reads in a transform from the scene file
void readTransform(render_object *object,
                   const boost::property_tree::ptree &pt) {
  object->transform.position = readVec3(pt.get_child("position"));
  glm::vec3 rot = readVec3(pt.get_child("rotation"));
  object->transform.rotate(rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
  object->transform.rotate(rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
  object->transform.rotate(rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
  object->transform.scale = readVec3(pt.get_child("scale"));
}

// Reads in all the render objects from the scene file
void readObjects(scene_data *scene, const boost::property_tree::ptree &pt) {
  boost::property_tree::ptree::const_iterator iter = pt.begin();

  // Iterate through the sub branches of the property file, reading in each
  // object
  for (; iter != pt.end(); ++iter) {
    // Create a render object
    render_object *object = new render_object();

    // Read in the name for the object
    std::string name = iter->first;

    // Read in the geometry
    std::string geom =
        iter->second.get_child("geometry").get_value<std::string>();
    object->geometry = scene->geometry[geom];

    // Read in the material
    std::string mat =
        iter->second.get_child("material").get_value<std::string>();
    object->material = scene->material[mat];

    // Read in the transform
    readTransform(object, iter->second.get_child("transform"));

    // Add the render object to the table of render objects
    scene->objects[name] = object;
  }
}

// Reads in lighting data for a scene
void readLighting(scene_data *scene, const boost::property_tree::ptree &pt) {
  scene->light.data.ambient = readVec4(pt.get_child("ambient"));
  scene->light.data.diffuse = readVec4(pt.get_child("diffuse"));
  scene->light.data.specular = readVec4(pt.get_child("specular"));
  scene->light.data.lightDir = readVec4(pt.get_child("lightDir"));
  scene->light.create();
}

// Reads in data for a point light
void readPointLight(scene_data *scene, const boost::property_tree::ptree &pt) {
  point_light_data point;
  point.ambient = readVec4(pt.get_child("ambient"));
  point.diffuse = readVec4(pt.get_child("diffuse"));
  point.specular = readVec4(pt.get_child("specular"));
  point.lightPos = readVec4(pt.get_child("lightPos"));
  point.attenuation = readVec4(pt.get_child("attenuation"));
  point.dist = pt.get_child("dist").get_value<float>();
  scene->dynamic.addPoint(point);
}

// Reads in data for a spot light
void readSpotLight(scene_data *scene, const boost::property_tree::ptree &pt) {
  spot_light_data spot;
  spot.ambient = readVec4(pt.get_child("ambient"));
  spot.diffuse = readVec4(pt.get_child("diffuse"));
  spot.specular = readVec4(pt.get_child("specular"));
  spot.lightPos = readVec4(pt.get_child("lightPos"));
  spot.attenuation = readVec4(pt.get_child("attenuation"));
  spot.lightDir = readVec4(pt.get_child("lightDir"));
  spot.power = pt.get_child("power").get_value<float>();
  spot.dist = pt.get_child("dist").get_value<float>();
  scene->dynamic.addSpot(spot);
}

// Read in all the dynamic lights for the scene
void readDynamicLights(scene_data *scene,
                       const boost::property_tree::ptree &pt) {
  boost::property_tree::ptree::const_iterator iter = pt.front().second.begin();
  for (; iter != pt.front().second.end(); ++iter) {
    if (iter->first == "point")
      readPointLight(scene, iter->second);
    else if (iter->first == "spot")
      readSpotLight(scene, iter->second);
    else {
      std::cerr << "Error - unknown dynamic light encountered: "
                << iter->second.front().first << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  scene->dynamic.create();
}

#include <fstream>
using namespace std;

// Read in the scene data, and load the necessary resources
scene_data *loadScene(const std::string &fileName) {
  std::ifstream t(fileName);
  std::string str;

  t.seekg(0, std::ios::end);
  str.reserve(t.tellg());
  t.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(t)),
             std::istreambuf_iterator<char>());

  struct json_value_s *root = json_parse(str.c_str(), strlen(str.c_str()));
  assert(root->type == json_type_object);

  struct json_object_s *object = (struct json_object_s *)root->payload;

  struct json_object_element_s *a = object->start;

  struct json_object_s *textures_node =
      getChildByName(object->start, "textures");
  struct json_object_s *geometry_node =
      getChildByName(object->start, "geometry");
  struct json_object_s *materials_node =
      getChildByName(object->start, "materials");
  struct json_object_s *objects_node = getChildByName(object->start, "objects");
  struct json_object_s *lighting_node =
      getChildByName(object->start, "lighting");
  struct json_object_s *dynamic_lighting_node =
      getChildByName(object->start, "dynamic_lighting");

  scene_data *scene = new scene_data;
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(fileName, pt);

  readTextures(scene, textures_node);
  readGeometry(scene, geometry_node);
  readMaterials(scene, pt.get_child("materials"), materials_node);
  readObjects(scene, pt.get_child("objects"));
  readLighting(scene, pt.get_child("lighting"));
  readDynamicLights(scene, pt.get_child("dynamic_lighting"));

  free(root);
  return scene;
}