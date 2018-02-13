#include "billboardMask.h"
namespace G2L {
	billBoard::billBoard()	{	}

	const float billBoard::vertices[] = {
		-0.5f, -0.5f, 0.0f, //bottom left
		0.5f, -0.5f, 0.0f,	//bottom right
		-0.5f, 0.5f, 0.0f, //top left
		0.5f, 0.5f, 0.0f, //top right
	};
	const unsigned int billBoard::indices[] = {
		0,2,3,
		0,1,3,
	}; // %TODO: Remove. Not retured
	const unsigned int billBoard::vLen = 12;
	const unsigned int billBoard::iLen = 6;

	void billBoard::init()
	{
		// Vertex Shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char *vShaderSource =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aIn;\n"
			"layout (location = 1) in vec4 aCen;\n"
			"layout (location = 2) in vec4 aColor;\n"
			"out vec4 ourColor;\n"
			//We want to:
				//clone aPos for all Center sets
				//offset its coordinates by centers
				//Scale its coordinates by scale?
			"void main()\n"
			"{\n"
			"float scale = 0.01 * aCen.w;\n"
			"vec3 aPos = aIn * scale;\n"
			"gl_Position = vec4(aPos.x+aCen.x,aPos.y+aCen.y,aPos.z+aCen.z,1.0);\n"
	/*		"gl_Position = vec4(aCen.x,aCen.y,aCen.z,1.0);\n"
			"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"*/
			"ourColor = aColor;\n"
			"}\n";
		glShaderSource(vertexShader, 1, &vShaderSource, NULL);
		glCompileShader(vertexShader);
		int success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "\tERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
			std::cin.ignore();
			exit(-1);
		}

		// Create and Set Fragment Shader
		//TODO: add layout above to input a color vector
		const char*fShaderSource =
			"#version 330 core\n"
			//"#extension GL_ARB_separate_shader_objects : enable\n"
			"uniform vec4 timeColor;\n"
			"out vec4 FragColor;\n"
			"in vec4 ourColor;\n"
			"void main()\n"
			"{\n"
			"FragColor = ourColor;\n"
			//"FragColor = timeColor;\n"
			//"FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
			"}\n";
		unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fShaderSource, NULL);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "\tERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
			std::cin.ignore();
			exit(-1);
		}

		//Link shaders

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragShader);
		glLinkProgram(shaderProgram);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "\tERROR::SHADER::PROGRAM::COMPILATION::FAILED\n" << infoLog << std::endl;
			std::cin.ignore();
			exit(-1);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragShader);


		// Buffers-----------------------------
		//initialise Vertex Array Object

		glGenVertexArrays(1, &VAO);

		// vertices
		glGenBuffers(1, &VBO_vertex); // Create a buffer for transfer to GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);// Copy vertices array into buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		

		// Centers (X,Y,Z,size)
		glGenBuffers(1, &VBO_centers);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_centers);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		

		// Colors (r,g,b,alpha)
		glGenBuffers(1, &VBO_colors);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*vLen, indices, GL_STATIC_DRAW);
		
	}
	void billBoard::update()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_centers);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numParticles * 4 * sizeof(GLfloat), centers);

		glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numParticles * 4 * sizeof(GLfloat), colors);


	}
	void billBoard::draw()
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);// Copy vertices array into buffer
		glVertexAttribPointer(
			0,//which attribute we want to configure (to match shader layout = #)
			3,//size (x,y,z)
			GL_FLOAT,//dtype
			GL_FALSE,//should it normalize?
			3 * sizeof(float)//Stride: space between consecutive sets (only non-trivial when there's space between sets)
			, (void*)0//beginning offset
		);
		glEnableVertexAttribArray(0);

		// Centers (X,Y,Z,size)
		glBindBuffer(GL_ARRAY_BUFFER, VBO_centers);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
			4 * sizeof(float),//auto-compute stride, but if manual == 4*sizeof(float)
			(void*)0
		);
		glEnableVertexAttribArray(1);

		// Colors (r,g,b,alpha)
		glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
			4 * sizeof(float),//auto-compute stride, but if manual == 4*sizeof(float)
			(void*)0
		);
		glEnableVertexAttribArray(2);

		glVertexAttribDivisor(0, 0); // divisor = 0 means never change
		glVertexAttribDivisor(1, 1); // divisor = n means update every n frames
		glVertexAttribDivisor(2, 1); 

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, vLen, GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(GL_TRIANGLES, vLen, GL_UNSIGNED_INT, 0, numParticles);
		glBindVertexArray(0);

	}
}

