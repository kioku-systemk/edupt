/*
 * SceneGraph/Material.h
 *
 * SceneGraph Library
 * Created by kioku on 11/09/15.
 *
 */

#ifndef INCLUDE_SCENEGRAPH_MATERIAL_H
#define INCLUDE_SCENEGRAPH_MATERIAL_H

#include "Type.h"
#include "Node.h"
#include <map>
#include <string>
#include "Math.h"
#include "Log.h"

namespace MOE
{
	namespace SceneGraph
	{
			
		class Material : public Node
		{
		public:
			enum DRAWMODE
			{
				MODE_POINTS         = 0,// MG_POINTS
				MODE_LINES          = 1,// MG_LINES
				MODE_LINE_LOOP      = 2,// MG_LINE_LOOP
				MODE_LINE_STRIP     = 3,// MG_LINE_STRIP
				MODE_TRIANGLES      = 4,// MG_TRIANGLES
				MODE_TRIANGLE_STRIP = 5,// MG_TRIANGLE_STRIP
				MODE_TRIANGLE_FAN   = 6// MG_TRIANGLE_FAN
			};
			Material()  : Node(NODETYPE_MATERIAL), m_drawmode(MODE_TRIANGLES) {}
			~Material(){}
			
			void Add(const std::string& name, const Math::matrix4x4& mat)
			{
				MOELogW("SceneGraph::Material::Add()method is depricated. You have to use Set() method.");
				m_mat[name] = mat;
			}
			void Add(const std::string& name, const Math::vec4& vec4)
			{
				MOELogW("SceneGraph::Material::Add() method is depricated. You have to use Set() method.");
				m_vec4[name] = vec4;
			}
			
			void Add(const std::string& name, const std::string& texname)
			{
				MOELogW("SceneGraph::Material::Add() method is depricated. You have to use Set() method.");
				m_tex[name] = texname;
			}

			void Set(const std::string& name, const Math::matrix4x4& mat)
			{
				m_mat[name] = mat;
			}
			void Set(const std::string& name, const Math::vec4& vec4)
			{
				m_vec4[name] = vec4;
			}
			
			void Set(const std::string& name, const std::string& texname)
			{
				m_tex[name] = texname;
			}

			void SetDrawMode(DRAWMODE mode)
			{
				m_drawmode = mode;
			}
			Math::matrix4x4 GetMatrix(const std::string& name) const
			{
				static const 
				std::map<std::string, Math::matrix4x4>::const_iterator it = m_mat.find(name);
				if (it != m_mat.end() )
					return it->second;
				else
					return Math::Identity();
			}
			
			Math::vec4 GetVec4(const std::string& name) const
			{
				std::map<std::string, Math::vec4>::const_iterator it = m_vec4.find(name);
				if (it != m_vec4.end() )
					return it->second;
				else
					return Math::vec4(0.0f,0.0f,0.0f,0.0f);
			}
			
			std::string GetTexture(const std::string& name) const
			{
				std::map<std::string, std::string>::const_iterator it = m_tex.find(name);
				if (it != m_tex.end() )
					return it->second;
				else
					return std::string();
			}
			
			b8 GetMatrix(s32 i, std::string& name, Math::matrix4x4& mat) const
			{
				if (i >= static_cast<s32>(m_mat.size()))
					return false;
				
				std::map<std::string, Math::matrix4x4>::const_iterator it = m_mat.begin();
				for (s32 t = 0; t < i; ++t)
					++it;
				
				name = it->first;
				mat  = it->second;
				return true;
			}

			b8 GetVec4(s32 i, std::string& name, Math::vec4& vec) const
			{
				if (i >= static_cast<s32>(m_vec4.size()))
					return false;
				
				std::map<std::string, Math::vec4>::const_iterator it = m_vec4.begin();
				for (s32 t = 0; t < i; ++t)
					++it;
				
				name = it->first;
				vec  = it->second;
				return true;
			}

			b8 GetTexture(s32 i, std::string& name, std::string& tex) const
			{
				if (i >= static_cast<s32>(m_tex.size()))
					return false;
				
				std::map<std::string, std::string>::const_iterator it = m_tex.begin();
				for (s32 t = 0; t < i; ++t)
					++it;
				
				name = it->first;
				tex  = it->second;
				return true;
			}

			s32 GetNumMatrix() const
			{
				return static_cast<s32>(m_mat.size());
			}
			s32 GetNumVec4() const
			{
				return static_cast<s32>(m_vec4.size());
			}

			s32 GetNumTexture() const
			{
				return static_cast<s32>(m_tex.size());
			}

			DRAWMODE GetDrawMode() const
			{
				return m_drawmode;
			}
			
		private:
			// TODO: std::string to hash.
            std::map<std::string, Math::matrix4x4> m_mat;
            std::map<std::string, Math::vec4>      m_vec4;
            std::map<std::string, std::string>     m_tex;
			DRAWMODE m_drawmode;
		};
		
	} // namespace SceneGraph
		
} // namespace MOE

#endif // INCLUDE_SCENEGRAPH_MATERIAL_H
