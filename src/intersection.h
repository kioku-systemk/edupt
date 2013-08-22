#ifndef _INTERSECTION_H_
#define _INTERSECTION_H_

#include "vec.h"
#include "constant.h"
#include "material.h"

namespace edupt {
    
    struct Hitpoint {
        double distance_;
        Vec normal_;
        Vec position_;
        
        Hitpoint() : distance_(kINF), normal_(), position_() {}
    };
    
    struct Intersection {
        Hitpoint hitpoint_;
        int object_id_;
        Material* mat_;
        
        Intersection() : object_id_(-1),mat_(0) {}
    };
    
};


#include "Vector3.h"

namespace pulsar{
	
	class Object;
	class Material;
	
	struct Intersection
	{
		float t;//distance to intersection
		Vector3 position;	//world position
		Vector3 normal;		//world normal
		Vector3 tex;		//
		const Object* pObject;//
		const Material*     pMaterial;	//
        int material;
	};
	
}

#endif