#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"
#include "D:\OpenGL\GLFW_GLAD_GLUT_GLEW_cmake_project\src\CuboRubik\Solver\solve.h"
#include "D:\OpenGL\GLFW_GLAD_GLUT_GLEW_cmake_project\src\CuboRubik\Solver\random.h"

using namespace std;

template<class T>
class Point {
public:
	T x, y, z, c1, c2, c3, t1,t2;
	Point(T _x = 0, T _y = 0, T _z = 0, glm::vec3 color=glm::vec3(0.0f,0.0f,0.0f), T _t1=0, T _t2=3) {
		x = _x;
		y = _y;
		z = _z;
		c1 = color.x;
		c2 = color.y;
		c3 = color.z;
		t1 = _t1;
		t2 = _t2;
	}
};

class Cubito {
public:
	vector<Point<float>> vertices;
	float dim;
	unsigned int VAO, VBO;
	glm::vec3 center;
	
	const char* title_texture;
	unsigned int texture;
	//glm::mat4 model;
	float acc = 1.0f;
	glm::mat4 prevRot=glm::mat4(1.0f);

	Cubito(float _dim,glm::vec3 _pos,vector<glm::vec3> color) {			//Depende del tipo hacemos push a sus vertices
		title_texture = "allspark.jpg";
		dim = _dim;
		center = _pos;
		//FRONT
		vertices.push_back(Point<float>(-dim, -dim, dim,  color[0], 0.0f, 0.0f));
		vertices.push_back(Point<float>(dim, -dim, dim,   color[0], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, dim,    color[0], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, dim,    color[0], 1.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, dim, dim,   color[0], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, dim,  color[0], 0.0f, 0.0f));
		//UP
		vertices.push_back(Point<float>(-dim, dim, -dim,  color[1], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, -dim,   color[1], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, dim,    color[1], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, dim,    color[1], 1.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, dim,   color[1], 0.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, -dim,  color[1], 0.0f, 1.0f));
		//LEFT
		vertices.push_back(Point<float>(-dim, dim, dim,   color[2], 1.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, -dim,  color[2], 1.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[2], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[2], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, dim,  color[2], 0.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, dim, dim,   color[2], 1.0f, 0.0f));
		//DOWN
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[3], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim,  color[3], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, dim,   color[3], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, -dim, dim,   color[3], 1.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, -dim, dim,  color[3], 0.0f, 0.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[3], 0.0f, 1.0f));
		//RIGHT
		vertices.push_back(Point<float>(dim, dim, dim,    color[4], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, -dim,   color[4], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim,  color[4], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim,  color[4], 0.0f, 1.0f));
		vertices.push_back(Point<float>(dim, -dim, dim,   color[4], 0.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, dim,    color[4], 1.0f, 0.0f));
		//BACK
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[5], 0.0f, 0.0f));
		vertices.push_back(Point<float>(dim, -dim, -dim,  color[5], 1.0f, 0.0f));
		vertices.push_back(Point<float>(dim, dim, -dim,   color[5], 1.0f, 1.0f));
		vertices.push_back(Point<float>(dim, dim, -dim,   color[5], 1.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, dim, -dim,  color[5], 0.0f, 1.0f));
		vertices.push_back(Point<float>(-dim, -dim, -dim, color[5], 0.0f, 0.0f));

	}
	void genBuffers();
	void load_create_texture(Shader*);
	void draw(Shader*,glm::vec3);
	void move(glm::vec3,Shader*,float);
	void breath(bool);
	void panting(bool,float,int,int);
	void expand();
	void twist(Shader*, glm::vec3, float);
	void rotateInTheFloor(Shader*,int,float);
	void vibrate(float);
	void deleteBuffers();
private:
	void updateBuffers();
	void move_vertices(glm::vec3,float);
};

