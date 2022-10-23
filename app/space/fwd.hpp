// Project: libv, File: app/space/fwd.hpp

#pragma once

// pro
#include <space/sim/fwd.hpp>


namespace space {

// -------------------------------------------------------------------------------------------------

class GameInstance;
class GameSession;
class GameThread;

class Renderer;

class User;

class MemoryStore;

class UniverseGenerationSettings;

class Playout;
class PlayoutDelayBuffer;

class SpaceCanvas;
class SpaceLobby;

class Lobby;
class NetworkLobby;
class NetworkClient;
class NetworkServer;

// -------------------------------------------------------------------------------------------------

class CTO_Introduction;

class SnapshotLobby;

//class CTO_ClientKick;
class CTO_ChatMessage;
class CTO_ClientJoined;
class CTO_ClientLeave;
class CTO_LobbyStatus;
//class CTO_LobbyClose;

class CTO_FleetSpawn;
class CTO_FleetSelect;
class CTO_FleetSelectAdd;
class CTO_FleetSelectBox;
class CTO_FleetClearSelection;
class CTO_FleetMove;
class CTO_FleetMoveQueue;
class CTO_FleetAttackFleet;
class CTO_FleetAttackFleetQueue;
class CTO_FleetAttackPlanet;
class CTO_FleetAttackPlanetQueue;
class CTO_ClearFleets;
class CTO_Shuffle;
class CTO_PlanetSpawn;
class CTO_ClearPlanets;

class CTO_TrackView;
class CTO_CameraWarpTo;
//class CTO_CameraMovement;
//class CTO_MouseMovement;

// -------------------------------------------------------------------------------------------------

class SceneMainControl;

// -------------------------------------------------------------------------------------------------

} // namespace space
