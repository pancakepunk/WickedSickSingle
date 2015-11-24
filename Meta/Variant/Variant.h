#pragma once


namespace Reflection
{

  class Metadata;


  class LuaUserdata;


  class Var
  {
    public:

      static void RegisterTypeData();
      Metadata* GetMetatype();

      Var(void);
      Var(Metadata* type, 
          void* data);
      Var(const Var& other);
      Var(LuaUserdata* data);
    
      ~Var();

      template<typename T>
      Var(T& data);
      template<typename T>
      Var(T&& data);
      template<typename T>
      Var(const T& data);
      template<>
      Var(LuaUserdata * const& tocopy);

      template<>
      Var(void * const& tocopy);




      template<typename T>
      T& GetValue(void);

      template<typename T>
      void SetValue(const T& value);
      template<typename T>
      void SetValue(T& value);
      template<>
      void SetValue(const char*&value);
      template<>
      void SetValue(void*& value);
      template<>
      void SetValue(std::string& value);

      void SetValueIndirection(void* value, int indirection);

      void SetOwns(bool owns);
      void SetType(Metadata* type);
      void SetData(void* data);

       template <typename Type>
      void Copy(const Type& type);

      void Invalidate();
      void Validate();

      bool IsValid();
      void* GetData();
      Metadata* GetMetadata();
      unsigned short GetIndirection();

      void SetLevelsOfIndirection(unsigned short indirection);

    private:
      bool owns_;
      void* data_;
      Metadata* type_;
      //Language origin;
      bool is_valid_;
      unsigned short levels_of_indirection_;
  };

}