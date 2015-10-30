# pragma once


namespace Reflection
{
  class Var;
}


#define STRINGIFY(x) #x

#define CONCATENATE(x, y) x##y


#define TypeFromNull(Type) ((Type*)(nullptr))
#define MemberOffset(var, type) (unsigned)(&TypeFromNull( type )->var)

#define RegisterVirtual\
  public: virtual const Reflection::Metadata* GetMetadata() const \
{ return Reflection::MetaSingleton<decltype(*this)>::GetMetadata(); }



//put in cpp for reflection of a type in the header, pass type name
// RegisterType
// {
//   ...
// }

//#define RegisterEnum(Namespace, yourenum, lastelement)                                              \
//  Reflection::Metadata Reflection::MetaSingleton<Namespace::yourenum>::metadata_( STRINGIFY(yourenum),                      \
//                                                          STRINGIFY(Namespace),                     \
//                                                          sizeof(Namespace::yourenum),              \
//                                                          ByValue,                                  \
//                                                          true);                                    \
//  Reflection::MetaSingleton<Namespace::yourenum> Namespace##yourenum##MetaSingleton;                            \
//  template <>                                                                                       \
//  void Reflection::MetaSingleton<Namespace::yourenum>::RegisterTypeData(void)                                   \
//  {                                                                                                 \
//    Reflection::MetaSingleton<Namespace::yourenum>::Get()->GetEnumConstants().resize(Namespace::lastelement+1); \
//    typedef Namespace::yourenum __this;
//
//#define DEFINE_SCOPED_ENUM(Namespace, yourclass, yourenum, lastelement)                                                   \
//  Reflection::Metadata Reflection::MetaSingleton<Namespace::yourclass::yourenum>::metadata_(STRINGIFY(yourclass##yourenum),                       \
//                                                                    STRINGIFY(Namespace),                                 \
//                                                                    sizeof(Namespace::yourclass::yourenum),               \
//                                                                    ByValue,                                              \
//                                                                    true);                                                \
//  Reflection::MetaSingleton<Namespace::yourclass::yourenum> Namespace##yourclass##yourenum##MetaSingleton;                            \
//  template <> \
//  void Reflection::MetaSingleton<Namespace::yourclass::yourenum>::RegisterTypeData(void)                                                   \
//  {                                                                                                                       \
//    Reflection::MetaSingleton<Namespace::yourclass::yourenum>::Get()->GetEnumConstants().resize(Namespace::yourclass::lastelement+1); \
//    typedef Namespace::yourclass::yourenum __this;
//
#define MetaDef static void RegisterTypeData(void); \
                Reflection::Metadata* GetMetatype(void); 


#define MetaDefVirtual static void RegisterTypeData(void); \
                               virtual Reflection::Metadata* GetMetatype(void); 


//#define DEFINE_TEMPLATED_EXTERNAL(Namespace, metatype, internalType) Metadata MetaSingleton<Namespace::metatype<internalType> >::metadata_(#metatype, #Namespace, sizeof(Namespace::metatype<internalType>), false); \
//  MetaSingleton<Namespace::metatype<internalType> > Namespace##metatype##MetaSingleton; \
//  template <> void MetaSingleton<Namespace::metatype<internalType> >::RegisterTypeData(void)

