CS300 Readme
Mark Lauzon
Assignment 1

building the project:
- anttweakbar.dll is required to build and run, make sure its in the working directory.

special instructions:
- change the object id to change the selected object.
  properties appear in the other bar.

- change the drawtype to toggle normal drawing (0 = default, 2 = vertnormals, 3 = facenormals)

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
- you may only set the position of a light via the transform (the position in the lightcomponent clones it, but not vice versa)
- when drawtype = 1, it draws the tris as lines (not as wireframe)
- oversaturation of light is not accounted for
- dramatic frame rate decrease when drawing normals
- creating extra objects untested and buggy

notes:
- graphics implementation spans graphics.cpp, shader.cpp, dxshader.cpp, model.cpp, dxmodel.cpp, buffer.cpp, dxbuffer.cpp, directx.cpp, graphicsapi.cpp
- relevant hlsl code is in content/shaders directory