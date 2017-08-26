#include <glm/glm.hpp>

#include "Material.h"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Utils.h"
#include "PathTracer.h"
#include "ConstantTexture.h"
#include "CheckerTexture.h"
#include "FileTexture.h"
#include "LightMaterial.h"
#include "Rectangle.h"
#include "Box.h"
#include "Sky.h"

#include <glm/gtc/random.hpp>


void initScene(Scene &scene) {

	Texture *checkerTex = new CheckerTexture(glm::vec3(0.2f, 0.3f, 0.1f), glm::vec3(0.8f));
	Texture *testTex = new FileTexture("../resources/test3.jpg");
	Texture *constTex = new ConstantTexture(glm::vec3(0.4f, 0.2f, 0.1f));
	Texture *constTex2 = new ConstantTexture(glm::vec3(0.7f, 0.6f, 0.5f));

	scene.addTexture(checkerTex);
	scene.addTexture(testTex);
	scene.addTexture(constTex);
	scene.addTexture(constTex2);

	Material *lam = new Lambertian(checkerTex, 4.0f);
	scene.addMaterial(lam);
	scene.addObject(new Sphere(glm::vec3(0, -1000, 0), 1000.0f, lam));

	for (int a = -21; a < 9; ++a) {
		for (int b = -15; b < 8; ++b) {
			float chooseMaterial = glm::linearRand(0.0f, 1.0f);
			glm::vec3 center(a + 0.9f * glm::linearRand(0.0f, 1.0f), 0.2f, b + 0.9f * glm::linearRand(0.0f, 1.0f));
			if ((center - glm::vec3(4, 0.2f, 0)).length() > 0.9f) {
				Material *material;

				if (chooseMaterial < 0.7f) { //Lambertian

					float chooseTex = glm::linearRand(0.0f, 1.0f);
					Texture *tex;
					if (chooseTex < 0.25f) {
						tex = testTex;
					}
					else {
						tex = new ConstantTexture(glm::vec3(glm::linearRand(0.0f, 1.0f)*glm::linearRand(0.0f, 1.0f),
							glm::linearRand(0.0f, 1.0f)*glm::linearRand(0.0f, 1.0f),
							glm::linearRand(0.0f, 1.0f)*glm::linearRand(0.0f, 1.0f)));
						scene.addTexture(tex);
					}
					material = new Lambertian(tex);
				}
				else if (chooseMaterial < 0.95f) { //Metal

					Texture *tex = new ConstantTexture(glm::vec3(0.5f * (1.0f + glm::linearRand(0.0f, 1.0f)),
						0.5f * (1.0f + glm::linearRand(0.0f, 1.0f)),
						0.5f * (1.0f + glm::linearRand(0.0f, 1.0f))));
					scene.addTexture(tex);

					material = new Metal(tex, 0.3f * glm::linearRand(0.0f, 1.0f));
				}
				else {
					material = new Dielectric(1.5f);
				}

				scene.addMaterial(material);
				scene.addObject(new Sphere(center, 0.2f, material));
			}
		}
	}

	Material *di = new Dielectric(1.5f);
	Material *l = new Lambertian(constTex);
	Material *m = new Metal(constTex2, 0.0f);

	scene.addMaterial(di);
	scene.addMaterial(l);
	scene.addMaterial(m);

	scene.addObject(new Sphere(glm::vec3(0, 1, 0), 1.0f, di));
	scene.addObject(new Sphere(glm::vec3(-4, 1, 0), 1.0f, l));
	scene.addObject(new Sphere(glm::vec3(4, 1, 0), 1.0f, m));
}

