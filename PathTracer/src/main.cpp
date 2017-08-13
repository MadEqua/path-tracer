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

	Texture *checker = new CheckerTexture(Vec3(0.2f, 0.3f, 0.1f), Vec3(0.8f));
	Material *metalChecker = new Metal(checker, 0.15f, 8.0f, 1.0f);
	scene.addTexture(checker);
	scene.addMaterial(metalChecker);

	Texture *tex = new FileTexture("../resources/test3.jpg");
	Material *lam = new Lambertian(tex, 1.0f, 1.0f);
	
	scene.addTexture(tex);
	scene.addMaterial(lam);

	scene.addObject(new Sphere(Vec3(0, -1000, 0), 1000.0f, metalChecker));
	scene.addObject(new Sphere(Vec3(0, 1, 0), 1.0f, lam));
	scene.addObject(new Sphere(Vec3(0, 1, 2.2f), 1.0f, lam));
	scene.addObject(new Sphere(Vec3(2.2f, 1, 0), 1.0f, lam));
}

int main(int argc, char **argv) {

	RenderSettings settings;
	settings.width = 1920;
	settings.height = 1080;
	settings.samples = 200;
	settings.maxRayDepth = 50;
	settings.tileSize = 32;
	settings.threads = 4;
	settings.outputFileName = "output22.bmp";
	
	Scene scene;
	initScene2(scene);

	//Vec3 lookfrom(13, 2, 3);

	float theta = PI / 4.0f;
	float R = 9.0f;
	Vec3 lookfrom(R * cos(theta), 8.0f, R * sin(theta));
	//Vec3 lookfrom(0, 20, 0);
	//Vec3 lookfrom(20, 0.5f, 0);
	//Vec3 lookfrom(0, 0.5f, 20);

	Vec3 upVector(0, 1, 0);
	//Vec3 upVector(0, 0, -1);

	Vec3 lookat(0, 0, 0);
	Camera *camera = new Camera(lookfrom, lookat, upVector,
		20.0f, (float)settings.width / (float)settings.height,
		0.05f, (lookfrom - lookat).length());
	scene.setCamera(camera);

	PathTracer pathTracer(settings, scene);
	pathTracer.printPreRender();
	pathTracer.renderScene();	
	pathTracer.printPostRender();

	std::cin.get();
	return 0;
}