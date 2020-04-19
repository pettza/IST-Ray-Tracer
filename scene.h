#pragma once

#include <vector>
using namespace std;

#include "camera.h"
#include "light.h"
#include "boundingBox.h"
#include "object.h"
#include "il/il.h"

//Skybox images constant symbolics
enum CubeMap { RIGHT, LEFT, TOP, BOTTOM, FRONT, BACK };

constexpr int maxDepth = 10;

class Scene
{
public:
	Scene() = default;
	virtual ~Scene();
	
	Camera* GetCamera() { return camera; }
	
	Color GetBackgroundColor() const { return bgColor; }
	void SetBackgroundColor(Color a_bgColor) { bgColor = a_bgColor; }
	
	Color GetSkyboxColor(Ray& r);
	
	bool GetSkyBoxFlg() { return SkyBoxFlg; }
	void SetSkyBoxFlg(bool skyboxFlg) { SkyBoxFlg = skyboxFlg; }
	
	void LoadSkybox(const char*);
	void SetCamera(Camera* camera) { this->camera = camera; }

	int getNumObjects();
	void addObject(Object* o);
	Object* getObject( unsigned int index );

	int getNumLights();
	void addLight(Light* l);
	Light* getLight(unsigned int index);

	bool load_p3f(const char *name);  //Load NFF file method
	
	Color rayTracing(const Ray& ray, int depth, float ior1) const;

	bool intercept(const Ray& ray, HitInfo& hitInfo) const;

private:
	vector<Object *> objects;
	vector<Light *> lights;

	Camera* camera;
	Color bgColor;  //Background color

	bool SkyBoxFlg = false;

	struct {
		ILubyte *img;
		unsigned int resX;
		unsigned int resY;
		unsigned int BPP; //bytes per pixel
	} skybox_img[6];
};
