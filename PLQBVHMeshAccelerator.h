/*
 *   pull from ototoi's raytracer : https://github.com/ototoi/pulsar
 */

#ifndef __PULSAR_PLQBVH_MESH_ACCELERATOR_H__
#define __PULSAR_PLQBVH_MESH_ACCELERATOR_H__

#include "Object.h"
#include "Triangle.h"
#include <vector>
#include "ray.h"

namespace pulsar{
    struct Intersection;
    
	class PLQBVHMeshAcceleratorImp;
	class PLQBVHMeshAccelerator
	{
	public:
		PLQBVHMeshAccelerator(std::vector<const Triangle*>& tris);
		~PLQBVHMeshAccelerator();
		virtual bool intersect(Intersection* info, const Ray& r, float tmin, float tmax)const;
		virtual Bound bound()const;
	protected:
		PLQBVHMeshAcceleratorImp* imp_;
	};
}

#endif