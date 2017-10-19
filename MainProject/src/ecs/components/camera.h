#pragma once
#include "../component.h"
#include "../../math/matrix.h"
#include "../../graphics/textures/texture_cube.h"

struct Camera : Component
{
private:
	float fieldOfView = 0;
	float near = 0, far = 0;
	float viewportWidth = 0, viewportHeight = 0;
	mutable std::unique_ptr<Matrix4> projection;
public:
	TextureCube* skyBox;

	float& getFieldOfView();
	float& getNear();
	float& getFar();
	float& getViewportWidth();
	float& getViewportHeight();

	const float& getFieldOfView() const;
	const float& getNear() const;
	const float& getFar() const;
	const float& getViewportWidth() const;
	const float& getViewportHeight() const;

	void setFieldOfView(float field_of_view);
	void setNear(float near);
	void setFar(float far);
	void setViewportWidth(float viewport_width);
	void setViewportHeight(float viewport_height);

	__declspec(property(get = getFieldOfView, put = setFieldOfView)) float FieldOfView;
	__declspec(property(get = getNear, put = setNear)) float Near;
	__declspec(property(get = getFar, put = setFar)) float Far;
	__declspec(property(get = getViewportWidth, put = setViewportWidth)) float ViewportWidth;
	__declspec(property(get = getViewportHeight, put = setViewportHeight)) float ViewportHeight;

	const Matrix4& getProjection() const;
};
