#pragma once

// volume setters
void setMasterVolume(float volume);
void setMusicVolume(float volume);
void setEffectVolume(float volume);
void setAmbientVolume(float volume);
void setDialogVolume(float volume);

// to refresh the volume in the other file
void refreshVolume(float master, float music, float effect, float ambient, float dialog);

// volume getters
float getMasterVolume();
float getMusicVolume();
float getEffectVolume();
float getAmbientVolume();
float getDialogVolume();