#version 410 core

precision highp float;

in vec4 gl_FragCoord;

uniform vec2 uWindowSize;
uniform vec2 uCenter;
uniform float uZoom;

#define MAX_ITERATION 250
#define RADIUS 1000

int maxIteration = int(MAX_ITERATION * sqrt(log(1.0+uZoom)));

// (x+yi)^2 = x^2 - y^2 + 2xyi
vec2 squareComplex(vec2 number)
{
    return vec2(
        pow(number.x, 2) - pow(number.y, 2), 2*number.x*number.y
    );
}

float squareModulus(vec2 number)
{
    return number.x*number.x + number.y*number.y;
}

// z = z^2 = c; z_0 = 0
float iterateSimple(vec2 coord)
{
    vec2 z = vec2(0,0);
    for (int i = 0; i < maxIteration; ++i) {
        z = squareComplex(z) + coord;
        if (length(z) > RADIUS)
            return float(i)/maxIteration;
    }
    return 1.0;
}

float getColor(float value, int iteration, float multiplier)
{
    if (iteration == maxIteration)
        return 0.0;

    // float R = value;
    // float V = max(0.0, 2*log(R)/pow(2, iteration));
    // float x = log(V)/log(maxIteration/10);
    // float x = iteration;
    float x = log(max(0.0, 2*log(value)/pow(2, iteration)))/log(maxIteration/10);

    #define SINUSODIAL_Y_OFFSET 0.2
    return (1 + SINUSODIAL_Y_OFFSET/2 - (1-SINUSODIAL_Y_OFFSET) * cos(multiplier*x))/2;
}

vec3 colorize(float square, int iteration)
{
    return vec3(
        getColor(square, iteration, 1/(7.0*pow(3.0, 0.25))),
        getColor(square, iteration, 1/(3.0*sqrt(2.0))),
        getColor(square, iteration, 1/(2.0*log(5.0)))
    );
}

vec3 iteratePretty(vec2 coord)
{
    #define STARTING_POINT 0.0
    #define EPSILON 0.0000001

    vec2 c = coord;
    vec2 z = STARTING_POINT + c;
    vec2 z_der = vec2(1.0);

    int i = 0;
    float z_square = 0;
    for (; i < maxIteration; ++i)
    {
        z_square = squareModulus(z);
        if (z_square > RADIUS*RADIUS)
            return colorize(z_square, i);

        z_der = 2.0*z_der*z;
        //if (length(z_der) < EPSILON)
        //    return colorize(0.0, maxIteration);

        z = squareComplex(z) + c;
    }
    return colorize(z_square, i);
}

vec2 getPixelCoordinate()
{
    // from -2 to 2 of complex coordinate
    #define START -2.0
    #define END 2.0

    float aspectRatio = uWindowSize.x/uWindowSize.y;
    vec2 range = vec2(
        (END - START),
        (END - START)/aspectRatio
    );

    vec2 normalizedFragCoord = gl_FragCoord.xy/uWindowSize - 0.5;     // [-0.5, 0.5]
    normalizedFragCoord *= 2;                                               // [-1, 1]

    vec2 location = range/uZoom * normalizedFragCoord + uCenter;
    return location;
}

void main()
{
    vec2 coordinate = getPixelCoordinate();
    // vec3 color = vec3(iterateSimple(coordinate);
    vec3 color = iteratePretty(coordinate);
    gl_FragColor = vec4(color, 1.0);
}
