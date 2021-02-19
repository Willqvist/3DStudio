//
// Created by William Lundqvist on 2020-11-02.
//

#ifndef GLPROJECT_TOOLS_H
#define GLPROJECT_TOOLS_H

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "cvec3.h"
#include <chrono>
#include <glad/glad.h>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtx/quaternion.hpp>

/**
 * returns true if char chr is a space
 * @param chr the char to check if it is a space
 * @return true if chr is a space.
 */
static bool isSpace(unsigned char chr) {
    return !std::isspace(chr);
}
namespace GLProj {
    class Tools {
    public:

        /**
         * splits a string at a given delimiter.
         * @param str  the string to split
         * @param delimiter the delimiter to split as
         * @param skip how many characters at the beginning to skip
         * @return a vector of strings of the splitted string.
         */
        static std::vector<std::string> split(std::string &str, char delimiter, int skip = 0,bool keepEmpty =false) {
            std::vector<std::string> result;
            std::stringstream stream(str);
            std::string splittedString;
            int skipped = 0;
            while (getline(stream, splittedString, delimiter)) {
                if (skipped < skip) {
                    skipped++;
                    continue;
                }
                if(splittedString.length() > 0 || keepEmpty)
                    result.push_back(splittedString);
            }
            return result;
        }

        static void splitFast(std::string &str, char delimiter, std::vector<std::string>& result, int skip = 0,bool keepEmpty =false) {
            int start = 0;
            int end = str.find_first_of(delimiter);
            result.clear();
            int skipped = 0;
            while (end <= std::string::npos) {
                if (skipped < skip) {
                    skipped++;
                } else {
                    std::string strSplit = str.substr(start, end - start);
                    if (strSplit.length() > 0 || keepEmpty)
                        result.emplace_back(strSplit);

                    if (end == std::string::npos)
                        break;
                }
                start=end+1;
                end = str.find_first_of(delimiter, start);
            }
        }

        /**
         * creates a transformation matrix.
         * @param position the position
         * @param scale the scale
         * @param rotation the rotation
         * @return a transformation matrix mat4.
         */
        static glm::mat4 createModelMatrix(const glm::vec3& position,const glm::vec3& scale,const glm::vec3 & rotation) {
            glm::mat4 out = glm::mat4(1.0f);
            out = glm::translate(out, position);
            out = glm::rotate(out, glm::radians(rotation.x), glm::cvec3::right);
            out = glm::rotate(out, glm::radians(rotation.y), glm::cvec3::up);
            out = glm::rotate(out, glm::radians(rotation.z), glm::cvec3::forward);
            out = glm::scale(out, scale);
            return out;
        }

        /**
         * creates a viewmatrix.
         * @param position the position
         * @param scale the scale
         * @param rotation the rotation
         * @return a vieewmatrix mat4.
         */
        static glm::mat4 createViewMatrix(const glm::vec3& position,const glm::vec3& scale,const glm::vec3& rotation) {
            glm::mat4 out = glm::mat4(1.0f);
            out = glm::rotate(out, glm::radians(-rotation.x), glm::cvec3::right);
            out = glm::rotate(out, glm::radians(-rotation.y), glm::cvec3::up);
            out = glm::rotate(out, glm::radians(-rotation.z), glm::cvec3::forward);
            out = glm::translate(out, -position);
            out = glm::scale(out, scale);
            return out;
        }

        /**
         * returns time in milliseconds since start of program.
         * @return
         */
        static int timeSinceStart() {
            auto current_time = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
        }

        /**
         * trims away spaces at beginning and end of a string
         * @param str the string to trim.
         */
        static void trim(std::string &str) {
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), isSpace));
            str.erase(std::find_if(str.rbegin(), str.rend(), isSpace).base(), str.end());
        }

        /**
         * Code provided by stefan from Umeå University for course computer graphics, course code: 5DV179
         * @param filePath
         * @return
         */
        static std::string readFile(std::string filePath) {
            std::string src;
            std::string line;

            std::fstream fs(filePath, std::ios::in);
            if(fs.fail()) {
                fprintf(stderr,"Could not load shader file: %s\n",filePath.c_str());
                exit(1);
            }

            while (!fs.eof()) {
                getline(fs, line);
                src.append(line + '\n');
            }
            fs.close();
            return src;
        }

        /**
         * custom read function that reads a shader and imports new file at #import.
         * @param filePath path to the file
         * @param depth depth, should not be changed.
         * @return the parsed shader source.
         */
        static std::string parseShader(std::string filePath,int depth = 0) {
            if(depth > 4) {
                fprintf(stderr,"import depth exceeded 4.");
                exit(1);
            }
            std::string src;
            std::string line;
            std::fstream fs(filePath, std::ios::in);
            if(fs.fail()) {
               fprintf(stderr,"Could not load shader file: %s\n",filePath.c_str());
                exit(1);
            }

            while (!fs.eof()) {
                getline(fs, line);
                if(line.substr(0,7) == "#import") {
                    std::string val = &line[8];
                    src.append(parseShader(val,depth+1));
                } else {
                    src.append(line + '\n');
                }
            }
            fs.close();
            return src;
        }

    private:
        static inline std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    };
}
#endif //GLPROJECT_TOOLS_H