class Rubik {
	GLFWwindow* window;
	Shader* shader;
	vector<Cubito> cubitos;
	float dim;
	unordered_map<char, vector<int>>  parts;
	bool inhala = true;

public:
	bool drawing = true;
	vector<Rubik*> brothers;
	int timesTwist;
	vector<string> shuffle;
	vector<string> solution;
	float degrees;
	Rubik(GLFWwindow* _window, Shader* _shader, float _dim = 0.2f) {
		window = _window;
		shader = _shader;
		timesTwist = 0;
		dim = _dim;
		degrees = -1.0f;
		float pos=dim*2+(dim/4);	//Positivo
		float neg=-1*(pos);		//Negativo
		glm::vec3 cubePositions[] = {
			glm::vec3(neg,  pos, pos),				//0Parte frontal
			glm::vec3(0.0f, pos, pos),				//1
			glm::vec3(pos,  pos, pos),				//2
			glm::vec3(neg,  0.0f,pos),				//3
			glm::vec3(0.0f,  0.0f, pos),			//4
			glm::vec3(pos,  0.0f,pos),				//5
			glm::vec3(neg,  neg, pos),				//6
			glm::vec3(0.0f, neg, pos),				//7
			glm::vec3(pos,  neg, pos),				//8

			glm::vec3(neg,  pos, 0.0f),				//9
			glm::vec3(0.0f, pos, 0.0f),				//10
			glm::vec3(pos,  pos, 0.0f),				//11
			glm::vec3(neg,  0.0f, 0.0f),			//12
			glm::vec3(pos,  0.0f, 0.0f),			//13
			glm::vec3(neg,  neg, 0.0f),				//14
			glm::vec3(0.0f, neg, 0.0f),				//15
			glm::vec3(pos,  neg, 0.0f),				//16


			glm::vec3(neg,  pos, neg),				//17
			glm::vec3(0.0f, pos, neg),				//18Parte trasera
			glm::vec3(pos,  pos, neg),				//19
			glm::vec3(neg,  0.0f,neg),				//20
			glm::vec3(0.0f, 0.0f,neg),				//21
			glm::vec3(pos,  0.0f,neg),				//22
			glm::vec3(neg,  neg, neg),				//23
			glm::vec3(0.0f, neg, neg),				//24
			glm::vec3(pos,  neg, neg)				//25
		};

		
		glm::vec3 colors[] = {
			glm::vec3(0.0f,0.0f,0.0f),			//Negro
			glm::vec3(1.0f,1.0f,1.0f),			//Blanco
			glm::vec3(1.0f,0.5f,0.0f),			//Naranja  
			glm::vec3(0.8f,0.15f,1.0f),			//Morado  
			glm::vec3(1.0f,0.025f,0.25f),			//Rojo
			glm::vec3(0.224f,1.0f,0.078f),			//Verde 
			glm::vec3(0.15f,0.35f,1.0f)			//Azul
		};
		int assignColor[][6] = {
			{4,1,5,0,0,0},	//Frontal
			{4,1,0,0,0,0},
			{4,1,0,0,6,0},
			{4,0,5,0,0,0},
			{4,0,0,0,0,0},
			{4,0,0,0,6,0},
			{4,0,5,3,0,0},
			{4,0,0,3,0,0},
			{4,0,0,3,6,0},	
			{0,1,5,0,0,0},	//Medio
			{0,1,0,0,0,0},
			{0,1,0,0,6,0},
			{0,0,5,0,0,0},
			{0,0,0,0,6,0},
			{0,0,5,3,0,0},
			{0,0,0,3,0,0},
			{0,0,0,3,6,0},
			{0,1,5,0,0,2}, //Trasera
			{0,1,0,0,0,2},
			{0,1,0,0,6,2},
			{0,0,5,0,0,2},
			{0,0,0,0,0,2},
			{0,0,0,0,6,2},
			{0,0,5,3,0,2},
			{0,0,0,3,0,2},
			{0,0,0,3,6,2}	
		};
		parts.insert({'F', vector<int>{0,1,2,3,4,5,6,7,8} });
		parts.insert({'B', vector<int>{19,18,17,22,21,20,25,24,23}});
		parts.insert({'L', vector<int>{17,9,0,20,12,3,23,14,6}});
		parts.insert({'R', vector<int>{2,11,19,5,13,22,8,16,25}});
		parts.insert({'U', vector<int>{17,18,19,9,10,11,0,1,2}});
		parts.insert({'D', vector<int>{6,7,8,14,15,16,23,24,25}});

		vector<glm::vec3> cubeColor;
		for(int i=0;i<26;i++){
			for (int j = 0; j < 6; j++) {
				cubeColor.push_back(colors[assignColor[i][j]]);
			}
			cubitos.push_back(Cubito(dim,cubePositions[i],cubeColor));
			cubeColor.clear();
		}

		genBuffers();
		load_create_texture();
	}
	void draw();
	void updateCurrentPart(char);
	void updateParts(char);
	void fillShuffle(char);
	void move(char);
	void solve(vector<string> sol);
	void setSolve();
	void breath();
	void expand();
	void twist();
	void vibrate();
	void magnet();
	void deleteBuffers();
private:
	void genBuffers();
	void load_create_texture();
	void fall();
};