void initScene2(Scene &scene) {

	Texture *constTexBlueish = new ConstantTexture(glm::vec3(0.8f, 0.8f, 0.95f));
	Texture *constTexGray = new ConstantTexture(glm::vec3(0.9f));
	Texture *constTexWhite = new ConstantTexture(glm::vec3(2.0f));
	
	Texture *colorfulTex = new FileTexture("../resources/test3.jpg");
	Texture *floorTex = new FileTexture("../resources/199.jpg");

	//Texture *checkerTex = new CheckerTexture(Vec3(0.0f), Vec3(1.0f));
	//Texture *normalMapBrushed = new FileTexture("../resources/181_norm.jpg", false);
	Texture *normalMapFloor = new FileTexture("../resources/199_norm.jpg", false);
	Texture *normalMapCells = new FileTexture("../resources/normal3.jpg", false);
	
	scene.addTexture(constTexBlueish);
	scene.addTexture(constTexGray);
	scene.addTexture(constTexWhite);
	scene.addTexture(colorfulTex);
	//scene.addTexture(checkerTex);
	scene.addTexture(floorTex);
	//scene.addTexture(normalMapBrushed);
	scene.addTexture(normalMapFloor);
	scene.addTexture(normalMapCells);

	Material *floorMat = new Metal(floorTex, normalMapFloor, 0.1f, 500.0f, 500.0f);
	scene.addMaterial(floorMat);

	Material *lam = new Lambertian(colorfulTex, 2, 2);
	scene.addMaterial(lam);

	Material *di = new Dielectric(constTexBlueish, normalMapCells, 1.1f, 3, 3);
	scene.addMaterial(di);

	Material *metal = new Metal(constTexGray, 0.4f, 2, 2);
	scene.addMaterial(metal);

	Material *light = new LightMaterial(constTexWhite);
	scene.addMaterial(light);

	scene.addObject(new Rectangle(Axis::X, 2.5f, 0.0f, 2.0f, 1.0f, light));
	scene.addObject(new Rectangle(Axis::X, -3.0f, 0.0f, 2.5f, 1.5f, light));

	scene.addObject(new Rectangle(Axis::Y, 2.5f, 0, 3.0f, 3.0f, light));
	//scene.addObject(new Rectangle(Axis::Z, -2.5f, 0, 2.6f, 0.2f, light));

	scene.addObject(new Rectangle(Axis::Y, 0.0f, 0.0f, 1000.0f, 1000.0f, floorMat));

	Object *sphere = new Sphere(glm::vec3(0.1f, 1.0f, 1.6f), 1.0f, di);
	sphere->getTransform().scale(glm::vec3(1.5f, 1.0f, 0.4f));
	scene.addObject(sphere);
	scene.addObject(new Sphere(glm::vec3(-1.6f, 0.5f, 0.0f), 0.5f, lam));
	scene.addObject(new Sphere(glm::vec3(1.6f, 0.5f, 0.0f), 0.5f, lam));
}