#define RegisterExternalType(Namespace, metatype)                                          \
  Reflection::Metadata Reflection::MetaSingleton<Namespace::metatype>::metadata_(#metatype,                   \
                                                         #Namespace,                  \
                                                         sizeof(Namespace::metatype), \
                                                         ByReference,                 \
                                                         false);                      \
  Reflection::MetaSingleton<Namespace::metatype> Namespace##metatype##MetaSingleton;              \
  template <> void Reflection::MetaSingleton<Namespace::metatype>::RegisterTypeData(void)         \
  {                                                                                   \
    typedef Namespace::metatype __this;

#define DEFINE_PRIMITIVE(metatype) Reflection::Metadata Reflection::MetaSingleton<metatype>::metadata_(#metatype, sizeof(metatype), ByValue, true);   \
  Reflection::MetaSingleton<metatype> metatype##MetaSingleton; \
  template <> void Reflection::MetaSingleton<metatype>::RegisterTypeData(void){}

#define FindType(metatype) (Reflection::MetaSingleton<metatype>::Get())

#define DEFINE_CONVERSIONS void Reflection::MetaManager::DefineConversions()

#define META(object) Reflection::MetaSingleton<decltype(object)>::Get()

#define CONVERTS(from, to) FindType(from)->AddConversion(FindType(to)); FindType(to)->AddConversion(FindType(from));

#define FIND_TYPE(type) (Reflection::MetaSingleton<typename SimplifyType(type)>::Get())

//used to define members in a class,
//
// RegisterType
// {
//   RegisterMember(member, baseType)
// }

#define RegisterEnumMember(enumeration) \
  Reflection::MetaSingleton<__this>::Get()->GetEnumConstants()[__this::enumeration] = STRINGIFY(enumeration)

#define REGISTER_GLOBAL_FUNCTION(functionName, function) Reflection::MetaSingleton<VoidType>::Get()->AddMemberFunction(Reflection::Function(function), functionName);

#define RegisterMethod(functionName, function) \
  Reflection::MetaSingleton< __this >::Get()->AddMethod(new Reflection::Function(function), functionName)

#define DECLARE_DERIVED(Namespace, className, baseType)                                                                   \
  Reflection::MetaSingleton<Namespace::baseType>::Derived<Namespace::className> Namespace##className##baseType;                       \
  Reflection::Metadata* Reflection::MetaSingleton<Namespace::baseType>::Derived<Namespace::className>::metadata_ = FindType(Namespace::className)

#define REGISTER_DERIVED(classname) FindType(__this)->AddDerived(Reflection::MetaSingleton<__this>::Derived<classname>::Get())

#define RegisterMember(member) Reflection::MetaSingleton< __this >::Get()->AddMember(Reflection::Member(#member, \
                                                                                FIND_TYPE(__this), \
                                                                                FIND_TYPE(decltype(__this::member)), \
                                                                                MemberOffset(__this::member, __this), \
                                                                                Reflection::IndirectionCount<decltype(__this::member)>::value))


#define EXPAND(X) X

#define F(x, ...) X = x and VA_ARGS = __VA_ARGS__
#define G(...) EXPAND(F(__VA_ARGS__))

#define PP_NARG(...) EXPAND(PP_NARG_(__VA_ARGS__, PP_RSEQ_N()))
#define PP_NARG_(...) EXPAND(PP_ARG_N(__VA_ARGS__))
#define PP_ARG_N(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, N, ...) N
#define PP_RSEQ_N() 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define DEFINE_TEMPLATED_EXTERNAL_2( Namespace, name, type0)                                    Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0>                                     >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0>                                    ), ByReference, false); Reflection::MetaSingleton<Namespace::name<type0>                                    > Namespace##name##Metasingleton##type0;                                    template <> void Reflection::MetaSingleton<Namespace::name<type0>                                     >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_3( Namespace, name, type0, type1)                             Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1>                              >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1>                             ), ByReference, false); Reflection::MetaSingleton<Namespace::name<type0, type1>                             > Namespace##name##Metasingleton##type0##type1;                             template <> void Reflection::MetaSingleton<Namespace::name<type0, type1>                              >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_4( Namespace, name, type0, type1, type2)                      Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2>                       >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2>                      ), ByReference, false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2>                      > Namespace##name##Metasingleton##type0##type1##type2;                      template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2>                       >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_5( Namespace, name, type0, type1, type2, type3)               Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3>                >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2, type3>               ), ByReference, false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3>               > Namespace##name##Metasingleton##type0##type1##type2##type3;               template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3>                >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_6( Namespace, name, type0, type1, type2, type3, type4)        Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4>         >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2, type3, type4>        ), ByReference, false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4>        > Namespace##name##Metasingleton##type0##type1##type2##type3##type4;        template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4>         >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_7( Namespace, name, type0, type1, type2, type3, type4, type5) Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4, type5>  >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2, type3, type4, type5> ), ByReference, false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4, type5> > Namespace##name##Metasingleton##type0##type1##type2##type3##type4##type5; template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4, type5>  >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_(N) DEFINE_TEMPLATED_EXTERNAL_##N
#define DEFINE_TEMPLATED_EXTERNAL_EVAL(N) DEFINE_TEMPLATED_EXTERNAL_(N)
#define DEFINE_TEMPLATED_EXTERNAL(...) EXPAND(DEFINE_TEMPLATED_EXTERNAL_EVAL(EXPAND(PP_NARG(__VA_ARGS__)))(__VA_ARGS__))


