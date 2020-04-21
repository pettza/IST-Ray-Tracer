#include <iostream>
#include <string>
#include <fstream>
#include <IL/il.h>

#include "maths.h"
#include "scene.h"
#include "shapes.h"

Scene::~Scene()
{
	for (auto obj : objects) delete obj;
	objects.clear();

	for (auto light : lights) delete light;
	lights.clear();

	delete camera;

	delete skybox_img[0].img;
	delete skybox_img[1].img;
	delete skybox_img[2].img;
	delete skybox_img[3].img;
	delete skybox_img[4].img;
	delete skybox_img[5].img;
}

int Scene::getNumObjects()
{ return objects.size(); }

void Scene::addObject(Object* o)
{ objects.push_back(o); }

Object* Scene::getObject(unsigned int index)
{
	if (index >= 0 && index < objects.size())
		return objects[index];
	return NULL;
}

int Scene::getNumLights()
{ return lights.size(); }

void Scene::addLight(Light* l)
{ lights.push_back(l); }

Light* Scene::getLight(unsigned int index)
{
	if (index >= 0 && index < lights.size())
		return lights[index];
	return NULL;
}

void Scene::LoadSkybox(const char *sky_dir)
{
	char *filenames[6];
	char buffer[100];
	const char *maps[] = { "/right.jpg", "/left.jpg", "/top.jpg", "/bottom.jpg", "/front.jpg", "/back.jpg" };

	for (int i = 0; i < 6; i++) {
		strcpy_s(buffer, sizeof(buffer), sky_dir);
		strcat_s(buffer, sizeof(buffer), maps[i]);
		filenames[i] = (char*) malloc(sizeof(buffer));
		strcpy_s(filenames[i], sizeof(buffer), buffer);
	}
	
	ILuint ImageName;

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	for (int i = 0; i < 6; i++) {
		ilGenImages(1, &ImageName);
		ilBindImage(ImageName);

		if (ilLoadImage(filenames[i]))  //Image loaded with lower left origin
			printf("Skybox face %d: Image sucessfully loaded.\n", i);
		else
			exit(0);

		ILint bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);

		ILenum format = IL_RGB;
		printf("bpp=%d\n", bpp);
		if (bpp == 24)
			format = IL_RGB;
		else if (bpp == 32)
			format = IL_RGBA;

		ilConvertImage(format, IL_UNSIGNED_BYTE);

		int size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
		skybox_img[i].img = (ILubyte *)malloc(size);
		ILubyte *bytes = ilGetData();
		memcpy(skybox_img[i].img, bytes, size);
		skybox_img[i].resX = ilGetInteger(IL_IMAGE_WIDTH);
		skybox_img[i].resY = ilGetInteger(IL_IMAGE_HEIGHT);
		format == IL_RGB ? skybox_img[i].BPP = 3 : skybox_img[i].BPP = 4;
		ilDeleteImages(1, &ImageName);
	}
	ilDisable(IL_ORIGIN_SET);
}

Color Scene::GetSkyboxColor(Ray& r) {
	float t_intersec;
	Vector cubemap_coords; //To index the skybox

	float ma;
	CubeMap img_side;
	float sc, tc, s, t;
	unsigned int xp, yp, width, height, bytesperpixel;

	//skybox indexed by the ray direction
	cubemap_coords = r.direction;


	if (fabs(cubemap_coords.x) > fabs(cubemap_coords.y)) {
		ma = fabs(cubemap_coords.x);
		cubemap_coords.x >= 0 ? img_side = LEFT : img_side = RIGHT;    //left cubemap at X = +1 and right at X = -1
	}
	else {
		ma = fabs(cubemap_coords.y);
		cubemap_coords.y >= 0 ? img_side = TOP : img_side = BOTTOM; //top cubemap at Y = +1 and bottom at Y = -1
	}

	if (fabs(cubemap_coords.z) > ma) {
		ma = fabs(cubemap_coords.z);
		cubemap_coords.z >= 0 ? img_side = FRONT : img_side = BACK;   //front cubemap at Z = +1 and back at Z = -1
	}

	switch (img_side) {

	case 0:  //right
		sc = -cubemap_coords.z;
		tc = cubemap_coords.y;
		break;

	case 1:  //left
		sc = cubemap_coords.z;
		tc = cubemap_coords.y;
		break;

	case 2:  //top
		sc = -cubemap_coords.x;
		tc = -cubemap_coords.z;
		break;

	case 3: //bottom
		sc = -cubemap_coords.x;
		tc = cubemap_coords.z;
		break;

	case 4:  //front
		sc = -cubemap_coords.x;
		tc = cubemap_coords.y;
		break;

	case 5: //back
		sc = cubemap_coords.x;
		tc = cubemap_coords.y;
		break;
	}

	double invMa = 1 / ma;
	s = (sc * invMa + 1) / 2;
	t = (tc * invMa + 1) / 2;

	width = skybox_img[img_side].resX;
	height = skybox_img[img_side].resY;
	bytesperpixel = skybox_img[img_side].BPP;

	xp = int((width - 1) * s);
	xp < 0 ? 0 : (xp > (width - 1) ? width - 1 : xp);
	yp = int((height - 1) * t);
	yp < 0 ? 0 : (yp > (height - 1) ? height - 1 : yp);

	float red = u8tofloat(skybox_img[img_side].img[(yp*width + xp) * bytesperpixel]);
	float green = u8tofloat(skybox_img[img_side].img[(yp*width + xp) * bytesperpixel + 1]);
	float blue = u8tofloat(skybox_img[img_side].img[(yp*width + xp) * bytesperpixel + 2]);

	return(Color(red, green, blue));
}


