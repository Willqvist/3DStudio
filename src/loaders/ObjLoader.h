//
// Created by William Lundqvist on 2020-10-30.
//

#ifndef PROJEKT_OBJLOADER_H
#define PROJEKT_OBJLOADER_H


#include <memory>
#include <string>
#include <map>
#include <glm/ext/quaternion_geometric.hpp>
#include "OBJ.h"
#include <unordered_map>

struct v3;
struct Face;

struct KeyFuncs;
typedef std::unordered_map<v3,std::vector<Face>,KeyFuncs,KeyFuncs> FaceMap;

namespace GLProj {

    /**
     * settings used when loading an OBJ.
     */
    struct ObjLoaderSettings {
        bool normalized; // true if object should be normalized into a NDC cube. [2,2,2] cube
        bool generateNormals; // true if it should generate vertex normals if no normals were given
    };

    /**
     * class used when loading obj files. it caches previously loaded obj files.
     */
    class ObjLoader {
    public:

        /**
         * loads a given object from a url with given settings.
         * @param url the url/path to the object file.
         * @param settings settings while loading the obj file.
         * @return
         */
        static std::shared_ptr<OBJ> loadOBJ(const std::string& url, const ObjLoaderSettings& settings);
    private:
        static inline std::map<std::string, std::shared_ptr<OBJ>> cache;

        /**
         * normalizes an obj into a 1x1x1 cube.
         * @param obj the obj to normalize
         * @param left the left most vertex
         * @param right the right most vertex
         * @param front the furthest vertex.
         * @param back the nearest vertex.
         * @param top the top highest vertex.
         * @param bottom the lowest vertex.
         * @param vertexSize size of the vertex, usually 3 or 4.
         */
        static void normalize(std::shared_ptr<OBJ> &obj, const v3 &left, const v3 &right, const v3 &front,
                       const v3 &back, const v3 &top, const v3 &bottom,
                       int vertexSize);

        /**
         * calculates the vertex normals from faces.
         * @param map a map where a key is a vertex and values is a list of faces for that vertex.
         * @param obj the obj to apply the new normals to
         * @param faces a list of all faces.
         */
        static void calculateVertexNormals(FaceMap& map,std::shared_ptr<OBJ>& obj, std::vector<Face>& faces);

    };
}


#endif //PROJEKT_OBJLOADER_H
