#pragma once
#include "Cubito.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/gtx/string_cast.hpp>
#include <Windows.h>

void Cubito::genBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	//vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return;
}

void Cubito::load_create_texture(Shader* ourShader) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(title_texture, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader->use();
	ourShader->setInt("texture1", 0);
	
	return;
}

void Cubito::updateBuffers() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	return;
}

void Cubito::draw(Shader* ourShader, glm::vec3 pivot = glm::vec3(0.0f, 0.0f, 0.0f)) {

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, center);
	ourShader->setMat4("model", model);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	return;
}

void Cubito::move_vertices(glm::vec3 pivot, float degrees) {
	glm::vec4 vertex;
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	for (int i = 0; i < vertices.size(); i++)
	{
		vertex = glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0f);

		vertex = glm::rotate(rotMatrix, glm::radians(degrees), pivot) * vertex;

		vertices[i].x = vertex.x;
		vertices[i].y = vertex.y;
		vertices[i].z = vertex.z;
	}
	updateBuffers();

	return;
}

void Cubito::move(glm::vec3 pivot, Shader* ourShader, float degrees) {

	glm::mat4 model = glm::mat4(1.0f);

	//Rotamos los centros
	glm::mat4 rot = glm::mat4(1.0f);
	rot = glm::rotate(rot, glm::radians(degrees), pivot);
	glm::vec4 r = rot * glm::vec4(center, 1.0f);
	center = r;

	model = glm::translate(model, center);
	move_vertices(pivot, degrees);
	ourShader->setMat4("model", model);


	glDrawArrays(GL_TRIANGLES, 0, 36);

	return;
}

void Cubito::breath(bool inhala) {
	float growth;
	glm::vec3 v = center;
	if (inhala) v /= 1500;
	else v /= -1500;
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);

	center = transform * glm::vec4(center, 1.0f);


	glm::vec4 vertex;
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	for (int k = 0; k < vertices.size(); k++)
	{
		vertex = glm::vec4(vertices[k].x, vertices[k].y, vertices[k].z, 1.0f);

		growth = v.x;
		if (inhala) {
			growth = 0.0005;
		}
		else {
			growth = -0.0005;
		}
		vertex = glm::scale(scaleMatrix, glm::vec3(1.0f + growth, 1.0f + growth, 1.0f + growth)) * vertex;

		vertices[k].x = vertex.x;
		vertices[k].y = vertex.y;
		vertices[k].z = vertex.z;
	}
	updateBuffers();

	return;
}

void Cubito::panting(bool inhala, float growth, int range, int speed) {
	glm::vec3 v = center;
	if (inhala) v /= range + speed;
	else v /= -1 * (range + speed);
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);

	center = transform * glm::vec4(center, 1.0f);


	glm::vec4 vertex;
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	for (int k = 0; k < vertices.size(); k++)
	{
		vertex = glm::vec4(vertices[k].x, vertices[k].y, vertices[k].z, 1.0f);

		vertex = glm::scale(scaleMatrix, glm::vec3(1.0f + growth, 1.0f + growth, 1.0f + growth)) * vertex;

		vertices[k].x = vertex.x;
		vertices[k].y = vertex.y;
		vertices[k].z = vertex.z;
	}
	updateBuffers();

	return;
}

void Cubito::expand() {
	glm::vec3 v = center;
	v /= 100;
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);

	center = transform * glm::vec4(center, 1.0f);

	updateBuffers();

	return;
}

void Cubito::twist(Shader* shader, glm::vec3 randomPivot, float growth) {
	glm::vec3 v = center;
	v /= 2000;
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);

	center = transform * glm::vec4(center, 1.0f);


	glm::vec4 vertex;
	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	for (int k = 0; k < vertices.size(); k++)
	{
		vertex = glm::vec4(vertices[k].x, vertices[k].y, vertices[k].z, 1.0f);

		vertex = glm::scale(scaleMatrix, glm::vec3(1.0f + growth, 1.0f + growth, 1.0f + growth)) * vertex;

		vertices[k].x = vertex.x;
		vertices[k].y = vertex.y;
		vertices[k].z = vertex.z;
	}


	glm::mat4 model = glm::mat4(1.0f);

	//Rotamos los centros
	glm::mat4 rot = glm::mat4(1.0f);
	rot = glm::rotate(rot, glm::radians(2.0f), randomPivot);
	glm::vec4 r = rot * glm::vec4(center, 1.0f);
	center = r;

	model = glm::translate(model, center);

	move_vertices(randomPivot, 2.0f);

	shader->setMat4("model", model);

	return;
}