void initSceneCornell(Scene &scene) {

	Texture *constTexRed = new ConstantTexture(glm::vec3(0.65f, 0.05f, 0.05f));
	Texture *constTexWhite = new ConstantTexture(glm::vec3(0.73f));
	Texture *constTexGreen = new ConstantTexture(glm::vec3(0.12f, 0.45f, 0.15f));
	Texture *constTexWhiteLight = new ConstantTexture(glm::vec3(3.0f));
	Texture *constTexGray = new ConstantTexture(glm::vec3(0.9f));

	scene.addTexture(constTexRed);
	scene.addTexture(constTexWhite);
	scene.addTexture(constTexGreen);
	scene.addTexture(constTexWhiteLight);
	scene.addTexture(constTexGray);

	Material *redMat = new Lambertian(constTexRed);
	scene.addMaterial(redMat);

	Material *whiteMat = new Lambertian(constTexWhite);
	scene.addMaterial(whiteMat);

	Material *greenMat = new Lambertian(constTexGreen);
	scene.addMaterial(greenMat);

	Material *light = new LightMaterial(constTexWhiteLight);
	scene.addMaterial(light);

	Material *di = new Dielectric(1.5f);
	scene.addMaterial(di);

	Material *metal = new Metal(constTexGray, 0.01f);
	scene.addMaterial(metal);

	const float WALL_SIZE = 4.0f;
	scene.addObject(new Rectangle(Axis::X, 1.0f, 0.0f, WALL_SIZE, WALL_SIZE, greenMat));
	scene.addObject(new Rectangle(Axis::X, -1.0f, 180.0f, WALL_SIZE, WALL_SIZE, redMat));
	scene.addObject(new Rectangle(Axis::Z, -1.0f, 0.0f, WALL_SIZE, WALL_SIZE, whiteMat));
	scene.addObject(new Rectangle(Axis::Y, 2.0f, 180.0f, WALL_SIZE, WALL_SIZE, whiteMat));
	scene.addObject(new Rectangle(Axis::Y, 0.0f, 0.0f, WALL_SIZE, WALL_SIZE, whiteMat));

	const float LIGHT_SIZE = 1.0f;
	scene.addObject(new Rectangle(Axis::Y, 1.99f, 0.0f, LIGHT_SIZE, LIGHT_SIZE, light));

	//const float SPHERE_RADIUS = 0.35f;
	//scene.addObject(new Sphere(glm::vec3(0.5f, SPHERE_RADIUS, 0.1f), SPHERE_RADIUS, di));
	//scene.addObject(new Sphere(glm::vec3(-0.5f, SPHERE_RADIUS, -0.4f), SPHERE_RADIUS, metal));

	const float BOX_W = 0.6f;
	const float SPHERE_RADIUS = 0.25f;
	scene.addObject(new Box(glm::vec3(-0.34f, 1.2f / 2.0f, -0.4f), glm::vec3(BOX_W, 1.2f, BOX_W), glm::vec3(0.0f, 18.0f, 0.0f), whiteMat));
	scene.addObject(new Box(glm::vec3(0.34f, BOX_W / 2.0f, 0.2f), glm::vec3(BOX_W, BOX_W, BOX_W), glm::vec3(0.0f, -18.0f, 0.0f), whiteMat));
	scene.addObject(new Sphere(glm::vec3(-0.34f, 1.2f + SPHERE_RADIUS, -0.4f), SPHERE_RADIUS, metal));
	scene.addObject(new Sphere(glm::vec3(0.34f, BOX_W + SPHERE_RADIUS, 0.2f), SPHERE_RADIUS, di));
}