#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_3(  Namespace, name, namespace0, type0)                                                                                                 Reflection::Metadata Reflection::MetaSingleton<Namespace::name<namespace0::type0>                                                                                                 >::metadata_(STRINGIFY(CONCATENATE(name, type0)),                                    #Namespace, sizeof(Namespace::name<namespace0::type0>                                                                                                ), ByReference, false); Reflection::MetaSingleton<Namespace::name<namespace0::type0>                                                                                                > Namespace##name##type0##Metasingleton;                                    template <> void Reflection::MetaSingleton<Namespace::name<namespace0::type0>                                                                                                >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_5(  Namespace, name, namespace0, type0, namespace1, type1)                                                                              Reflection::Metadata Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1>                                                                              >::metadata_(STRINGIFY(CONCATENATE(name, type0##type1)),                             #Namespace, sizeof(Namespace::name<namespace0::type0, namespace1::type1>                                                                             ), ByReference, false); Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1>                                                                             > Namespace##name##type0##type1##Metasingleton;                             template <> void Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1>                                                                             >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_7(  Namespace, name, namespace0, type0, namespace1, type1, namespace2, type2)                                                           Reflection::Metadata Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2>                                                           >::metadata_(STRINGIFY(CONCATENATE(name, type0##type1##type2)),                      #Namespace, sizeof(Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2>                                                          ), ByReference, false); Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2>                                                          > Namespace##name##type0##type1##type2##Metasingleton;                      template <> void Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2>                                                          >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_9(  Namespace, name, namespace0, type0, namespace1, type1, namespace2, type2, namespace3, type3)                                        Reflection::Metadata Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3>                                        >::metadata_(STRINGIFY(CONCATENATE(name, type0##type1##type2##type3)),               #Namespace, sizeof(Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3>                                       ), ByReference, false); Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3>                                       > Namespace##name##type0##type1##type2##type3##Metasingleton;               template <> void Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3>                                       >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_11( Namespace, name, namespace0, type0, namespace1, type1, namespace2, type2, namespace3, type3, namespace4, type4)                     Reflection::Metadata Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4>                     >::metadata_(STRINGIFY(CONCATENATE(name, type0##type1##type2##type3##type4)),        #Namespace, sizeof(Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4>                    ), ByReference, false); Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4>                    > Namespace##name##type0##type1##type2##type3##type4##Metasingleton;        template <> void Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4>                    >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_13( Namespace, name, namespace0, type0, namespace1, type1, namespace2, type2, namespace3, type3, namespace4, type4, namespace5, type5)  Reflection::Metadata Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4, namespace5::type5>  >::metadata_(STRINGIFY(CONCATENATE(name, type0##type1##type2##type3##type4##type5)), #Namespace, sizeof(Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4, namespace5::type5> ), ByReference, false); Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4, namespace5::type5> > Namespace##name##type0##type1##type2##type3##type4##type5##Metasingleton; template <> void Reflection::MetaSingleton<Namespace::name<namespace0::type0, namespace1::type1, namespace2::type2, namespace3::type3, namespace4::type4, namespace5::type5> >::RegisterTypeData(void)
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_(N) DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_##N
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_EVAL(N) DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_(N)
#define DEFINE_TEMPLATED_EXTERNAL_NAMESPACES(...) EXPAND(DEFINE_TEMPLATED_EXTERNAL_NAMESPACES_EVAL(EXPAND(PP_NARG(__VA_ARGS__)))(__VA_ARGS__))