void Cubito::rotateInTheFloor(Shader* shader, int distance, float randDirection = 0.0f) {
	glm::vec3 v = center;
	v.y = 0.0f;
	if (randDirection != 0.0f) {
		if (v.x < 0) v.x -= randDirection;
		else v.x += randDirection;
		if (v.z < 0) v.z -= randDirection;
		else v.z += randDirection;
	}
	v /= distance;
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);
	center = transform * glm::vec4(center, 1.0f);

	//Rotamos los centros
	glm::vec3 pivot = center;
	pivot.y = 0.0f;
	float temp90deg = pivot.x;
	pivot.x = pivot.z;
	pivot.z = -1 * pivot.x;

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 rot = glm::mat4(1.0f);
	float magnitudeRotation = ((rand() % 100) / 100);
	rot = glm::rotate(rot, glm::radians(magnitudeRotation), pivot);
	glm::vec4 r = rot * glm::vec4(center, 1.0f);
	center = r;

	model = glm::translate(model,center);

	move_vertices(pivot, 2.0f);

	shader->setMat4("model", model);

	return;
}

void Cubito::vibrate(float magnitude) {
	glm::vec3 v = glm::vec3(magnitude, magnitude, magnitude);
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, v);
	center = transform * glm::vec4(center, 1.0f);

	return;
}

void Cubito::deleteBuffers() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	return;
}


void Rubik::genBuffers() {
	for (int i = 0; i < cubitos.size(); i++){
		cubitos[i].genBuffers();
	}

	return;
}

void Rubik::load_create_texture() {
	for (int i = 0; i < cubitos.size(); i++) {
		cubitos[i].load_create_texture(shader);
	}
	
	return;
}

void Rubik::draw() {			
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < cubitos.size(); i++)
	{
		cubitos[i].draw(shader);
	}
	for (int i = 0; i < brothers.size(); i++) {
		if (brothers[i]->drawing) {
			for (int j = 0; j < brothers[i]->cubitos.size(); j++)
			{
				brothers[i]->cubitos[j].draw(shader);
			}

		}
	}
	glfwSwapBuffers(window);
	glfwPollEvents();

	return;
}

void Rubik::updateCurrentPart(char p) {             //Actualizamos las posiciones de los cubitos de la capa actual
	vector<int>* pv = &(parts.find(p)->second);
	vector<int> temp = (*pv);

	for (int i = 0; i < 3; i++) {
		(*pv)[i] = temp[6 - (i * 3)];                  //0,1,2
		(*pv)[i + 3] = temp[10 - ((i + 1) * 3)];     //3,4,5
		(*pv)[i + 6] = temp[i + 12 - ((i + 1) * 4)];   //6,7,8
	}

	return;
}

void Rubik::updateParts(char movedChoosen) {  //Sides
	vector<int>* pMoved = &(parts.find(movedChoosen)->second);         //Side moved
	vector<char> updateSides;   //Sides to update R,D,L,U,F,B
	vector<vector<int>> updateIndex; //Index to update divided in parts of a side
	switch (movedChoosen)
	{
	case 'F':
		updateSides = vector<char>{ 'U','R','D','L' };
		updateIndex = vector<vector<int>>{ {6,7,8}, {0,3,6}, {0,1,2}, {2,5,8} };
		break;
	case 'B':
		updateSides = vector<char>{ 'U','L','D','R' };       //Actualizar desde B
		updateIndex = vector<vector<int>>{ {2,1,0}, {0,3,6}, {8,7,6}, {2,5,8} };
		break;
	case 'L':
		updateSides = vector<char>{ 'U','F','D','B' };
		updateIndex = vector<vector<int>>{ {0,3,6}, {0,3,6}, {6,3,0}, {2,5,8} };
		break;
	case 'R':
		updateSides = vector<char>{ 'U','B','D','F' };
		updateIndex = vector<vector<int>>{ {8,5,2}, {0,3,6}, {2,5,8}, {2,5,8} };
		break;
	case 'D':
		updateSides = vector<char>{ 'F','R','B','L' };
		updateIndex = vector<vector<int>>{ {6,7,8}, {6,7,8}, {8,7,6}, {8,7,6} };
		break;
	case 'U':
		updateSides = vector<char>{ 'B','R','F','L' };
		updateIndex = vector<vector<int>>{ {2,1,0}, {2,1,0}, {0,1,2}, {0,1,2} };
		break;
	default:
		break;
	}
	updateCurrentPart(movedChoosen);
	vector<vector<int>> sortedIndex = { {0,1,2},{2,5,8},{6,7,8},{0,3,6} }; //Orden de los indices que actualizaremos
	vector<int>* side_updating;          //Side updating
	for (int i = 0; i < 4; i++) {
		side_updating = &(parts.find(updateSides[i])->second);
		for (int j = 0; j < 3; j++) {
			(*side_updating)[updateIndex[i][j]] = (*pMoved)[sortedIndex[i][j]];
		}
	}

	return;
}

