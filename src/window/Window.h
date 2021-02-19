//
// Created by William Lundqvist on 2020-11-02.
//

#ifndef GLPROJECT_WINDOW_H
#define GLPROJECT_WINDOW_H

#include <memory>
#include <string>
#include <glm/vec4.hpp>
#include <vector>

namespace GLProj {
    /**
     * Interface for eventlistener for a window.
     */
    class WindowEventListener {
        public:
            virtual void onKey(int key, int state) = 0;
            virtual void onMouseMove(int x,int y) = 0;
            virtual void onMousePress(int key, int state) = 0;
            virtual void onWindowResize(int width,int height) = 0;
            virtual void onClear() = 0;
    };

    /**
     * Adapter class for WindowEventListener.
     */
    class WindowEventListenerAdapter: public WindowEventListener {
    public:
        virtual void onKey(int key, int state){}
        virtual void onMouseMove(int x,int y){}
        virtual void onMousePress(int key, int state){}
        virtual void onWindowResize(int width,int height){}
        virtual void onClear(){}
    };

    /**
     * abstract Window class for representing a window on the screen.
     */
    class Window {
    public:
        Window(int width, int height,const std::string &title, bool vsync) : width(width), height(height), title(title),
                                                                        vsync(vsync) {}
        /**
         * Updates the window
         * @return  true if the update was successfull, false if the window should close.
         */
        virtual bool update() = 0;

        /**
         * clears the window screen to color set in background(), otherwise black.
         */
        virtual void clear() = 0;

        /**
         * disposes the screen
         */
        virtual void dispose() = 0;

        /**
         * sets is the mouse should be visible or not
         * @param enabled true if mouse should be visible.
         */
        virtual void mouseEnabled(bool enabled) = 0;

        /**
         * adds a listener to events happening in this window
         * @param listener the listener to listen to events.
         */
        void addListener(std::shared_ptr<WindowEventListener> listener) {
            listeners.push_back(listener);
        }

        /**
         * sets a background color when clearing the screen.
         * @param r  red value, between 0 to 1
         * @param g green value, between 0 to 1
         * @param b blue value, between 0 to 1
         * @param a alpha value, between 0 to 1
         */
        void background(float r, float g, float b,float a) {
            bgColor.r = r;
            bgColor.g = g;
            bgColor.b = b;
            bgColor.a = a;
        }

        int getWidth() {
            return width;
        };

        int getHeight() {
            return height;
        };

        bool isVsync() {
            return vsync;
        };

        /**
         * @return true if the window is minimized or out of focus
         */
        bool isSleeping() {
            return sleeping;
        };

        const std::string &getTitle() {
            return title;
        };

        /**
         * creates a new window
         * @param width width of window, in pixels
         * @param height height of window, in pixels
         * @param title title of the window
         * @param vsync true to use vsync
         * @return a new shared_ptr to a new Window.
         */
        static std::shared_ptr<Window> create(int width, int height,const std::string &title, bool vsync = false);

        virtual ~Window() = default;

    protected:
        int width, height;
        std::string title;
        bool vsync;
        glm::vec4 bgColor = glm::vec4(0,0,0,1);
        std::vector<std::shared_ptr<WindowEventListener>> listeners;
        bool sleeping = false;
    };
}
#endif //GLPROJECT_WINDOW_H
