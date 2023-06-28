#pragma once
#include "sub-engine.h"

#define MAX_CHANNEL_ALLOCATED 512

/** @brief Holds the SoundEngine content and functions */
typedef struct SoundEngine
{
    /** @brief Fetch a music track from the assets folder.
     * @param trackName the name of the music track.
     * @return The music track. NULL if the track does not exist.
     */
    musicTrack *(*getTrack)(const char *trackName);

    /** @brief Play a music track. If the track is already playing,
              this will be ignored. If another track is already playing,
              it will be stopped after a transition.
              Paused tracks will remain untouch.
     * @param track The musicTrack object to play.
    */
    void (*playTrack)(musicTrack *track);

    /** @brief Play a music track effect from it's Id
              as specified in the assets folder.
              An effect is played once.
     * @param effectId the Id of the effect to play.
    */
    void (*playTrackEffect)(int effectId);

    /** @brief Stop the music tracks currently playing.
     */
    void (*stopTracks)(void);

    /** @brief Pause the music tracks currently playing.
     */
    void (*pauseTracks)(void);

    /** @brief Resume the music tracks currently paused.
              If a track is not paused, it will be stopped.
    */
    void (*resumeTracks)(void);

    /** @brief Set the master volume.
     * @param volume The volume to set between 0.0 and 1.0.
     */
    void (*setMasterVolume)(float volume);

    /** @brief Set the volume playing volume for music.
     * @param volume The volume to set between 0.0 and 1.0.
     */
    void (*setMusicVolume)(float volume);

    /** @brief Set the volume playing volume for effects.
     * @param volume The volume to set between 0.0 and 1.0.
     */
    void (*setEffectVolume)(float volume);

    /** @brief Set the volume playing volume for ambience tracks.
     * @param volume The volume to set between 0.0 and 1.0.
     */
    void (*setAmbientVolume)(float volume);

    /** @brief Set the volume playing volume for voice.
     * @param volume The volume to set between 0.0 and 1.0.
     */
    void (*setDialogVolume)(float volume);

    /** @brief Fetch the master volume.
     * @return Float between 0.0 and 1.0.
     */
    float (*getMasterVolume)(void);

    /** @brief Fetch the music volume.
     * @return Float between 0.0 and 1.0.
     */
    float (*getMusicVolume)(void);

    /** @brief Fetch the effects volume.
     * @return Float between 0.0 and 1.0.
     */
    float (*getEffectVolume)(void);

    /** @brief Fetch the ambience volume.
     * @return Float between 0.0 and 1.0.
     */
    float (*getAmbientVolume)(void);

    /** @brief Fetch an effect from it's name.
     * @param effectName The name of the effect.
     * @return The effect. NULL if the effect does not exist.
     */
    soundEffect *(*getEffect)(const char *effectName);

    /** @brief Play an effect.
     * @param effect The effect to play.
     * @return The soundEffect instance.
     */
    soundEffectInstance *(*playEffect)(soundEffect *effect);

    /** @brief Stop an effect playing. Ignored if the effect is not playing.
     * @param Pointer to the instance of the effect to stop.
              This will be set to NULL.
     */
    void (*stopEffect)(soundEffectInstance **instance);

    /** @brief Fetch a dialog from it's name.
     * @param dialogName The name of the dialog.
     * @return The dialog. NULL if the effect does not exist.
     */
    dialog *(*getDialog)(const char *dialogName);

    /** @brief Play a dialog.
     * @param dialog The dialog to play.
     * @return The dialog instance.
     */
    dialogInstance *(*playDialog)(dialog *dialog);

    /** @brief Stop a dialog playing. Ignored if the dialog is not playing.
     * @param Pointer to the instance of the dialog to stop.
              This will be set to NULL.
     */
    void (*stopDialog)(dialogInstance **instance);

    /** @brief Fetch an ambient track from it's name.
     * @param ambientTrackName The name of the ambient track.
     * @return The ambient track. NULL if the ambiant track does not exist.
     */
    ambientTrack *(*getAmbientTrack)(const char *ambientTrackName);

    /** @brief Play an ambient track. If the track is already playing, this will be ignored. If another track is already playing, it will not be affetced.
     * @param track The ambientTrack object to play.
              The track will play in loop until stopped.
     */
    ambientTrackInstance *(*playAmbientTrack)(ambientTrack *track);

    /** @brief Stop an ambient track playing. Ignored if the track is not playing.
     * @param track A pointer to the ambientTrack instance to stop.
              This will be set to NULL.
     */
    void (*stopAmbientTrack)(ambientTrackInstance **track);

    /** @brief Stop all the ambient tracks currently playing.
     */
    void (*stopAmbientTracks)(void);

    /** @brief Pause all effects, musics and ambience tracks.
     */
    void (*pause)(void);

    /** @brief Resume all effects, musics and ambience tracks. All effects, musics and ambience tracks currently playing will be stopped.
     */
    void (*resume)(void);
} SoundEngine;

/** @brief Initialize the sound engine.
          This will load all the music tracks and effects.
 * @param MusicWeightControl The music weight control. Ignore if NULL.
 * @return SoundEngine A new instance sound engine. NULL if failed.
 */
SoundEngine *initSoundEngine(float *MusicWeightControl);

/** @brief Free the sound engine. This will stop all tracks and effects,
          and free all resources.
 * @param SoundEngine The sound engine to free.
 */
void destroySoundEngine(SoundEngine *SoundEngine);