void Rubik::fillShuffle(char sideMove) {
	if (degrees == -1) {
		shuffle.push_back(string(1, sideMove));
		cout << sideMove << " ";
	}
	else {
		string s(1, sideMove);
		s.push_back('\'');
		shuffle.push_back(s);
		cout << s << " ";
	}
	
	return;
}

void Rubik::move(char sideMove) {
	vector<int>* pv = &(parts.find(sideMove)->second);
	vector<bool> moviendo(26, false);
	for (int k = 0; k < 90; k++) {
		for (int j = 0; j < pv->size(); j++) {
			cubitos[(*pv)[j]].move(cubitos[(*pv)[4]].center, shader, degrees);      //[4] es el pivot
			//cubitos[(*pv)[j]].draw(ourShader,cubitos[(*pv)[4]].center);      //Pivot
			moviendo[(*pv)[j]] = true;
		}
		draw();
	}

	return;
}

void Rubik::solve(vector<string> sol) {
	solution = sol;
	char sideMoved;
	degrees = -1.0f;
	for (int i = 0; i < solution.size(); i++) {
		if (i > 1) {
			std::cout << "\nSOLUTION: ";
			for (int j = i; j < solution.size(); j++) std::cout << solution[j] << " ";
		}
		if (solution[i].size() == 1) {
			sideMoved = solution[i].c_str()[0];
			move(sideMoved);
			updateParts(sideMoved);
		}
		else if (solution[i].size() == 2) {
			if (solution[i][1] == '\'') {
				degrees = 1.0f;
				sideMoved = solution[i].c_str()[0];
				move(sideMoved);
				updateParts(sideMoved);
				updateParts(sideMoved);
				updateParts(sideMoved);
				degrees = -1.0f;
			}
			else {
				sideMoved = solution[i].c_str()[0];
				move(sideMoved);
				updateParts(sideMoved);
				move(sideMoved);
				updateParts(sideMoved);
			}
		}
	}
	std::cout << "\n\nSHUFFLE: ";

	return;
}

void Rubik::setSolve() {
	vector<string> move = get_solution(to_cube_not(shuffle));
	cout << "\nSOLUTION: ";
	for (int i = 0; i < move.size(); i++) cout << move[i] << " ";
	solve(move);
	shuffle.clear();
	"\nSHUFFLE: ";

	return;
}

void Rubik::breath() {
	for (int j = 0; j < 4000; j++) {
		if (j != 0 && j % 1000 == 0) {
			if (inhala) inhala = false;
			else inhala = true;
		}
		for (int i = 0; i < cubitos.size(); i++) {
			cubitos[i].breath(inhala);
	    }
		draw();
	}
	inhala = true;

	return;
}

void Rubik::expand() {
	float growth = 0.0007;
	int range = 1000;
	int steps = 0;
	int speed = 200;
	glm::vec3 v;

	for (int i = range; i > 0; i -= speed) {
		steps += i * 2;
	}

	int count = 0;
	int turn = 0;
	for (int j = 0; j < steps; j++) {
		if (count != 0 && count == range) {
			if (inhala) {
				inhala = false;
			}
			else {
				inhala = true;
				range -= speed;
				growth *= 1.2;
				turn += 1;
			}
			count = 0;
			growth *= -1;
		}
		count += 1;
		for (int i = 0; i < cubitos.size(); i++) {
			cubitos[i].panting(inhala,growth,range,speed);
		}
		draw();
	}

	inhala = true;
	growth *= -1;
	count = 0;
	for (int i = 0; i < 5;) {
		if (count != 0 && count == range) {
			if (inhala) {
				inhala = false;
			}
			else {
				inhala = true;
				i += 1;
				if (range >= 50 * 2) range -= 50;
			}
			count = 0;
			growth *= -1;
		}
		count += 1;
		for (int i = 0; i < cubitos.size(); i++) {
			cubitos[i].panting(inhala, growth, range, speed);
		}
		draw();
	}
	inhala = true;

	for (int j = 0; j < 150; j++) {
		for (int i = 0; i < cubitos.size(); i++) {
			cubitos[i].expand();
		}
		draw();
	}

	return;
}

void Rubik::twist() {
	int idx_pivots[6] = { 4,21,12,13,16,10 };
	glm::vec3 randomPivot = cubitos[idx_pivots[rand() % 6]].center;
	float growth = 0.0005;
	for (int i = 0; i < 90; i++) {
		for (int j = 0; j < cubitos.size(); j++) {
			cubitos[j].twist(shader,randomPivot,growth);
		}
		draw();
	}

	return;
}

