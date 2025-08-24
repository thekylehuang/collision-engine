#version 330 core

out vec4 FragColor;

uniform vec2 iResolution;

void main()
{
  // Shadertoy type boilerplate
  vec2 fragCoord = gl_FragCoord.xy;
  vec2 uv = fragCoord / iResolution.xy;

  // Aspect correction
  uv.x *= iResolution.x / iResolution.y;

  // Main Grid
  float gridSize = 87.0;
  float gridStroke = 2.0;

  vec2 localPos = mod(fragCoord.xy, gridSize);
  float line = step(localPos.x, gridStroke) + step(localPos.y, gridStroke);

  // Sub Grid
  float subGridSize = gridSize / 2.0;
  float subGridStroke = 1.0;
  
  vec2 subLocalPos = mod(localPos, subGridSize);
  float subLine = step(subLocalPos.x, subGridStroke) + step(subLocalPos.y, subGridStroke);

  // Coloring
  vec3 bgColor = vec3(0.063,0.071,0.078);
  vec3 gridColor = vec3(0.133,0.141,0.149);
  vec3 subGridColor = vec3(0.192,0.2,0.212);

  vec4 col = vec4(bgColor, 1.0);
  if (line > 0.0) {
    col = vec4(gridColor, 1.0);
  }
  else if (subLine > 0.0) {
    col = vec4(subGridColor, 1.0);
  }

  FragColor = vec4(col);
}