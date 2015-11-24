CS300 Readme
Mark Lauzon
Assignment 2

building the project:
- anttweakbar.dll is required to build and run, make sure its in the working directory.
- must be built on vs 2015 or higher will NOT build on prior versions of visual studio

special instructions:
- change the object id to change the selected object.
  properties appear in the other bar.

- enums relevant to project demo
  demo type enum
  0 no demo
  1 normal mapping
  2 texture mapping

  entity type enum
  0 centroid
  1 normal
  2 position
  3 reflection

  mapping type enum
  0 planar
  1 cylindrical
  2 spherical
  3 cubic

- change the shader field to toggle between shaders
  "pixelblinn"
  "pixelphong"
  "vertexblinn"
  "vertexphong"

- change the lighttype to change between directional, spot, and point lights( 0 = spot, 1 = point, 2 = dir )

controls:
- right click + drag to rotate camera
- ctrl + right click + drag to move camera
- scroll wheel / + or - to zoom

known bugs:
- when drawtype = 1, it draws the tris as lines (not as wireframe)
- oversaturation of light is not accounted for
- dramatic frame rate decrease when drawing normals
- creating extra objects untested and buggy

notes:
- graphics implementation spans graphics.cpp, shader.cpp, dxshader.cpp, model.cpp, dxmodel.cpp, buffer.cpp, dxbuffer.cpp, directx.cpp, graphicsapi.cpp
- relevant hlsl code is in content/shaders directory