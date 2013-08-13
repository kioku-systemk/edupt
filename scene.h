#ifndef	_SCENE_H_
#define	_SCENE_H_

#include "constant.h"
#include "sphere.h"
#include "intersection.h"

#include "MOE/SceneGraph.h"
#include "MOE/MrzLoader.h"

#include "PLQBVHMeshAccelerator.h"
#include "intersection.h"


namespace edupt {
    
const Material materials[] = {
    Material(Color(), Color(0.75, 0.75, 0.75),REFLECTION_TYPE_DIFFUSE),//照明
	Material(Color(10,10,10), Color(),REFLECTION_TYPE_DIFFUSE),// 左
	Material(Color(), Color(0.25, 0.25, 0.75),REFLECTION_TYPE_DIFFUSE),// 右
	Material(Color(), Color(0.75, 0.75, 0.75),REFLECTION_TYPE_DIFFUSE),// 奥
	Material(Color(), Color(0.75, 0.75, 0.75),REFLECTION_TYPE_DIFFUSE),// 手前
	Material(Color(), Color(0.75, 0.75, 0.75),REFLECTION_TYPE_DIFFUSE),// 床
	Material(Color(), Color(0.75, 0.75, 0.75),REFLECTION_TYPE_DIFFUSE),// 天井
	Material(Color(), Color(1,1,1)*.99, REFLECTION_TYPE_SPECULAR),// 鏡
	Material(Color(), Color(1,1,1)*.99, REFLECTION_TYPE_REFRACTION),//ガラス
	Material(Color(9,9,9), Color(), REFLECTION_TYPE_DIFFUSE),//照明
};

pulsar::PLQBVHMeshAccelerator* g_bvh = 0;
std::vector< std::vector<pulsar::Vector3> > g_tribufs;
std::vector< pulsar::Triangle > g_ts;
std::vector<const pulsar::Triangle*> g_tris;
std::vector<Material> g_mats;
edupt::Vec g_campos=Vec(0.0, 50.0, 320.0), g_camdir=Vec(0.0, 0.0, -1.0), g_camup=Vec(0.0, 1.0, 0.0);

const edupt::Vec& GetCameraPos()
{
    return g_campos;
}

const edupt::Vec& GetCameraDir()
{
    return g_camdir;
}

const edupt::Vec& GetCameraUp()
{
    return g_camup;
}

void recGeoTris(MOE::SceneGraph::Node* node, std::vector< std::vector<pulsar::Vector3> >& tribufs, std::vector<edupt::Material>& mats)
{
    if (!node)
        return;
    
    using namespace MOE::SceneGraph;
    NODETYPE nt = node->GetType();
    if (nt == NODETYPE_GROUP || nt == NODETYPE_TRANSFORM)
    {
        if (node->GetName() == "Camera")
        {
            Transform* tr = static_cast<Transform*>(node);
            using namespace MOE::Math;
            matrix cm = tr->GetMatrix();
            vec4 pos = vec4(0,0,0,1);
            pos = cm * pos;
            vec4 dir = vec4(0,0,-1,0);
            dir = cm * dir;
            vec4 up = vec4(0,1,0,0);
            up = cm * up;
            
            g_campos = Vec(pos.x,pos.y,pos.z);
            g_camdir = Vec(dir.x,dir.y,dir.z);
            g_camup  = Vec(up.x, up.y, up.z );
        }
        Group* grp = static_cast<Group*>(node);
        u32 n = grp->GetChildCount();
        for (u32 i = 0; i < n; ++i)
            recGeoTris(grp->GetChild(i), tribufs, mats);
    }
    else if(nt == NODETYPE_GEOMETRY)
    {
        Geometry* geo = static_cast<Geometry*>(node);
        u32 icnt = geo->GetIndexCount();
        Geometry::VertexFormat* vtx = geo->GetVertex();
        u32 idxoffset = geo->GetIndexTypeSize();
        u32 tcnt = icnt / 3;
        
        const MOE::SceneGraph::Material* gm = geo->GetMaterial();
        mats.push_back(edupt::Material());
        edupt::Material& m = mats[mats.size()-1];
        MOE::Math::vec4 col = gm->GetVec4("color");
        m.color_ = Vec(col.x,col.y,col.z);
        printf("%s - (%f,%f,%f)\n", gm->GetName().c_str(), col.x,col.y,col.z);
        if (gm->GetName() == "light")
            m.emission_ = Vec(10,10,10);
        tribufs.push_back(std::vector<pulsar::Vector3>());
        std::vector<pulsar::Vector3>& tri = tribufs[tribufs.size()-1];
        tri.reserve(1024*1024);
        
        if (idxoffset == 2) {
            const Index* idx = reinterpret_cast<Index*>(geo->GetIndex());
            for (u32 i = 0; i < tcnt; ++i)
            {
                pulsar::Vector3 t;
                const u32 i3 = 3*i;
                for (u32 j = 0; j < 3; ++j){
                    tri.push_back(pulsar::Vector3(vtx[idx[i3+j]].pos.x,
                                                  vtx[idx[i3+j]].pos.y,
                                                  vtx[idx[i3+j]].pos.z));
                }
            }
        } else {
            const Index32* idx = reinterpret_cast<Index32*>(geo->GetIndex());
            for (u32 i = 0; i < tcnt; ++i)
            {
                pulsar::Vector3 t;
                const u32 i3 = 3*i;
                for (u32 j = 0; j < 3; ++j){
                    tri.push_back(pulsar::Vector3(vtx[idx[i3+j]].pos.x,
                                                  vtx[idx[i3+j]].pos.y,
                                                  vtx[idx[i3+j]].pos.z));
                }
            }
        }
    }
}
void convertSceneGraph(MOE::SceneGraph::Node* node, std::vector<const pulsar::Triangle*>& tris)
{
    g_tribufs.clear();
    g_ts.clear();
    g_mats.clear();
    recGeoTris(node, g_tribufs, g_mats);
    
    size_t alltrinum = 0;
    for (size_t i = 0; i < g_tribufs.size(); ++i)
        alltrinum += g_tribufs[i].size() / 3;
    g_ts.resize(alltrinum);
    
    printf("tri = %ld\n",alltrinum);
    
    size_t tricnt = 0;
    for (size_t i = 0; i < g_tribufs.size(); ++i)
    {
        const size_t tn = g_tribufs[i].size() / 3;
        for (size_t j = 0; j < tn; ++j)
        {
            g_ts[tricnt].points[0] = &g_tribufs[i][3*j  ];
            g_ts[tricnt].points[1] = &g_tribufs[i][3*j+1];
            g_ts[tricnt].points[2] = &g_tribufs[i][3*j+2];
            g_ts[tricnt].material = static_cast<int>(i);
            ++tricnt;
        }
    }
    
    tris.resize(alltrinum);
    for (size_t i = 0; i < g_ts.size(); ++i)
        tris[i] = &g_ts[i];
}

inline bool sceneInit(s8* modelpath)
{
    FILE* fp = fopen(modelpath,"rb");
    if (!fp)
        return false;
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    unsigned char* modeldata = new unsigned char[size];
    fread(modeldata, size, 1, fp);
    fclose(fp);
    
    MOE::MrzLoader loader;
    MOE::SceneGraph::Node* root = loader.Load(reinterpret_cast<const void*>(modeldata));
    delete [] modeldata;
    if (!root)
        return false;

    convertSceneGraph(root, g_tris);
    
    g_bvh = new pulsar::PLQBVHMeshAccelerator(g_tris);
     
    return true;
}
    
// シーンとの交差判定関数
inline bool intersect_scene(const Ray &ray, Intersection *intersection) {
	intersection->hitpoint_.distance_ = kINF;
	intersection->object_id_  = -1;

    pulsar::Intersection isect;
    pulsar::Ray pray(pulsar::Vector3(ray.org_.x_,ray.org_.y_,ray.org_.z_),
                     pulsar::Vector3(ray.dir_.x_,ray.dir_.y_,ray.dir_.z_));
    if (g_bvh->intersect(&isect, pray, 0.001, kINF))
    {
        intersection->hitpoint_.distance_ = isect.t;
        intersection->hitpoint_.position_.x_ = isect.position[0];
        intersection->hitpoint_.position_.y_ = isect.position[1];
        intersection->hitpoint_.position_.z_ = isect.position[2];
        intersection->hitpoint_.normal_.x_ = isect.normal[0];
        intersection->hitpoint_.normal_.y_ = isect.normal[1];
        intersection->hitpoint_.normal_.z_ = isect.normal[2];
        intersection->object_id_ = isect.material;
        intersection->mat_ = &g_mats[isect.material];
    }
    
    /*const double n = sizeof(spheres) / sizeof(Sphere);
	for (int i = 0; i < int(n); i ++) {
		Hitpoint hitpoint;
		if (spheres[i].intersect(ray, &hitpoint)) {
			if (hitpoint.t < intersection->hitpoint.t) {
				intersection->hitpoint = hitpoint;
				intersection->object_id = i;
			}
		}
	}*/

	return (intersection->object_id_ != -1);
}

};

#endif