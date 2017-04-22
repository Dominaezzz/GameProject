//
// Created by Dominic on 17/12/2016.
//


#include <GLFW/glfw3.h>

class VideoMode {
private:
	GLFWvidmode vidmode;
public:
	VideoMode();
	VideoMode(int width, int height);
	VideoMode(int width, int height, int refreshrate);
	VideoMode(int width, int height, int redbits, int greenbits, int bluebits);
	VideoMode(int width, int height, int redbits, int greenbits, int bluebits, int refreshrate);
	VideoMode(GLFWvidmode vidmode);

	/*!\brief Gets the width of the videomode
	 * \returns the width of the videomode
	 */
	int getWidth() const;

	/*!\brief Sets the width of the videomode
	 * \param[in] width the width of the videomode
	 */
	void setWidth(int width);

	/*!\brief Gets the height of the videomode
	 * \returns the height of the videomode
	 */
	int getHeight() const;

	/*!\brief Sets the height of the videomode
	 * \param[in] height the height of the videomode
	 */
	void setHeight(int height);

	/*!\brief Gets the bit depth for red
	 * \returns the red bit depth
	 */
	int getRedBits() const;

	/*!\brief Sets the bit depth for red
	 * \param[in] redbits The new red bit depth
	 */
	void setRedBits(int redbits);

	/*!\brief Gets the bit depth for green
	 * \returns the green bit depth
	 */
	int getGreenBits() const;

	/*!\brief Sets the bit depth for green
	 * \param[in] greenbits The new green bit depth
	 */
	void setGreenBits(int greenbits);

	/*!\brief Gets the bit depth for blue
	 * \returns the blue bit depth
	 */
	int getBlueBits() const;

	/*!\brief Sets the bit depth for blue
	 * \param[in] bluebits The new blue bit depth
	 */
	void setBlueBits(int bluebits);

	/*!\brief Gets the refresh rate for the monitor
	 * \returns the monitor's refresh rate
	 */
	int getRefreshRate() const;

	/*!\brief Sets the refresh rate for the monitor
	 * \param[in] refreshrate the monitor's new refresh rate
	 */
	void setRefreshRate(int refreshrate);

	/*!\brief Gets the C GLFW pointer for the monitor
	 * \returns the monitor's C GLFW pointer
	 */
//	operator GLFWvidmode*() const;
};