#define DEFINE_STL_CONTAINER_2( Namespace, name, type0)                                    Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0>                                     >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0>                                    ), false); Reflection::MetaSingleton<Namespace::name<type0>                                    > Namespace##name##Metasingleton; template <> void Reflection::MetaSingleton<Namespace::name<type0>                                     >::RegisterTypeData(void)
#define DEFINE_STL_CONTAINER_3( Namespace, name, type0, type1)                             Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1>                              >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1>                             ), false); Reflection::MetaSingleton<Namespace::name<type0, type1>                             > Namespace##name##Metasingleton; template <> void Reflection::MetaSingleton<Namespace::name<type0, type1>                              >::RegisterTypeData(void)
#define DEFINE_STL_CONTAINER_4( Namespace, name, type0, type1, type2)                      Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2>                       >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2>                      ), false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2>                      > Namespace##name##Metasingleton; template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2>                       >::RegisterTypeData(void)
#define DEFINE_STL_CONTAINER_5( Namespace, name, type0, type1, type2, type3)               Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3>                >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2, type3>               ), false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3>               > Namespace##name##Metasingleton; template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3>                >::RegisterTypeData(void)
#define DEFINE_STL_CONTAINER_6( Namespace, name, type0, type1, type2, type3, type4)        Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4>         >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2, type3, type4>        ), false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4>        > Namespace##name##Metasingleton; template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4>         >::RegisterTypeData(void)
#define DEFINE_STL_CONTAINER_7( Namespace, name, type0, type1, type2, type3, type4, type5) Reflection::Metadata Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4, type5>  >::metadata_(#name, #Namespace, sizeof(Namespace::name<type0, type1, type2, type3, type4, type5> ), false); Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4, type5> > Namespace##name##Metasingleton; template <> void Reflection::MetaSingleton<Namespace::name<type0, type1, type2, type3, type4, type5>  >::RegisterTypeData(void)
#define DEFINE_STL_CONTAINER_(N) DEFINE_STL_CONTAINER_##N
#define DEFINE_STL_CONTAINER_EVAL(N) DEFINE_STL_CONTAINER_(N)
#define DEFINE_STL_CONTAINER(...) EXPAND(DEFINE_STL_CONTAINER_EVAL(EXPAND(PP_NARG(__VA_ARGS__)))(__VA_ARGS__))


