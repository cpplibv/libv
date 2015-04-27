// File: Renderer.hpp, Created on 2014. április 11. 23:24, Author: Vader

#pragma once

// external
// vLib
// std
// project
//#include "Algorithm/DynamicVisitor.hpp"
//
//template <typename T>
//using Renderer = DynamicVisitor<T>;
//using RendererBase = DynamicVisitorBase;
//
//template <typename T>
//using Renderable = DynamicVisitable<T>;
//using RenderableBase = DynamicVisitableBase;

//TODO P3: give function to validate {visitor - visitable} assignment ? ~~ kind of

//Felmerült bennem az a gondolat, hogy a rendererek varidic map szerüen legyen hozzárendelve a
//a renderablekhez


//minden létező renderer benn egy közös helyen 1 példánnyal, és mindenki csak egy "számot" tud
//rájuk huzni, hogy ők ki kezelje - (hibás assign ugyan az mint volt)


//vagy nem lehet-e megoldás, hogy függvény pointerrben tároljuk azt akit rendernél meghivunk
//EZ GYANÚS, hogy jó lesz... úgyan úgy kell egy ~~ bonyolult lefedés a templatekre meg a castokra,
//de csak renderable oldalon! renderernél csak egyszer! <<
//szóval a kód kell de a pattern repül~~
//kell assignRender fvény a renderable-ben melyben lehet hibát kezelni!! és tök jól ráadásul
//szóval kb csak annyi változik, hogy fvény ptr-t használok accept(renderProxy) helyett?

//szóval amiről szó lenne, az callbackek beregelése így eltünik egy dyn cast renderer oldaláról
//szóval a renderer egy type - object - object - callback tár lenne ami lehet fire-lni
//ha minden igaz így fire-nél lehetséges egyből a jó overloadra lőni!

//na szóval: render proxyba kell eldugni a type nfo-t, őket base-nél fogva set/get render

//ez a probléma nem oldható meg maradéktalanúl csak sig/slot-tal