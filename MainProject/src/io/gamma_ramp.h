//
// Created by Dominic on 17/12/2016.
//


#include <GLFW/glfw3.h>

class GammaRamp {
private:
	GLFWgammaramp gammaramp;

public:

	GammaRamp(const GLFWgammaramp& gammaramp);
	GammaRamp();

	/*! \brief Returns an array of values describing the response of the red channel.
	 * \return An array of values describing the response of the red channel.
	 */
	unsigned short* getRed() const;
	/*! \brief Sets the array of values describing the response of the red channel.
	 * \param[in] red An array of values describing the response of the red channel.
	 */
	void setRed(unsigned short* red);

	/*! \brief Returns an array of values describing the response of the green channel.
	 * \return An array of values describing the response of the green channel.
	 */
	unsigned short* getGreen() const;
	/*! \brief Sets the array of values describing the response of the green channel.
	 * \param[in] green An array of values describing the response of the green channel.
	 */
	void setGreen(unsigned short* green);

	/*! \brief Returns an array of values describing the response of the blue channel.
	 * \return An array of values describing the response of the blue channel.
	 */
	unsigned short* getBlue() const;
	/*! \brief Sets the array of values describing the response of the blue channel.
	 * \param[in] blue An array of values describing the response of the blue channel.
	 */
	void setBlue(unsigned short* blue);

	/*! \brief Returns the size of the ramp
	 * \return The size of the ramp
	 */
	unsigned int getSize() const;
	/*! \brief Sets the size of the ramp
	 * \param[in] size The size of the ramp
	 */
	void setSize(unsigned int size);

	/*! \brief Returns the C GLFW pointer to the data
	 * \return the C GLFW pointer to the data
	 */
	operator GLFWgammaramp*();
};