#define REGISTER_CONSTRUCTOR_0( name )                                                                                                                                                            {__this* c = (__this*)alloca(sizeof(__this));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c));                                                               }
#define REGISTER_CONSTRUCTOR_1( name, argType1)                                                                                                                                                   {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d));                                                           }
#define REGISTER_CONSTRUCTOR_2( name, argType1, argType2)                                                                                                                                         {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e));                                                       }
#define REGISTER_CONSTRUCTOR_3( name, argType1, argType2, argType3)                                                                                                                               {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f));                                                   }
#define REGISTER_CONSTRUCTOR_4( name, argType1, argType2, argType3, argType4)                                                                                                                     {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g));                                               }
#define REGISTER_CONSTRUCTOR_5( name, argType1, argType2, argType3, argType4, argType5)                                                                                                           {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h));                                           }
#define REGISTER_CONSTRUCTOR_6( name, argType1, argType2, argType3, argType4, argType5, argType6)                                                                                                 {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6));                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i));                                       }
#define REGISTER_CONSTRUCTOR_7( name, argType1, argType2, argType3, argType4, argType5, argType6, argType7)                                                                                       {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7));                                                                                                                                                                                                                                                                                                                                                                                                                                           Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j));                                   }
#define REGISTER_CONSTRUCTOR_8( name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8)                                                                             {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8));                                                                                                                                                                                                                                                                                                                                                                                        Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k));                               }
#define REGISTER_CONSTRUCTOR_9( name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8, argType9)                                                                   {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8)); argType9* l = (argType9*)alloca(sizeof(argType9));                                                                                                                                                                                                                                                                                                                                     Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k, *l));                           }
#define REGISTER_CONSTRUCTOR_10(name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8, argType9, argType10)                                                        {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8)); argType9* l = (argType9*)alloca(sizeof(argType9)); argType10* m = (argType10*)alloca(sizeof(argType10));                                                                                                                                                                                                                                                                               Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m));                       }
#define REGISTER_CONSTRUCTOR_11(name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8, argType9, argType10, argType11)                                             {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8)); argType9* l = (argType9*)alloca(sizeof(argType9)); argType10* m = (argType10*)alloca(sizeof(argType10)); argType11* n = (argType11*)alloca(sizeof(argType11));                                                                                                                                                                                                                         Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m, *n));                   }
#define REGISTER_CONSTRUCTOR_12(name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8, argType9, argType10, argType11, argType12)                                  {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8)); argType9* l = (argType9*)alloca(sizeof(argType9)); argType10* m = (argType10*)alloca(sizeof(argType10)); argType11* n = (argType11*)alloca(sizeof(argType11)); argType12* o = (argType12*)alloca(sizeof(argType12));                                                                                                                                                                   Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m, *n, *o));               }
#define REGISTER_CONSTRUCTOR_13(name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8, argType9, argType10, argType11, argType12, argType13)                       {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8)); argType9* l = (argType9*)alloca(sizeof(argType9)); argType10* m = (argType10*)alloca(sizeof(argType10)); argType11* n = (argType11*)alloca(sizeof(argType11)); argType12* o = (argType12*)alloca(sizeof(argType12)); argType13* p = (argType13*)alloca(sizeof(argType13));                                                                                                             Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m, *n, *o, *p));           }
#define REGISTER_CONSTRUCTOR_14(name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8, argType9, argType10, argType11, argType12, argType13, argType14)            {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8)); argType9* l = (argType9*)alloca(sizeof(argType9)); argType10* m = (argType10*)alloca(sizeof(argType10)); argType11* n = (argType11*)alloca(sizeof(argType11)); argType12* o = (argType12*)alloca(sizeof(argType12)); argType13* p = (argType13*)alloca(sizeof(argType13)); argType14* q = (argType14*)alloca(sizeof(argType14));                                                       Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m, *n, *o, *p, *q));       }
#define REGISTER_CONSTRUCTOR_15(name, argType1, argType2, argType3, argType4, argType5, argType6, argType7, argType8, argType9, argType10, argType11, argType12, argType13, argType14, argType15) {__this* c = (__this*)alloca(sizeof(__this)); argType1*d = (argType1*)alloca(sizeof(argType1)); argType2*e = (argType2*)alloca(sizeof(argType2)); argType3* f = (argType3*)alloca(sizeof(argType3)); argType4* g = (argType4*)alloca(sizeof(argType4)); argType5* h = (argType5*)alloca(sizeof(argType5)); argType6* i = (argType6*)alloca(sizeof(argType6)); argType7* j = (argType7*)alloca(sizeof(argType7)); argType8* k = (argType8*)alloca(sizeof(argType8)); argType9* l = (argType9*)alloca(sizeof(argType9)); argType10* m = (argType10*)alloca(sizeof(argType10)); argType11* n = (argType11*)alloca(sizeof(argType11)); argType12* o = (argType12*)alloca(sizeof(argType12)); argType13* p = (argType13*)alloca(sizeof(argType13)); argType14* q = (argType14*)alloca(sizeof(argType14)); argType15* r = (argType15*)alloca(sizeof(argType15)); Reflection::MetaSingleton<__this>::Get()->AddConstructor(name, Constructor(*c, *d, *e, *f, *g, *h, *i, *j, *k, *l, *m, *n, *o, *p, *q, *r));   }

