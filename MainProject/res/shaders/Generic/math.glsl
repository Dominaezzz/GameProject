#version 400

//  mat4 translate(in vec3 amount)
//  {
//  	return translate(amount.x, amount.y, amount.z);
//  }

mat4 translate(float x, float y, float z)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}

//  mat4 scale(in vec3 amount)
//  {
//  	return scale(amount.x, amount.y, amount.z);
//  }

mat4 scale(float x, float y, float z)
{
    return mat4(
        vec4(x,   0.0, 0.0, 0.0),
        vec4(0.0, y,   0.0, 0.0),
        vec4(0.0, 0.0, z,   0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat4 rotate_x(float theta)
{
    return mat4(
        vec4(1.0,         0.0,         0.0, 0.0),
        vec4(0.0,  cos(theta),  sin(theta), 0.0),
        vec4(0.0, -sin(theta),  cos(theta), 0.0),
        vec4(0.0,         0.0,         0.0, 1.0)
    );
}

mat4 rotate_y(float theta)
{
	return mat4(
		vec4(cos(theta), 0.0, -sin(theta), 0.0),
		vec4(0.0,        1.0,         0.0, 0.0),
		vec4(sin(theta), 0.0,  cos(theta), 0.0),
		vec4(0.0,        0.0,         0.0, 1.0)
	);
}

mat4 rotate_z(float theta)
{
	return mat4(
		vec4( cos(theta),  sin(theta), 0.0, 0.0),
		vec4(-sin(theta),  cos(theta), 0.0, 0.0),
		vec4(        0.0,         0.0, 0.0, 0.0),
		vec4(        0.0,         0.0, 0.0, 1.0)
	);
}
