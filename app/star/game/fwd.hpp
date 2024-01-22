// Project: libv, File: app/star/game/fwd.hpp

#pragma once

#include <memory>


namespace star {

// -------------------------------------------------------------------------------------------------

// --- Config ---
class ClientConfigT;
template <typename ConfigT> class Config;
using ClientConfig = Config<ClientConfigT>;

// --- Core ---
class GameClient;

// --- Controllers ---
struct TimeController;
using TimeController_sp = std::shared_ptr<TimeController>;

// --- Sim ---
struct Universe;
struct Starmap;
struct Surface;
using Universe_sp = std::shared_ptr<Universe>;
using Starmap_sp = std::shared_ptr<Starmap>;
using Surface_sp = std::shared_ptr<Surface>;

// --- UI Utility ---
class ControllerContext;

// --- UI ---
class CanvasStarmap;
class CanvasSurface;

// -------------------------------------------------------------------------------------------------

} // namespace star
