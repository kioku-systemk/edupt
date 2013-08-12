#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vec.h"

namespace edupt {
    
    typedef Vec Color;
    
    enum ReflectionType {
        REFLECTION_TYPE_DIFFUSE,	// 完全拡散面。いわゆるLambertian面。
        REFLECTION_TYPE_SPECULAR,	// 理想的な鏡面。
        REFLECTION_TYPE_REFRACTION,	// 理想的なガラス的物質。
    };
    
    const double kIor = 1.5; // 屈折率(Index of refraction)
    
    struct Material
    {
        Material(){emission_ = 0; color_ = 0; reflection_type_ = REFLECTION_TYPE_DIFFUSE; }
        Material(const Color& _em, const Color& _cl, ReflectionType ref)
        {
            emission_ = _em;
            color_ = _cl;
            reflection_type_ = ref;
        }
        Color emission_;
        Color color_;
        ReflectionType reflection_type_;
    };
};

#endif