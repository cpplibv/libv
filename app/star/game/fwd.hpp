// Project: libv, File: app/star/game/fwd.hpp

#pragma once


namespace star {

// -------------------------------------------------------------------------------------------------

class ClientConfigT;
template <typename ConfigT> class Config;
using ClientConfig = Config<ClientConfigT>;

class GameClient;

// UI
class ControllerContext;

// Scene - Surface
struct Surface;
class CanvasSurface;

// -------------------------------------------------------------------------------------------------

} // namespace star
