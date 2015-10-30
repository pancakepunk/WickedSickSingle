#pragma once

namespace WickedSick
{
  namespace ShaderType
  {
    enum Enum
    {
      Vertex,
      Pixel,
      Compute,
      Tesselation,
      Geometry,
      Count
    };
  }

  namespace LightType
  {
    enum Enum
    {
      SpotLight,
      PointLight,
      DirectionalLight,
      Count
    };
  }

  namespace AccessType
  {
    enum Enum
    {
      None,
      CpuWrite,
      CpuRead,
      CpuBoth,
      Count
    };
  }

  namespace UsageType
  {
    enum Enum
    {
      Default,//read/write via gpu
      Dynamic,//read access via gpu, write via cpu
      Static,//read-only, no cpu access
      Staging,//can copy from gpu to cpu
      Count
    };
  }

  namespace BufferType
  {
    enum Enum
    {
      Vertex,
      Index,
      Constant,
      Count
    };
  }

  enum DrawMethod
  {
    PointList,
    LineList,
    TriangleList,
    TriangleStrip,

    Count
  };

  namespace APIType
  {
    enum Enum
    {
      DirectX,
      OpenGLAPI,
      Count
    };
  }
  namespace DrawType
  {
    enum Enum
    {
      Default,
      Wireframe,
      VertexNormals,
      FaceNormals,
      Count
    };
  }
}
