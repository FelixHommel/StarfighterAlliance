#ifndef INTERNAL_SHADER_HPP
#define INTERNAL_SHADER_HPP

#include "glm/ext/matrix_float4x4.hpp"

#include <string>

class Shader
{
	public:
		/** Constructors */
		Shader() = default;
		Shader(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

		/** Public member functions */
		void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
		Shader& use();
		[[nodiscard]] unsigned int getID() const { return m_id; }

		void setFloat(const char* name, float value, bool useShader = false);
		void setInteger(const char* name, int value, bool useShader = false);
		void setVector2f(const char* name, float x, float y, bool useShader = false);
		void setVector2f(const char* name, const glm::vec2& value, bool useShader = false);
		void setVector3f(const char* name, float x, float y, float z, bool useShader = false);
		void setVector3f(const char* name, const glm::vec3& value, bool useShader = false);
		void setVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
		void setVector4f(const char* name, const glm::vec4& value, bool useShader = false);
		void setMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);

	private:
		/** Member variables */
		unsigned int m_id;

		/** Private member functions */
		static void checkCompileErrors(unsigned int object, std::string type);
};

#endif //!INTERNAL_SHADER_HPP