////////////////////////////////////////////////////////////////////////////////
// P3F file parsing methods.
//
void next_token(ifstream& file, char *token, const char *name)
{
	file >> token;
	if (strcmp(token, name))
		cerr << "'" << name << "' expected.\n";
}

bool Scene::load_p3f(const char *name)
{
	const int lineSize = 1024;
	string cmd;
	char token[256];
	ifstream file(name, ios::in);
	Material* material = nullptr;
	
	if (file >> cmd)
	{
		while (true)
		{
			if (cmd == "f")  //Material
			{
				double Kd, Ks, Shine, T, ior;
				Color cd, cs;
				
				file >> cd >> Kd >> cs >> Ks >> Shine >> T >> ior;
				
				material = new Material(cd, Kd, cs, Ks, Shine, T, ior);
			}
			else if (cmd == "s")  //Sphere
			{
				Vector center;
				float radius;
				Sphere* sphere;
				Object* obj;
				
				file >> center >> radius;
				sphere = new Sphere(center,radius);
				obj = new Object(material, sphere);
				
				addObject(obj);
			}
			else if (cmd == "box")    //axis aligned box
			{
				Vector minpoint, maxpoint;
				aaBox	*box;
				Object* obj;
				
				file >> minpoint >> maxpoint;
				box = new aaBox(minpoint, maxpoint);
				obj = new Object(material, box);
				addObject(obj);
			}
			else if (cmd == "p")  // Polygon: just accepts triangles for now
			{
				Vector p0, p1, p2;
				Triangle* triangle;
				unsigned total_vertices;
				Object* obj;
				
				file >> total_vertices;
				if (total_vertices == 3)
				{
					file >> p0 >> p1 >> p2;
					triangle = new Triangle(p0, p1, p2);
					obj = new Object(material, triangle);
					
					addObject(obj);
				}
				else
				{
					cerr << "Unsupported number of vertices.\n";
					break;
				}
			}
			else if (cmd == "pl")  // General Plane
			{
				Vector p0, p1, p2;
				Plane* plane;
				Object* obj;
				
				file >> p0 >> p1 >> p2;
				plane = new Plane(p0, p1, p2);
				obj = new Object(material, plane);
				
				addObject(obj);
			}
			else if (cmd == "l")  // Need to check light color since by default is white
			{
				Vector pos;
				Color color;
				
				file >> pos >> color;
				
				addLight(new Light(pos, color));
			}
			else if (cmd == "v")
			{
				Vector up, from, at;
				float fov, hither;
				int xres, yres;
				Camera* camera;
				float focal_ratio; //ratio beteween the focal distance and the viewplane distance
				float aperture_ratio; // number of times to be multiplied by the size of a pixel
				
				next_token (file, token, "from");
				file >> from;
				
				next_token (file, token, "at");
				file >> at;
				
				next_token (file, token, "up");
				file >> up;
				
				next_token (file, token, "angle");
				file >> fov;
				
				next_token (file, token, "hither");
				file >> hither;
				
				next_token (file, token, "resolution");
				file >> xres >> yres;
				
				next_token(file, token, "aperture");
				file >> aperture_ratio;
				
				next_token(file, token, "focal");
				file >> focal_ratio;
				
				// Create Camera
				camera = new Camera(from, at, up, fov, hither, 100.0*hither, xres, yres, aperture_ratio, focal_ratio);
				SetCamera(camera);
			}
			else if (cmd == "bclr")   //Background color
			{
				Color bgcolor;
				file >> bgcolor;
				this->SetBackgroundColor(bgcolor);
			}
			else if (cmd == "env")
			{
				file >> token;
				
				LoadSkybox(token);
				SetSkyBoxFlg(true);
			}
			else if (cmd[0] == '#')
			{
				file.ignore(lineSize, '\n');
			}
			else
			{
				cerr << "unknown command '" << cmd << "'.\n";
				break;
			}

			if (!(file >> cmd)) break;
		}
	}
	
	file.close();
	return true;
};

bool Scene::intercept(const Ray& ray, HitInfo& hitInfo) const
{
	bool isHit = false;

	for (auto obj : objects)
	{
		HitInfo tmp;
		if (obj->intercepts(ray, tmp) && tmp.rayT < hitInfo.rayT)
		{
			isHit = true;
			hitInfo = tmp;
		}
	}

	return isHit;
}

Color Scene::rayTracing(const Ray& ray, int depth, float ior) const //index of refraction of medium 1 where the ray is travelling
{
	// Find intersection if one exists
	HitInfo hitInfo;
	bool isHit = intercept(ray, hitInfo);
	hitInfo.ior = ior;

	// If there's none return background color
	if (!isHit) return GetBackgroundColor();

	// Color accumulator
	Color colorAcc(0.0, 0.0, 0.0);

	// Get direct light
	for (auto light : lights)
		colorAcc += light->GetLighting(*this, hitInfo);

	// If we reached the max recursion depth just return the color so far
	if (depth > maxDepth) return colorAcc;

	// Follow reflection ray
	if (hitInfo.material->GetReflection() > 0.f)
	{
		Color rColor = rayTracing(hitInfo.GetReflectedRay(), depth + 1, ior);
		colorAcc += rColor * hitInfo.material->GetReflection();
	}

	// Follow refraction ray
	if (hitInfo.material->GetTransmittance() > 0.f)
	{
		Ray tRay = hitInfo.GetRefractedRay();
		if (tRay.direction != Vector(0.f, 0.f, 0.f));
		{
			float ior2 = (ior == hitInfo.material->GetRefrIndex()) ? 1.f : hitInfo.material->GetRefrIndex();
			Color tColor = rayTracing(tRay, depth + 1, ior2);
			colorAcc += tColor * hitInfo.material->GetTransmittance();
		}
	}

	return colorAcc;
}