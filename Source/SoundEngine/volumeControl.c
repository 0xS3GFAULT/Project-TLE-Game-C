#include "sound-engine.h"
#include "volumeControl.h"


static float MasterVolume = 1.;
static float MusicVolume = 1.;
static float EffectVolume = 1.;
static float AmbientVolume = 1.;
static float DialogVolume = 1.;


void setMasterVolume(float volume)
{
    MasterVolume = volume;
    refreshVolume(MasterVolume, MusicVolume, EffectVolume, AmbientVolume, DialogVolume);
}

void setMusicVolume(float volume)
{
    MusicVolume = volume;
    refreshVolume(MasterVolume, MusicVolume, EffectVolume, AmbientVolume, DialogVolume);
}

void setEffectVolume(float volume)
{
    EffectVolume = volume;
    refreshVolume(MasterVolume, MusicVolume, EffectVolume, AmbientVolume, DialogVolume);
}

void setAmbientVolume(float volume)
{
    AmbientVolume = volume;
    refreshVolume(MasterVolume, MusicVolume, EffectVolume, AmbientVolume, DialogVolume);
}

void setDialogVolume(float volume)
{
    DialogVolume = volume;
    refreshVolume(MasterVolume, MusicVolume, EffectVolume, AmbientVolume, DialogVolume);
}

float getMasterVolume()
{
    return MasterVolume;
}

float getMusicVolume()
{
    return MusicVolume;
}

float getEffectVolume()
{
    return EffectVolume;
}

float getAmbientVolume()
{
    return AmbientVolume;
}

float getDialogVolume()
{
    return DialogVolume;
}
