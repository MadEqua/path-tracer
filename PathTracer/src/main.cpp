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

void initScene(Scene &scene) {
	
	Texture *tex = new CheckerTexture(Vec3(0.2f, 0.3f, 0.1f), Vec3(0.9f, 0.9f, 0.9f));
	Material *lam = new Lambertian(tex, 4.0f);
	scene.addTexture(tex);
	scene.addMaterial(lam);
	scene.addObject(new Sphere(Vec3(0, -1000, 0), 1000.0f, lam));
	
	for (int a = -21; a < 9; ++a) {
		for (int b = -15; b < 8; ++b) {
			float chooseMaterial = Utils::random0To1();
			Vec3 center(a + 0.9f * Utils::random0To1(), 0.2f, b + 0.9f * Utils::random0To1());
			if ((center - Vec3(4, 0.2f, 0)).length() > 0.9f) {
				Material *material;
				
				if (chooseMaterial < 0.8f) {
					/*Texture *tex = new ConstantTexture(Vec3(Utils::random0To1()*Utils::random0To1(),
						Utils::random0To1()*Utils::random0To1(),
						Utils::random0To1()*Utils::random0To1()));
					scene.addTexture(tex);*/
					material = new Lambertian(tex, Utils::random0To1() + 2.0f, Utils::random0To1() + 2.0f);
				}
				else if (chooseMaterial < 0.95f) {
					material = new Metal(Vec3(0.5f * (1.0f + Utils::random0To1()),
						0.5f * (1.0f + Utils::random0To1()),
						0.5f * (1.0f + Utils::random0To1())),
						0.3f * Utils::random0To1());
				}
				else {
					material = new Dielectric(1.5f);
				}

				scene.addMaterial(material);
				scene.addObject(new Sphere(center, 0.2f, material));
			}
		}
	}

	tex = new ConstantTexture(Vec3(0.4f, 0.2f, 0.1f));
	scene.addTexture(tex);

	Material *di = new Dielectric(1.5f);
	Material *l = new Lambertian(tex);
	Material *m = new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f);

	scene.addMaterial(di);
	scene.addMaterial(l);
	scene.addMaterial(m);

	scene.addObject(new Sphere(Vec3(0, 1, 0), 1.0f, di));
	scene.addObject(new Sphere(Vec3(-4, 1, 0), 1.0f, l));
	scene.addObject(new Sphere(Vec3(4, 1, 0), 1.0f, m));
}

void initScene2(Scene &scene) {

	Texture *tex = new CheckerTexture(Vec3(0.2f, 0.3f, 0.1f), Vec3(0.9f, 0.9f, 0.9f));
	Material *lam = new Lambertian(tex, 2.0f, 1.0f);
	scene.addTexture(tex);
	scene.addMaterial(lam);

	scene.addObject(new Sphere(Vec3(0, -1000, 0), 1000.0f, lam));
	scene.addObject(new Sphere(Vec3(0, 1, 0), 1.0f, lam));
	scene.addObject(new Sphere(Vec3(0, 1, 2), 1.0f, lam));
	scene.addObject(new Sphere(Vec3(2, 1, 0), 1.0f, lam));
}

int main(int argc, char **argv) {

	RenderSettings settings;
	settings.width = 600;
	settings.height = 400;
	settings.samples = 20;
	settings.maxRayDepth = 50;
	settings.tileSize = 32;
	settings.threads = 4;
	settings.outputFileName = "output18.bmp";
	
	Scene scene;
	initScene(scene);

	Vec3 lookfrom(13, 2, 3);

	//Vec3 lookfrom(6, 2, 9);
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