#pragma once

#include "mge/core/AbstractGame.hpp"

class DebugHud;

/**
 * An example subclass of AbstractGame showing how we can setup a scene.
 */
class TheScene: public AbstractGame
{
    //PUBLIC FUNCTIONS

	public:
		TheScene();
		virtual ~TheScene();

		

        //override initialize so we can add a DebugHud
        virtual void initialize();

	protected:
	    //override so we can construct the actual scene
        virtual void _initializeScene();

	    //override render to render the hud as well.
	    virtual void _render();

	private:
		DebugHud* _hud;                   //hud display

        void _updateHud();

		TheScene(const TheScene&);
		TheScene& operator=(const TheScene&);
};

