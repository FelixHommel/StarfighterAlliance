#ifndef INTERNAL_TEXTURE_HPP
#define INTERNAL_TEXTURE_HPP

/*
 * @file Texture.hpp
 * @brief Abstracts OpenGL Textures
 *
 * @details Holds all the information of a Texture and registers it within OpenGL
 * 
 * @author Felix Hommel
 * @date Nov 17, 2024
 */
class Texture2D
{
public:
    /** !! Constructor !! */
    /** ATTENTION: do not use the flag expect for unit testing */
    Texture2D(bool flag = false);
	/** Class cannot have a destructor, will be called accidentally --> no texture rendering */

    /** Public member functions */
    void generate(int width, int height, unsigned char* data);
	void bind() const;

	[[nodiscard]] const unsigned int* getID() const { return &m_id; }
    [[nodiscard]] int getWidth() const { return m_width; }
    [[nodiscard]] int getHeight() const { return m_height; }

	void setRGBA();

private:
    /** Member variables */
	unsigned int m_id;
	int m_width;
	int m_height;
	int m_internalFormat;
	int m_imageFormat;
	int m_wrapS;
	int m_wrapT;
	int m_filterMin;
	int m_filterMax;
};

#endif //!INTERNAL_TEXTURE_HPP
