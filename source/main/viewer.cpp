#include <source/main/viewer.h>

GLFWwindow* window;

int viewer(void) {
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 1000;
	int height = 750;

	int size = 1;

	float h = (sqrt(3*size) / 2);; // Height of the lateral tetrahedron
	float H = (sqrt(6*size) / 3);; // space height of the tetrahedron


	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Basic Example", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	//we set the cursor at the centre so that it always start on our origin (0,0,0)
	glfwSetCursorPos(window, width / 2, height / 2);


	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile


	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	//glEnable(GL_CULL_FACE);

	glm::vec3 pos0 = glm::vec3(0.0f, 1.0f, 0.0f); //top 
	glm::vec3 pos1 = glm::vec3(-0.5f, 1.0f - H, 0.0f + (h / 2)); //right
	glm::vec3 pos2 = glm::vec3(0.5f, 1.0f - H, 0.0f + (h / 2)); //left
	glm::vec3 pos3 = glm::vec3(0.0f, 1.0f - H, 0.0f - (h / 2)); //middle

	glm::vec3 norm0 = glm::normalize(glm::cross(glm::vec3(pos1-pos0),glm::vec3(pos2-pos0)));
	glm::vec3 norm1 = glm::normalize(glm::cross(glm::vec3(pos3 - pos0), glm::vec3(pos2 - pos0)));
	glm::vec3 norm2 = glm::normalize(glm::cross(glm::vec3(pos3 - pos0), glm::vec3(pos1 - pos0)));
	glm::vec3 norm3 = glm::normalize(glm::cross(glm::vec3(pos1 - pos3), glm::vec3(pos2 - pos3)));

	std::vector<glm::vec3> normals;

	normals.push_back(norm0);
	normals.push_back(norm0);
	normals.push_back(norm0);
	normals.push_back(norm1);
	normals.push_back(norm1);
	normals.push_back(norm1);
	normals.push_back(norm2);
	normals.push_back(norm2);
	normals.push_back(norm2);
	normals.push_back(norm3);
	normals.push_back(norm3);
	normals.push_back(norm3);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() *
		sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, 1.0f-H, 0.0f + (h / 2), //right
		0.5f, 1.0f-H, 0.0f + (h / 2), //left
		0.0f, 1.0f, 0.0f,  //top

		0.0f, 1.0f, 0.0f,  //top
		0.0f, 1.0f-H, 0.0f - (h / 2), //middle
		0.5f, 1.0f - H, 0.0f + (h / 2), //left

		0.0f, 1.0f, 0.0f,  //top
		0.0f, 1.0f - H, 0.0f - (h / 2), //middle
		-0.5f, 1.0f - H, 0.0f + (h / 2), //right

		0.0f, 1.0f - H, 0.0f - (h / 2), //middle
		-0.5f, 1.0f - H, 0.0f + (h / 2), //right
		0.5f, 1.0f - H, 0.0f + (h / 2), //left
	};


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


	static const GLfloat g_color_buffer_data[] = {
		0.0,  1.0,  0.0f,
		0.0,  1.0,  0.0f,
		0.0,  1.0,  0.0f,
		0.0,  0.0,  1.0f,
		0.0,  0.0,  1.0f,
		0.0,  0.0,  1.0f,
		1.0,  0.0,  0.0f,
		1.0,  0.0,  0.0f,
		1.0,  0.0,  0.0f,
		0.0,  1.0,  0.0f,
		0.0,  1.0,  0.0f,
		0.0,  1.0,  0.0f,


	
	};

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	
	GLuint programID = LoadShaders("LightVertexShader.hlsl", "LightFragmentShader.hlsl");

	do {
		// Clear the screen. 
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);
		computeMatricesFromInputs(window, width, height);

		GLuint MatrixID = glGetUniformLocation(programID, "MVP");

		GLuint ViewID = glGetUniformLocation(programID, "V");

		GLuint ModelID = glGetUniformLocation(programID, "M");

		GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

		GLuint CameraID = glGetUniformLocation(programID, "CameraPosition_worldspace");

		//********Calculate the MVP matrix

	  //***********PROJECTION*****************

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

		//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);


	//***********CAMERA*****************

	// Camera matrix

		glm::mat4 View = glm::lookAt(

			glm::vec3(0, 2, -1), // Camera is at (0,0,-1), in World Space

			glm::vec3(0, 0, 0), // and looks at the origin

			glm::vec3(0.25, -0.5, 0)  // Head is up (set to 0,-1,0 to look upside-down)

		);

		//***********MODEL*****************

		glm::mat4 Model = glm::mat4(1.0);


		// Our ModelViewProjection : multiplication of our 3 matrices

		glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around



		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, &View[0][0]);
		glm::vec3 lightPosition = glm::vec3(0,1,1);

		glm::vec3 cameraPosition = glm::vec3(0,2,-1); 

		glUniform3f(LightID, lightPosition.x, lightPosition.y, lightPosition.z);

		glUniform3f(CameraID, cameraPosition.x, cameraPosition.y, cameraPosition.z);


		//changed height value to see full polygon when compiling on vs
		glViewport(0, 0, width, height);


		//********Add the Geometry
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the geometry !
	    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 13); // 3 indices starting at 0 -> 1

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		glViewport(width, 0, width, height);



		//********Add the Geometry
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 3rd attribute buffer : normals

		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);

		glVertexAttribPointer(

			2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.

			3,                  // size

			GL_FLOAT,           // type

			GL_FALSE,           // normalized?

			0,                  // stride

			(void*)0            // array buffer offset

		);


		// Draw the geometry !
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, 13); // 3 indices starting at 0 -> 1

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}