#pragma once


#include "meta/Type/MetaSingleton.h"


/*

Okay, so we need to bind the specializations of these appropriate,
and then we may create the elements with the correct names
we can then use the names for this archetype list to find the
correct associated game object, and pass it to whatever will
view it 

you may duplicate an archetype, create a blank archetype, or delete an archetype

when clicked, each archetype should be editable, or cloneable

each Unit should be able to change only local data, such as position, rotation,
scale, stat modifiers, velocity, acceleration

on the archetype editor, there are as many tabs as components,
each tab allowing you to edit a part of what makes the archetype

the same reasoning applies for per Unit changes, however, you may only
modify appropriate values, as stated above.

*/