void Rubik::fall() {
	vector<int> current;
	glm::vec3 v;
	glm::vec3 pivot;
	int step = 1;
	char firstPart;
	vector<int> cubesFirstPart;
	vector<int> cubesSecondPart;
	float rangeSecondPart = 0.0f;
	float temp90deg = 0.0f;
	float randDirection = 0.4f*brothers.size(); 
	//FALL first part
	for (std::unordered_map<char, vector<int>>::iterator it = parts.begin(); step == 1 && it != parts.end(); ++it) {
		current = it->second;
		cubesFirstPart = current;
		rangeSecondPart = cubitos[current[4]].center.y;
		if (cubitos[current[4]].center.y < -dim) {
			firstPart = it->first;
			for (int times = 0; times < 350; times++) {
				for (int i = 0; i < current.size(); i++) {
					v = glm::vec3(0.0f, -0.01f, 0.0f);
					glm::mat4 transform(1.0f);
					transform = glm::translate(transform, v);
					cubitos[current[i]].center = transform * glm::vec4(cubitos[current[i]].center, 1.0f);
				}
				draw();
			}
			step++;
		}
	}
	if (rangeSecondPart < 0.0f) rangeSecondPart *= -1;
	rangeSecondPart -= 0.01;		//Disminuir rango para evitar elegir cubos muy cercanos al rango 
	for (int j = 0; j < cubitos.size(); j++) {
		if (cubitos[j].center.y < rangeSecondPart && cubitos[j].center.y > -rangeSecondPart) {
			cubesSecondPart.push_back(j);
		}
	}
	for (int times = 0; times < 400; times++) {
		for (int i = 0; i < cubesFirstPart.size(); i++) {			//Girando y moviendose en el piso
			cubitos[cubesFirstPart[i]].rotateInTheFloor(shader,200);
			
		}
		for (int i = 0; i < cubesSecondPart.size(); i++) {
			v = glm::vec3(0.0f, -0.01f, 0.0f);
			glm::mat4 transform(1.0f);
			transform = glm::translate(transform, v);
			cubitos[cubesSecondPart[i]].center = transform * glm::vec4(cubitos[cubesSecondPart[i]].center, 1.0f);
		}
		draw();
	}
	step++;


	char thirdPart;
	if (firstPart == 'B') thirdPart = 'F';
	else if (firstPart == 'F') thirdPart = 'B';
	else if (firstPart == 'R') thirdPart = 'L';
	else if (firstPart == 'L') thirdPart = 'R';
	else if (firstPart == 'U') thirdPart = 'D';
	else thirdPart = 'U';
	
	current = parts.find(thirdPart)->second;
	for (int times = 0; times < 450; times++) {
		for (int i = 0; i < cubesSecondPart.size(); i++) {			//Girando y moviendose en el piso
			cubitos[cubesSecondPart[i]].rotateInTheFloor(shader,250,randDirection);
		}
		for (int i = 0; i < current.size(); i++) {
			v = glm::vec3(0.0f, -0.01f, 0.0f);
			glm::mat4 transform(1.0f);
			transform = glm::translate(transform, v);
			cubitos[current[i]].center = transform * glm::vec4(cubitos[current[i]].center, 1.0f);
		}
		draw();
	}

	for (int times = 0; times < 400; times++) {
		for (int i = 0; i < current.size(); i++) {			//Girando y moviendose en el piso
			cubitos[current[i]].rotateInTheFloor(shader,400,randDirection);
		}
		draw();
	}

	return;
}

void Rubik::vibrate() {
	float magnitude = 0.05f;
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < cubitos.size(); j++) {
			cubitos[j].vibrate(magnitude);
		}
		draw();
		for (int j = 0; j < cubitos.size(); j++) {
			cubitos[j].vibrate(-magnitude);
		}
		draw();
	}
	fall();

	return;
}

void Rubik::magnet() {
	glm::vec3 vAttract;
	float magnitude = 0.07f;
	for (int i = 0; i < cubitos.size(); i++) {
		for (int j = 0; j < 70; j++) {
			cubitos[i].vibrate(magnitude);
			draw();
			cubitos[i].vibrate(-magnitude);
			draw();
		}
		for (int times = 0; times < 150; times++) {
			vAttract = cubitos[i].center;
			vAttract /= -100;
			glm::mat4 transform(1.0f);
			transform = glm::translate(transform, vAttract);
			cubitos[i].center = transform * glm::vec4(cubitos[i].center, 1.0f);

			draw();
		}
	}

	return;
}

void Rubik::deleteBuffers() {
	for (int i = 0; i < cubitos.size(); i++)
	{
		cubitos[i].deleteBuffers();
	}

	return;
}