/*
    Copyright 2021 Team 16BitSoft

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
    AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef AUDIO
#define AUDIO

class Audio
{
public:

    Audio(void);
    virtual ~Audio(void);

    bool AudioWorking;

    char Mix_Init_Error[256];

    #define MusicTotal      31
    Mix_Music *MusicTrack[MusicTotal];
    int MusicVolume;
    Uint8 CurrentlySelectedMusicTrack;
    Uint8 MusicJukeboxMode;
    Uint8 CurrentMusicTrackPlaying;

    #define SoundTotal      18
    Mix_Chunk *SoundFX[SoundTotal];
    int SoundVolume;

    void SetupAudio(void);
    void PlayMusic(Uint8 musicIndex, int loop);
    void PlayDigitalSoundFX(Uint8 soundIndex, int loop);
};

#endif
