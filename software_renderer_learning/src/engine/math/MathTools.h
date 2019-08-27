#pragma once

float degreesToRadians(float angle);
float radiansToDegrees(float angle);

/* value = (1 - gradient) * start + gradient * end
or 
value = start + gradient * (end - start)

0->start
1->end */
float getGradient2(float value, float start, float end);
float linearInterpolation2(float start, float end, float gradient);