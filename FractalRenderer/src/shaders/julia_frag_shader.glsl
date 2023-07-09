//#version 460 core
//in vec2 fragCoord;
//out vec4 fragColor;
//
//uniform int maxIterations;
//uniform float escapeRadius;
//uniform vec2 juliaConstant;
//
//vec3 gradientColor(float t)
//{
//    // Define your color gradient here
//    vec3 color1 = vec3(0.0, 7.0f/255.0f, 100.0/255.0);
//    vec3 color2 = vec3(32.0f/25.0f, 107.0f/255.0f, 203.0f/255.0f);
//    vec3 color3 = vec3(237.0f/255.0f, 1.0, 1.0);
//    vec3 color4 = vec3(1.0, 170.0/255.0, 0.0);
//    vec3 color5 = vec3(0.0, 2.0/255.0, 0.0);
//
//    // return a gradient color based on t and the defined colors
//    if (t < 0.2)
//    {
//		return mix(color1, color2, t * 5.0);
//	}
//    else if (t < 0.4)
//    {
//		return mix(color2, color3, (t - 0.2) * 5.0);
//	}
//    else if (t < 0.6)
//    {
//		return mix(color3, color4, (t - 0.4) * 5.0);
//	}
//    else if (t < 0.8)
//    {
//		return mix(color4, color5, (t - 0.6) * 5.0);
//	}
//    else
//    {
//		return mix(color5, color1, (t - 0.8) * 5.0);
//	}
//}
//
//void main()
//{
//    vec2 z = vec2(fragCoord.x, fragCoord.y);
//    int iteration = 0;
//
//    for (int i = 0; i < maxIterations; i++)
//    {
//        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + juliaConstant;
//
//        if (dot(z, z) > escapeRadius * escapeRadius)
//        {
//            float normalizedIteration = float(iteration) / float(maxIterations);
//            fragColor = vec4(gradientColor(normalizedIteration), 1.0);
//            return;
//        }
//
//        iteration++;
//    }
//
//    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
//}


//#version 460 core
//in vec2 fragCoord;
//out vec4 fragColor;
//
//uniform int maxIterations;
//uniform float escapeRadius;
//uniform vec2 juliaConstant;
//
//vec3 gradientColor(float t)
//{
//	// Define your color gradient here
//	vec3 color1 = vec3(0.0, 7.0 / 255.0, 100.0 / 255.0);
//	vec3 color2 = vec3(32.0 / 255.0, 107.0 / 255.0, 203.0 / 255.0);
//	vec3 color3 = vec3(237.0 / 255.0, 1.0, 1.0);
//	vec3 color4 = vec3(1.0, 170.0 / 255.0, 0.0);
//	vec3 color5 = vec3(0.0, 2.0 / 255.0, 0.0);
//
//	// Return a gradient color based on t and the defined colors
//	if (t < 0.2)
//	{
//		return mix(color1, color2, t * 5.0);
//	}
//	else if (t < 0.4)
//	{
//		return mix(color2, color3, (t - 0.2) * 5.0);
//	}
//	else if (t < 0.6)
//	{
//		return mix(color3, color4, (t - 0.4) * 5.0);
//	}
//	else if (t < 0.8)
//	{
//		return mix(color4, color5, (t - 0.6) * 5.0);
//	}
//	else
//	{
//		return mix(color5, color1, (t - 0.8) * 5.0);
//	}
//}
//
//void main()
//{
//	vec2 c = vec2(fragCoord.x, fragCoord.y);
//	vec2 z = c;
//	int iteration = 0;
//
//	for (int i = 0; i < maxIterations; i++)
//	{
//		vec2 zSquare = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
//		z = zSquare + juliaConstant;
//
//		if (dot(z, z) > escapeRadius * escapeRadius)
//		{
//			// Calculate smooth iteration count using logarithmic scale
//			float logIter = float(iteration) + 1.0 - log2(log2(dot(z, z))) / 2.0;
//			float normalizedIteration = logIter / float(maxIterations);
//			fragColor = vec4(gradientColor(normalizedIteration), 1.0);
//			return;
//		}
//
//		iteration++;
//	}
//
//	fragColor = vec4(1.0, 1.0, 1.0, 1.0);
//}

#version 460 core
in vec2 fragCoord;
out vec4 fragColor;

uniform int maxIterations;
uniform float escapeRadius;
uniform vec2 juliaConstant;

vec3 gradientColor(float t)
{
	// Define your color gradient here
	vec3 color1 = vec3(0.0, 7.0 / 255.0, 100.0 / 255.0);
	vec3 color2 = vec3(32.0 / 255.0, 107.0 / 255.0, 203.0 / 255.0);
	vec3 color3 = vec3(237.0 / 255.0, 1.0, 1.0);
	vec3 color4 = vec3(1.0, 170.0 / 255.0, 0.0);
	vec3 color5 = vec3(0.0, 2.0 / 255.0, 0.0);

	// Return a gradient color based on t and the defined colors
	if (t < 0.2)
	{
		return mix(color1, color2, t * 5.0);
	}
	else if (t < 0.4)
	{
		return mix(color2, color3, (t - 0.2) * 5.0);
	}
	else if (t < 0.6)
	{
		return mix(color3, color4, (t - 0.4) * 5.0);
	}
	else if (t < 0.8)
	{
		return mix(color4, color5, (t - 0.6) * 5.0);
	}
	else
	{
		return mix(color5, color1, (t - 0.8) * 5.0);
	}
}

void main()
{
	vec2 c = vec2(fragCoord.x, fragCoord.y);
	vec2 z = c;
	int iteration = 0;

	float smoothIter = 0.0;
	float smoothed = 0.0;
	float logBase = 2.0;

	for (int i = 0; i < maxIterations; i++)
	{
		vec2 zSquare = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y);
		z = zSquare + juliaConstant;

		float magnitude = dot(z, z);

		if (magnitude > escapeRadius * escapeRadius)
		{
			// Calculate smooth iteration count using smoothing
			smoothIter = float(iteration) + 1.0 - log2(log2(magnitude)) / logBase;
			smoothed = float(iteration) + 1.0 - log2(log2(magnitude + 1.0 - log2(log2(magnitude)))) / logBase;
			break;
		}

		iteration++;
	}

	float normalizedIteration = smoothIter / float(maxIterations);
	float smoothedNormalized = smoothed / float(maxIterations);
	float smoothness = smoothedNormalized - normalizedIteration;

	if (smoothness < 0.0)
	{
		smoothness = 0.0;
	}

	fragColor = vec4(gradientColor(normalizedIteration + smoothness), 1.0);
}