#define REGISTER_CONSTRUCTOR_(N) REGISTER_CONSTRUCTOR_##N
#define REGISTER_CONSTRUCTOR_EVAL(N) REGISTER_CONSTRUCTOR_(N)
#define RegisterConstructor(...) EXPAND(REGISTER_CONSTRUCTOR_EVAL(EXPAND(PP_NARG(__VA_ARGS__)))(__VA_ARGS__))


#define RegisterEnum_1(yourenum, lastelement)                                           \
Reflection::Metadata Reflection::MetaSingleton<yourenum>::metadata_(STRINGIFY(yourenum),\
                                                                    "",                 \
                                                                    sizeof(yourenum),   \
                                                                    ByValue,            \
                                                                    true);              \
Reflection::MetaSingleton<yourenum> yourenum##MetaSingleton;                            \
template <>                                                                             \
void Reflection::MetaSingleton<yourenum>::RegisterTypeData(void)                        \
{                                                                                       \
  Reflection::MetaSingleton<yourenum>::Get()->GetEnumConstants().resize(lastelement+1); \
  typedef yourenum __this;

#define RegisterEnum_2(namespace1, yourenum, lastelement)                                                       \
Reflection::Metadata Reflection::MetaSingleton<namespace1::yourenum>::metadata_(STRINGIFY(namespace1##yourenum),\
                                                                    STRINGIFY(namespace1),                      \
                                                                    sizeof(namespace1::yourenum),               \
                                                                    ByValue,                                    \
                                                                    true);                                      \
Reflection::MetaSingleton<namespace1::yourenum> namespace1##yourenum##MetaSingleton;                            \
template <>                                                                                                     \
void Reflection::MetaSingleton<namespace1::yourenum>::RegisterTypeData(void)                                    \
{                                                                                                               \
  Reflection::MetaSingleton<namespace1::yourenum>::Get()->GetEnumConstants().resize(namespace1::lastelement+1); \
  typedef namespace1::yourenum __this;                                      

#define RegisterEnum_3(namespace1, namespace2, yourenum, lastelement)                                                                   \
Reflection::Metadata Reflection::MetaSingleton<namespace1::namespace2::yourenum>::metadata_(STRINGIFY(namespace2##yourenum),            \
                                                                    STRINGIFY(namespace1##namespace2),                                  \
                                                                    sizeof(namespace1::namespace2::yourenum),                           \
                                                                    ByValue,                                                            \
                                                                    true);                                                              \
Reflection::MetaSingleton<namespace1::namespace2::yourenum> namespace1##namespace2##yourenum##MetaSingleton;                            \
template <>                                                                                                                             \
void Reflection::MetaSingleton<namespace1::namespace2::yourenum>::RegisterTypeData(void)                                                \
{                                                                                                                                       \
  Reflection::MetaSingleton<namespace1::namespace2::yourenum>::Get()->GetEnumConstants().resize(namespace1::namespace2::lastelement+1); \
  typedef namespace1::namespace2::yourenum __this;       

#define RegisterEnum_4(namespace1, namespace2, namespace3, yourenum, lastelement)                                                                               \
Reflection::Metadata Reflection::MetaSingleton<namespace1::namespace2::namespace3::yourenum>::metadata_(STRINGIFY(namespace3##yourenum),                        \
                                                                    STRINGIFY(namespace1##namespace2##namespace3),                                              \
                                                                    sizeof(namespace1::namespace2::namespace3::yourenum),                                       \
                                                                    ByValue,                                                                                    \
                                                                    true);                                                                                      \
Reflection::MetaSingleton<namespace1::namespace2::namespace3::yourenum> namespace1##namespace2##namespace3##yourenum##MetaSingleton;                            \
template <>                                                                                                                                                     \
void Reflection::MetaSingleton<namespace1::namespace2::namespace3::yourenum>::RegisterTypeData(void)                                                            \
{                                                                                                                                                               \
  Reflection::MetaSingleton<namespace1::namespace2::namespace3::yourenum>::Get()->GetEnumConstants().resize(namespace1::namespace2::namespace3::lastelement+1); \
  typedef namespace1::namespace2::namespace3::yourenum __this;     


#define RegisterEnum_5(namespace1, namespace2, namespace3, namespace4, yourenum, lastelement)                                                                                           \
Reflection::Metadata Reflection::MetaSingleton<namespace1::namespace2::namespace3::namespace4::yourenum>::metadata_(STRINGIFY(namespace4##yourenum),                                    \
                                                                    STRINGIFY(namespace1##namespace2##namespace3##namespace4),                                                          \
                                                                    sizeof(namespace1::namespace2::namespace3::namespace4::yourenum),                                                   \
                                                                    ByValue,                                                                                                            \
                                                                    true);                                                                                                              \
Reflection::MetaSingleton<namespace1::namespace2::namespace3::namespace4::yourenum> namespace1##namespace2##namespace3##namespace4##yourenum##MetaSingleton;                            \
template <>                                                                                                                                                                             \
void Reflection::MetaSingleton<namespace1::namespace2::namespace3::namespace4::yourenum>::RegisterTypeData(void)                                                                        \
{                                                                                                                                                                                       \
  Reflection::MetaSingleton<namespace1::namespace2::namespace3::namespace4::yourenum>::Get()->GetEnumConstants().resize(namespace1::namespace2::namespace3::namespace4::lastelement+1); \
  typedef namespace1::namespace2::namespace3::namespace4::yourenum __this;     



#define RegisterEnum_(N) RegisterEnum_##N
#define RegisterEnum_EVAL(N) RegisterEnum_(N)
#define RegisterEnum(...) EXPAND(RegisterEnum_EVAL(EXPAND(PP_NARG(__VA_ARGS__)))(__VA_ARGS__))




#define RegisterType_1( Namespace, metatype )                                                \
  Reflection::Metadata Reflection::MetaSingleton<Namespace::metatype>::metadata_( #metatype,                      \
                                                          #Namespace,                     \
                                                          sizeof(Namespace::metatype));   \
  Reflection::MetaSingleton<Namespace::metatype> Namespace##metatype##MetaSingleton;                  \
  template <>                                                                             \
  void Reflection::MetaSingleton<Namespace::metatype>::RegisterTypeData(void)                         \
  {                                                                                       \
    Namespace::metatype::RegisterTypeData();                                              \
  }                                                                                       \
  Reflection::Metadata* Namespace::metatype::GetMetatype()                                            \
  {                                                                                       \
    return FindType(Namespace::metatype);                                                 \
  }                                                                                       \
  void Namespace::metatype::RegisterTypeData(void)                                        \
  {                                                                                       \
    typedef Namespace::metatype __this;
/**/
#define RegisterType_2( Namespace, metatype, valuetype )                                     \
  Reflection::Metadata Reflection::MetaSingleton<Namespace::metatype>::metadata_( #metatype,                      \
                                                          #Namespace,                     \
                                                          sizeof(Namespace::metatype),    \
                                                          valuetype );                    \
  Reflection::MetaSingleton<Namespace::metatype> Namespace##metatype##MetaSingleton;      \
  template <>                                                                             \
  void Reflection::MetaSingleton<Namespace::metatype>::RegisterTypeData(void)             \
  {                                                                                       \
    Namespace::metatype::RegisterTypeData();                                              \
  }                                                                                       \
  Reflection::Metadata* Namespace::metatype::GetMetatype()                                \
  {                                                                                       \
    return FindType(Namespace::metatype);                                                 \
  }                                                                                       \
  void Namespace::metatype::RegisterTypeData(void)                                        \
  {                                                                                       \
    typedef Namespace::metatype __this;

#define RegisterType_(N) RegisterType_##N
#define RegisterType_EVAL(N) RegisterType_(N)
#define RegisterType(...) EXPAND(RegisterType_EVAL(EXPAND(PP_NARG(__VA_ARGS__)))(__VA_ARGS__))









#define SET_VALUE(objectToSet,                                              \
                  MetaData,                                                 \
                  valueString)                                              \
{                                                                           \
  char* genericObj = (reinterpret_cast<char*>(objectToSet));                \
  if(MetaData->GetName() == "bool")                                         \
  {                                                                         \
    bool* value = reinterpret_cast<bool*>(genericObj);                      \
    if(valueString == "true")                                               \
    {                                                                       \
      *value = true;                                                        \
    }                                                                       \
    else                                                                    \
    {                                                                       \
      *value = false;                                                       \
    }                                                                       \
  }                                                                         \
  else if(MetaData->GetName() == "char")                                    \
  {                                                                         \
    char* value = reinterpret_cast<char*>(genericObj);                      \
    *value = valueString[0];                                                \
  }                                                                         \
  else if(MetaData->GetName() == "short")                                   \
  {                                                                         \
    short* value = reinterpret_cast<short*>(genericObj);                    \
    *value = static_cast<short>(std::stoi(valueString.c_str()));            \
  }                                                                         \
  else if(MetaData->GetName() == "int")                                     \
  {                                                                         \
    int* value = reinterpret_cast<int*>(genericObj);                        \
    *value = std::stoi(valueString.c_str());                                \
  }                                                                         \
  else if(MetaData->GetName() == "long")                                    \
  {                                                                         \
    long* value = reinterpret_cast<long*>(genericObj);                      \
    *value = std::stol(valueString.c_str());                                \
  }                                                                         \
  else if(MetaData->GetName() == "long long")                               \
  {                                                                         \
    long long* value = reinterpret_cast<long long*>(genericObj);            \
    *value = std::stoll(valueString.c_str());                               \
  }                                                                         \
  else if(MetaData->GetName() == "float")                                   \
  {                                                                         \
    float* value = reinterpret_cast<float*>(genericObj);                    \
    *value = std::stof(valueString.c_str());                                \
  }                                                                         \
  else if(MetaData->GetName() == "double")                                  \
  {                                                                         \
    double* value = reinterpret_cast<double*>(genericObj);                  \
    *value = std::stod(valueString.c_str());                                \
  }                                                                         \
  else if(MetaData->GetName() == "long double")                             \
  {                                                                         \
    long double* value = reinterpret_cast<long double*>(genericObj);        \
    *value = std::stold(valueString.c_str());                               \
  }                                                                         \
  else if(MetaData->GetName() == "unsigned")                                \
  {                                                                         \
    unsigned* value = reinterpret_cast<unsigned*>(genericObj);              \
    *value = static_cast<unsigned>(std::stol(valueString.c_str()));         \
  }                                                                         \
  else if(MetaData->GetName() == "unsigned long")                           \
  {                                                                         \
    unsigned long* value = reinterpret_cast<unsigned long*>(genericObj);    \
    *value = std::stoul(valueString.c_str());                               \
  }                                                                         \
  else if(MetaData->GetName() == "unsigned short")                          \
  {                                                                         \
    unsigned short* value = reinterpret_cast<unsigned short*>(genericObj);  \
    *value = std::stoi(valueString.c_str());                                \
  }                                                                         \
  else if(MetaData->GetName() == "unsigned char")                           \
  {                                                                         \
    unsigned char* value = reinterpret_cast<unsigned char*>(genericObj);    \
    *value = valueString[0];                                                \
  }                                                                         \
}
//  else if(MetaData->GetName() == "string")                                  \
//  {                                                                         \
//    std::string* value = reinterpret_cast<std::string*>(genericObj);                  \
//    value->assign(valueString);                                             \
//  }                                                                         \
//}

