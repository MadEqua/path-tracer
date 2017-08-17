#include "Vec3.h"
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

void initScene(Scene &scene) {
	
	Texture *checkerTex = new CheckerTexture(Vec3(0.2f, 0.3f, 0.1f), Vec3(0.8f));
	Texture *testTex = new FileTexture("../resources/test3.jpg");
	Texture *constTex = new ConstantTexture(Vec3(0.4f, 0.2f, 0.1f));
	Texture *constTex2 = new ConstantTexture(Vec3(0.7f, 0.6f, 0.5f));

	scene.addTexture(checkerTex);
	scene.addTexture(testTex);
	scene.addTexture(constTex);
	scene.addTexture(constTex2);
	
	Material *lam = new Lambertian(checkerTex, 4.0f);
	scene.addMaterial(lam);
	scene.addObject(new Sphere(Vec3(0, -1000, 0), 1000.0f, lam));
	
	for (int a = -21; a < 9; ++a) {
		for (int b = -15; b < 8; ++b) {
			float chooseMaterial = Utils::random0To1();
			Vec3 center(a + 0.9f * Utils::random0To1(), 0.2f, b + 0.9f * Utils::random0To1());
			if ((center - Vec3(4, 0.2f, 0)).length() > 0.9f) {
				Material *material;
				
				if (chooseMaterial < 0.7f) { //Lambertian

					float chooseTex = Utils::random0To1();
					Texture *tex;
					if (chooseTex < 0.25f) {
						tex = testTex;
					}
					else {
						tex = new ConstantTexture(Vec3(Utils::random0To1()*Utils::random0To1(),
							Utils::random0To1()*Utils::random0To1(),
							Utils::random0To1()*Utils::random0To1()));
						scene.addTexture(tex);
					}
					material = new Lambertian(tex);
				}
				else if (chooseMaterial < 0.95f) { //Metal

					Texture *tex = new ConstantTexture(Vec3(0.5f * (1.0f + Utils::random0To1()),
						0.5f * (1.0f + Utils::random0To1()),
						0.5f * (1.0f + Utils::random0To1())));
					scene.addTexture(tex);
					
					material = new Metal(tex, 0.3f * Utils::random0To1());
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

	scene.addObject(new Sphere(Vec3(0, 1, 0), 1.0f, di));
	scene.addObject(new Sphere(Vec3(-4, 1, 0), 1.0f, l));
	scene.addObject(new Sphere(Vec3(4, 1, 0), 1.0f, m));
}

void initScene2(Scene &scene) {

	Texture *constTexBlueish = new ConstantTexture(Vec3(0.8f, 0.8f, 0.95f));
	Texture *constTexGray = new ConstantTexture(Vec3(0.9f));
	Texture *constTexWhite = new ConstantTexture(Vec3(2.0f));
	
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

	Material *floorMat = new Metal(floorTex, normalMapFloor, 0.1f, 1.0f, 1.0f);
	scene.addMaterial(floorMat);

	Material *lam = new Lambertian(colorfulTex, 2, 2);
	scene.addMaterial(lam);

	Material *di = new Dielectric(constTexBlueish, normalMapCells, 1.1f, 2, 2);
	scene.addMaterial(di);

	Material *metal = new Metal(constTexGray, 0.4f, 2, 2);
	scene.addMaterial(metal);

	Material *light = new LightMaterial(constTexWhite);
	scene.addMaterial(light);

	//Material *lightGobo = new LightMaterial(checkerTex, 5, 5);
	//scene.addMaterial(lightGobo);

	scene.addObject(new Rectangle(Axis::X, 2.5f, 0, 1, -2, 2, light));
	scene.addObject(new Rectangle(Axis::X, -3.0f, 0.0f, 0.5f, -0.5f, 0.5f, light));
	scene.addObject(new Rectangle(Axis::Y, 2.5f, -2, 2, -2, 2, light));
	scene.addObject(new Rectangle(Axis::Z, -2.5f, -1.2f, 1.2f, 0, 0.3f, light));
	scene.addObject(new Rectangle(Axis::Y, 0.0f, floorMat));

	scene.addObject(new Sphere(Vec3(0.0f, 1.0f, 0.0f), 1.0f, metal));
	scene.addObject(new Sphere(Vec3(-1.6f, 0.5f, 0.0f), 0.5f, lam));
	scene.addObject(new Sphere(Vec3(1.6f, 0.5f, 0.0f), 0.5f, di));
}

void initSceneCornell(Scene &scene) {

	Texture *constTexRed = new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f));
	Texture *constTexWhite = new ConstantTexture(Vec3(0.73f));
	Texture *constTexGreen = new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f));
	Texture *constTexWhiteLight = new ConstantTexture(Vec3(3.0f));

	scene.addTexture(constTexRed);
	scene.addTexture(constTexWhite);
	scene.addTexture(constTexGreen);
	scene.addTexture(constTexWhiteLight);

	Material *redMat = new Lambertian(constTexRed);
	scene.addMaterial(redMat);

	Material *whiteMat = new Lambertian(constTexWhite);
	scene.addMaterial(whiteMat);

	Material *greenMat = new Lambertian(constTexGreen);
	scene.addMaterial(greenMat);

	Material *light = new LightMaterial(constTexWhiteLight);
	scene.addMaterial(light);

	scene.addObject(new Rectangle(Axis::X, 1.0f, redMat));
	scene.addObject(new Rectangle(Axis::X, -1.0f, greenMat));
	scene.addObject(new Rectangle(Axis::Z, -1.0f, whiteMat));
	scene.addObject(new Rectangle(Axis::Y, 2.0f, whiteMat));
	scene.addObject(new Rectangle(Axis::Y, 0.0f, whiteMat));

	const float LIGHT_SIZE = 0.2f;
	scene.addObject(new Rectangle(Axis::Y, 1.95f, -LIGHT_SIZE, LIGHT_SIZE, -LIGHT_SIZE, LIGHT_SIZE, light));
}

int main(int argc, char **argv) {

	RenderSettings settings;
	settings.width = 300;
	settings.height = 300;
	settings.samples = 20;
	settings.maxRayDepth = 50;
	settings.tileSize = 32;
	settings.threads = 4;
	settings.outputFileName = "output32";
	settings.outputFileFormat = FileFormat::PNG;
	
	Scene scene;
	initSceneCornell(scene);

	//Vec3 lookfrom(0, 4, 13);
	//Vec3 lookfrom(0, 20, 0);
	//Vec3 lookfrom(20, 0.5f, 0);
	//Vec3 lookfrom(7, 0.5f, 0);
	Vec3 lookfrom(0.0f, 1.0f, 3.0f); //Cornell

	Vec3 upVector(0, 1, 0);
	//Vec3 upVector(0, 0, -1);

	//Vec3 lookat(0, 0, 0);
	//Vec3 lookat(5, 0, 5);
	Vec3 lookat(0, 1.0f, 0); //Cornell

	Camera *camera = new Camera(lookfrom, lookat, upVector,
		40.0f, (float)settings.width / (float)settings.height,
		0.01f, (lookfrom - lookat).length());
	scene.setCamera(camera);

	PathTracer pathTracer(settings, scene);
	pathTracer.printPreRender();
	pathTracer.renderScene();	
	pathTracer.printPostRender();

	std::cin.get();
	return 0;
}