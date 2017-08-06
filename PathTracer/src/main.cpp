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

void initScene(Scene &scene) {
	
	Material *lam = new Lambertian(Vec3(0.5f));
	scene.addMaterial(lam);
	scene.addObject(new Sphere(Vec3(0, -1000, 0), 1000.0f, lam));
	
	for (int a = -21; a < 9; ++a) {
		for (int b = -15; b < 8; ++b) {
			float chooseMaterial = Utils::random0To1();
			Vec3 center(a + 0.9f * Utils::random0To1(), 0.2f, b + 0.9f * Utils::random0To1());
			if ((center - Vec3(4, 0.2f, 0)).length() > 0.9f) {
				Material *material;
				
				if (chooseMaterial < 0.8f) {
					material = new Lambertian(Vec3(Utils::random0To1()*Utils::random0To1(),
						Utils::random0To1()*Utils::random0To1(),
						Utils::random0To1()*Utils::random0To1()));
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

	Material *di = new Dielectric(1.5f);
	Material *l = new Lambertian(Vec3(0.4f, 0.2f, 0.1f));
	Material *m = new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f);

	scene.addMaterial(di);
	scene.addMaterial(l);
	scene.addMaterial(m);

	scene.addObject(new Sphere(Vec3(0, 1, 0), 1.0f, di));
	scene.addObject(new Sphere(Vec3(-4, 1, 0), 1.0f, l));
	scene.addObject(new Sphere(Vec3(4, 1, 0), 1.0f, m));
}

int main(int argc, char **argv) {

	RenderSettings settings;
	settings.width = 198;
	settings.height = 108;
	settings.samples = 10;
	settings.maxRayDepth = 50;
	settings.tileSize = 16;
	settings.outputFileName = "tt.bmp";
	
	Scene scene;
	initScene(scene);

	Vec3 lookfrom(13, 2, 3);
	Vec3 lookat(0, 0, 0);
	Camera *camera = new Camera(lookfrom, lookat, Vec3(0, 1, 0),
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