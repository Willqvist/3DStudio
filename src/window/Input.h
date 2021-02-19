//
// Created by William Lundqvist on 2020-11-10.
//

#ifndef GLPROJECT_INPUT_H
#define GLPROJECT_INPUT_H

#include <glm/vec2.hpp>
#include "Window.h"
#define KEY_SIZE 500
#define MOUSE_SIZE 8
namespace GLProj {

    /**
     * struct containing state for a key.
     */
    struct KeyState {
        bool down;
        bool pressed;
        bool released;
    };

    /**
     * class handling Input from a keyboard.
     */
    class Input : public WindowEventListener {
    public:
        void onKey(int key, int state) override;

        void onMouseMove(int x, int y) override;

        void onMousePress(int key, int state) override;

        void onWindowResize(int width, int height) override;

        /**
         * clears all previous states from all keys.
         */
        void onClear() override;

        /**
         * return true if a key is held down
         * @param key the key to check if being held down
         * @return true if key is held down
         */
        static bool isKeyDown(int key);

        /**
        * return true if a mouse is held down
        * @param key the key to check if being held down
        * @return true if mouse is held down
        */
        static bool isMouseDown(int mouse);

        /**
         * return true if a mouse has been pressed.
         * @param mouse the mouse key to check
         * @return  true if mouse has been pressed.
         */
        static bool isMousePressed(int mouse);

        /**
         * return true if a mouse has been released.
         * @param mouse the mouse key to check
         * @return  true if a mouse has been released.
         */
        static bool isMouseRelease(int mouse);

        /**
         * returns the speed of the mouse in x and y directions, between each onClear().
         * @return the speed of the mouse.
         */
        static glm::vec2 deltaMouse();
    private:
        static KeyState keys[KEY_SIZE];
        static KeyState mouseKeys[MOUSE_SIZE];
        static glm::vec2 mousePos;
        static glm::vec2 prevMousePos;
    };
}
#endif //GLPROJECT_INPUT_H
