// include safety
#pragma once

// include of SDL2 libs
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "SE_types.h"

#include "listManager.h"
#include "volumeControl.h"


// les noms pour tom
// storage of the loaded elements
typedef MusicStorageElement musicTrack;
typedef StorageElement soundEffect;
typedef StorageElement ambientTrack;
typedef StorageElement dialog;

// storage of the playing elements
typedef RequestedElement musicTrackInstance;
typedef PlayingElement soundEffectInstance;
typedef PlayingElement ambientTrackInstance;
typedef PlayingElement dialogInstance;