void initSceneSky(Scene &scene) {

	Texture *constTexWhite = new ConstantTexture(glm::vec3(1.0f));
	Texture *constTexRed = new ConstantTexture(glm::vec3(0.65f, 0.05f, 0.05f));
	Texture *constTexGreen = new ConstantTexture(glm::vec3(0.12f, 0.45f, 0.15f));
	Texture *constTexBlue = new ConstantTexture(glm::vec3(0.02f, 0.05f, 0.75f));

	Texture *baseTex = new FileTexture("../resources/marble3.jpg");

	scene.addTexture(constTexWhite);
	scene.addTexture(constTexRed);
	scene.addTexture(constTexGreen);
	scene.addTexture(constTexBlue);
	scene.addTexture(baseTex);

	Material *baseMat = new Metal(baseTex, 0.5f);
	scene.addMaterial(baseMat);
	
	Material *redMetal = new Metal(constTexRed, 0.0f);
	scene.addMaterial(redMetal);

	Material *greenMetal = new Metal(constTexGreen, 0.0f);
	scene.addMaterial(greenMetal);

	Material *blueMetal = new Metal(constTexBlue, 0.0f);
	scene.addMaterial(blueMetal);

	Material *di = new Dielectric(1.5f);
	scene.addMaterial(di);

	Material *mirrorMat = new Metal(constTexWhite, 0.0f);
	scene.addMaterial(mirrorMat);
	
	const float BASE_SIZE = 12.0f;
	const float BASE_H = 0.5f;
	scene.addObject(new Box(glm::vec3(0.0f, -BASE_H/2.0f, 0.0f), glm::vec3(BASE_SIZE, BASE_H, BASE_SIZE), glm::vec3(0.0f), baseMat));
	
	const float SPHERE_RADIUS = 1.1f;
	const float SPHERE_DIST = 4.4f;
	scene.addObject(new Sphere(glm::vec3(0.0f, SPHERE_RADIUS*1.4f, 0.0f), SPHERE_RADIUS*1.4f, di));
	scene.addObject(new Sphere(glm::vec3(SPHERE_DIST, SPHERE_RADIUS, 0.0f), SPHERE_RADIUS, blueMetal));
	scene.addObject(new Sphere(glm::vec3(-SPHERE_DIST, SPHERE_RADIUS, 0.0f), SPHERE_RADIUS, redMetal));
	scene.addObject(new Sphere(glm::vec3(0.0f, SPHERE_RADIUS, SPHERE_DIST), SPHERE_RADIUS, greenMetal));
	scene.addObject(new Sphere(glm::vec3(0.0f, SPHERE_RADIUS, -SPHERE_DIST), SPHERE_RADIUS, greenMetal));

	//const float BOX_SIZE = 5.0f;
	//const float BOX_HEIGHT = 3.0f;
	//const float BOX_THICKNESS = 0.07f;
	//scene.addObject(new Box(glm::vec3(4.0f, BOX_HEIGHT / 2.0f, 4.0f), glm::vec3(BOX_SIZE, BOX_HEIGHT, BOX_THICKNESS), glm::vec3(0.0f, 45.0f, 0.0f), mirrorMat));
	//scene.addObject(new Box(glm::vec3(4.0f, BOX_HEIGHT / 2.0f, -4.0f), glm::vec3(BOX_SIZE, BOX_HEIGHT, BOX_THICKNESS), glm::vec3(0.0f, -45.0f, 0.0f), mirrorMat));
	//scene.addObject(new Box(glm::vec3(-4.0f, BOX_HEIGHT / 2.0f, -4.0f), glm::vec3(BOX_SIZE, BOX_HEIGHT, BOX_THICKNESS), glm::vec3(0.0f, 45.0f, 0.0f), mirrorMat));

	//scene.setSky(new Sky("../resources/stpeters_probe.hdr"));
	//scene.setSky(new Sky("../resources/grace_probe.hdr"));
	scene.setSky(new Sky("../resources/rnl_probe.hdr"));
}

int main(int argc, char **argv) {

	RenderSettings settings;
	settings.width = 500;
	settings.height = 500;
	settings.samples = 256;
	settings.maxRayDepth = 8;
	settings.tileSize = 32;
	settings.threads = 4;

	settings.luminosityThreshold = 3.0f;
	settings.gaussianKernelSize = 128;
	settings.gaussianSigma = 2.0f;

	settings.key = 0.18f;
	settings.lumWhite = 2.0f;

	settings.outputFileName = "output58";
	settings.outputFileFormat = FileFormat::PNG;
	
	Scene scene;
	initSceneCornell(scene);

	//glm::vec3 lookfrom(13, 2, 3); //scene1
	//glm::vec3 lookfrom(0, 3, 8);
	glm::vec3 lookfrom(0.0f, 1.0f, 3.5f); //Cornell
	//glm::vec3 lookfrom(-7, 3.5f, 8); //sceneSky

	glm::vec3 upVector(0, 1, 0);
	//glm::vec3 upVector(0, 0, -1);

	//glm::vec3 lookat(0, 0, 0);
	glm::vec3 lookat(0, 1.0f, 0); //Cornell

	Camera *camera = new Camera(lookfrom, lookat, upVector,
		40.0f, (float)settings.width / (float)settings.height,
		0.3f, glm::length(lookfrom - lookat));
	scene.setCamera(camera);

	PathTracer pathTracer(settings, scene);
	pathTracer.renderScene();	

	std::cin.get();
	return 0;
}