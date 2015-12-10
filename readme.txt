CS300 Readme
Mark Lauzon
Assignment 3

building the project:
- anttweakbar.dll is required to build and run, make sure its in the working directory.
- must be built on vs 2015 or higher will NOT build on prior versions of visual studio.
- requires a dx11 compatible device
- must be built in with x86 configuration (vs 2015 defaults to x64)

special instructions:
- change the object id to change the selected object.
  properties appear in the other tweak bar.
  0 - camera
  1 - plane
  2 - object
  3 - skybox
  4/12 - lights

- enums relevant to project demo
  demo type enum
  0 no demo
  1 normal mapping
  2 texture mapping

- change the shader field to toggle between shaders
  A complete list of shaders may be found inside Content/Shaders
  relevant shaders include
  "reflection"
  "pixelblinn"

- change the model field to see with other models
  "bettersphere"
  "sphere"
  "smoothbox"
  "box"

- change the lighttype to change between directional, spot, and point lights( 0 = spot, 1 = point, 2 = dir )

controls:
- right click + drag to rotate camera
- ctrl + right click + drag to move camera
- scroll wheel / + or - to zoom

known bugs:
- when drawtype = 1, it draws the tris as lines (not as wireframe)
- creating extra objects untested and buggy

notes:
- graphics implementation spans everything in the Graphics folder
- relevant hlsl code is in content/shaders directory