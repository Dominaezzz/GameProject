//
// Created by Dominic on 17/12/2016.
//

#include <GLFW/glfw3.h>
#include <vector>
#include <functional>

#include "video_mode.h"
#include "gamma_ramp.h"

/*! \brief Monitor events (Connected/Disconnected)
	 */
enum MonitorConfigChangeEvents
{
	Connected = GLFW_CONNECTED,
	Disconnected = GLFW_DISCONNECTED
};

struct MonitorSize {
	int width, height;
};
struct MonitorPos {
	int x, y;
};

/*! \brief A class representing a computer monitor
 */
class Monitor
{
	GLFWmonitor* monitor;
public:
	Monitor(GLFWmonitor* monitor);

	/*!\brief Gets a list of monitors currently plugged into the machine.
	 * \returns a list of monitors currently plugged into the machine
	 */
	static const std::vector<Monitor> getMonitors();

	/*!\brief Gets the primary monitor
	 * \returns the primary monitor
	 */
	static const Monitor getPrimaryMonitor();

	/*!\brief Gets the position of the monitor
	 * \returns the position of the monitor
	 */
	const MonitorPos getPos() const;

	/*!\brief Gets the physical size of the monitor
	 * \returns the physical size of the monitor
	 */
	const MonitorSize getPhysicalSize() const;

	/*!\brief Gets the name of the monitor
	 * \returns the name of the monitor
	 */
	const std::string getName() const;

	/*!\brief Gets the list of videomodes supported by the monitor
	 * \returns the list of videomodes supported by the monitor
	 */
	const std::vector<VideoMode> getVideoModes() const;

	/*!\brief Gets the monitor's current videomode
	 * \returns the monitor's current videomode
	 */
	const VideoMode getVideoMode() const;

	/*!\brief Sets the gamma of the monitor
	 * \param[in] gamma the monitor's new gamma.
	 */
	void setGamma(float gamma);

	/*!\brief Gets the gamma ramp of the monitor.
	 * \returns the monitor's gamma ramp.
	 */
	const GammaRamp getGammaRamp() const;

	/*!\brief Sets the gamma ramp of the monitor.
	 * \param[in] ramp the monitor's new gamma ramp.
	 */
	void setGammaRamp(GammaRamp &ramp);

	/*!\brief Gets the C GLFW pointer for this Monitor.
	 * \returns the C GLFW pointer for this Monitor.
	 */
	operator GLFWmonitor*() const;
};
