#pragma once
#include "G2L.h"
#include <initializer_list>
#include <memory>

namespace G2L {
	class billBoard {
	private:
		// GL IDs
		unsigned int VBO_vertex, VAO, EBO;
		unsigned int VBO_colors;
		unsigned int VBO_centers;
		
		
		//Params
		static const unsigned int vLen, iLen;
		unsigned int maxParticles = 1000;


	public:
		// data arrays. 
		//THESE ARE NOT MANAGED HERE. THEY MUST BE SUPPLIED
		//These store data for ALL particles
		float * centers; // ALL Particles
		float * colors;
		//These are the shared ones, common to all particles
		static const unsigned int indices[];
		static const float vertices[];
		
		//params
		int numParticles = 1;

		// GL IDs
		unsigned int shaderProgram;
		

		

		billBoard();
		void init();
		void update();
		void draw();


	};
}