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

#include <stdio.h>
#include <cstring>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "visuals.h"

#include "screens.h"

extern Screens* screens;

//-------------------------------------------------------------------------------------------------
Visuals::Visuals(void)
{
    CoreFailure = false;

    ForceAspectRatio = true;

    FullScreenMode = 0;

    FrameLock = 16;
    SystemTicks = SDL_GetTicks();
    NextFrameTicks = SystemTicks + FrameLock;
    NumberOfFrames = 0;

    CurrentFramePerSecond = 0;

    for (int index = 0; index < 10; index++)
    {
        FramesPerSecondArray[index] = 0;
    }

    NextSecondTick = 0;
    AverageFPS = 0;

    TotalNumberOfLoadedStaffTexts = 0;

    TextCacheCurrentIndex = 0;

	for (int index = 0; index < NumberOfTextsCached; index++)
	{
		TextTexture[index] = NULL;
		TextOutlineTexture[index] = NULL;
	}
    ClearTextCache();

    if (TTF_Init()==-1)
    {
        printf( "SDL2_TTF initialization failed: %s\n", TTF_GetError() );
        CoreFailure = true;
    }
}

//-------------------------------------------------------------------------------------------------
Visuals::~Visuals(void)
{
    ClearTextCache();

    UnloadFontsFromMemory();

    TTF_Quit();

    for (int index = 0; index < NumberOfSprites; index++)
    {
        if (Sprites[index].Texture != NULL)
        {
            SDL_DestroyTexture(Sprites[index].Texture);
        }
    }
    printf("Unloaded all sprite images from memory.\n");

    SDL_DestroyRenderer(Renderer);
    printf("SDL2 renderer destroyed.\n");

    SDL_DestroyWindow(Window);
    printf("SDL2 window destroyed.\n");
}

//-------------------------------------------------------------------------------------------------
void Visuals::CalculateFramerate(void)
{
    SystemTicks = SDL_GetTicks();
    NextFrameTicks = SystemTicks + FrameLock;

    NumberOfFrames++;

    if (SystemTicks > NextSecondTick)
    {
        NextSecondTick = SystemTicks + 1000;

        FramesPerSecondArray[CurrentFramePerSecond] = NumberOfFrames;
        NumberOfFrames = 0;

        if (CurrentFramePerSecond < 9)  CurrentFramePerSecond++;
        else  CurrentFramePerSecond = 0;

        Uint32 frameTotal = 0;
        for (int index = 0; index < 10; index++)
        {
            frameTotal += FramesPerSecondArray[index];
        }

        AverageFPS = frameTotal / 10;
    }
}

//-------------------------------------------------------------------------------------------------
void Visuals::ProcessFramerate(void)
{
    SystemTicks = SDL_GetTicks();
    if (NextFrameTicks > SystemTicks)  SDL_Delay(NextFrameTicks - SystemTicks);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::InitializeWindow(void)
{
    SDL_Surface* windowIcon = SDL_LoadBMP("data/visuals/icon.bmp");

    Window = NULL;
    Window = SDL_CreateWindow("''T-Crisis 4 110% A.I. Turbo Remix'' Retail3 Final - By Team 16BitSoft"
                                 , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_RESIZABLE);

    if (Window == NULL)
    {
        printf( "SDL2 create window failed: %s\n", SDL_GetError() );
        CoreFailure = true;
        return(false);
    }
    else  printf("SDL2 window created.\n");

    WindowWidthCurrent = 640;
    WindowHeightCurrent = 480;

    SDL_SetWindowIcon(Window, windowIcon);

    Renderer = NULL;
    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

    if (Renderer == NULL)
    {
        printf( "SDL2 create renderer failed: %s\n", SDL_GetError() );
        CoreFailure = true;
        return(false);
    }
    else  printf("SDL2 renderer created.\n");

    return(true);
}

//-------------------------------------------------------------------------------------------------
void Visuals::ClearScreenBufferWithColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
    SDL_SetRenderDrawColor(Renderer, red, green, blue, alpha);
    SDL_RenderFillRect(Renderer, NULL);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadSpritesAndInitialize(void)
{
char filePath[256];
SDL_Surface *tempSurface = NULL;
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    for (int index = 0; index < NumberOfSprites; index++)
    {
        Sprites[index].Texture = NULL;
    }

    for (int index = 0; index < NumberOfSprites; index++)
    {
        strcpy(filePath, "~");

        switch(index)
        {
            case 0:
                strcpy(filePath, "data/visuals/Screen-Fade-Black-Box.png");
                break;

            case 1:
                strcpy(filePath, "data/visuals/16BitSoft-Logo.png");
                break;

            case 2:
                strcpy(filePath, "data/visuals/BG-SaintBasilsCathedral.png");
                break;

            case 3:
                strcpy(filePath, "data/visuals/T-C-4a-Logo.png");
                break;

            case 4:
                strcpy(filePath, "data/visuals/Keyboard-Controls.png");
                break;

            case 7:
                strcpy(filePath, "data/visuals/Line.png");
                break;

            case 9:
                strcpy(filePath, "data/visuals/SDL-Logo.png");
                break;

            case 10:
                strcpy(filePath, "data/visuals/SDL2-Logo.png");
                break;

            case 13:
                strcpy(filePath, "data/visuals/TC1Palm-Title.png");
                break;

            case 14:
                strcpy(filePath, "data/visuals/TC1Palm-InGame.png");
                break;

            case 15:
                strcpy(filePath, "data/visuals/Review.png");
                break;

            case 17:
                strcpy(filePath, "data/visuals/T-Crisis_PromoCover.png");
                break;

            case 30:
                strcpy(filePath, "data/visuals/PlayfieldStory.png");
                break;

            case 31:
                strcpy(filePath, "data/visuals/Playfield.png");
                break;

            case 32:
                strcpy(filePath, "data/visuals/Playfield-Block-Attack.png");
                break;


            case 39:
                strcpy(filePath, "data/story/BG-Space.png");
                break;

            case 40:
                strcpy(filePath, "data/story/Mars.png");
                break;

            case 41:
                strcpy(filePath, "data/story/ShipAway.png");
                break;

            case 42:
                strcpy(filePath, "data/story/Earth.png");
                break;

            case 43:
                strcpy(filePath, "data/story/ShipComing.png");
                break;

            case 44:
                strcpy(filePath, "data/story/Explosion.png");
                break;


            case 70:
                strcpy(filePath, "data/story/Story1.png");
                break;

            case 71:
                strcpy(filePath, "data/story/Story2.png");
                break;

            case 72:
                strcpy(filePath, "data/story/Story3.png");
                break;

            case 73:
                strcpy(filePath, "data/story/Story4.png");
                break;


            case 75:
                strcpy(filePath, "data/story/End1.png");
                break;


            case 100:
                strcpy(filePath, "data/visuals/BG-SaintBasilsCathedral.png");
                break;

            case 101:
                strcpy(filePath, "data/visuals/BG-MIG31.png");
                break;

            case 102:
                strcpy(filePath, "data/visuals/BG-GT-R.png");
                break;

            case 103:
                strcpy(filePath, "data/visuals/BG-NY.png");
                break;

            case 104:
                strcpy(filePath, "data/visuals/BG-Van-Gogh.png");
                break;

            case 105:
                strcpy(filePath, "data/visuals/BG-Kittens.png");
                break;

            case 106:
                strcpy(filePath, "data/visuals/BG-Psycho.png");
                break;

            case 108:
                strcpy(filePath, "data/visuals/BG-Story.png");
                break;

            case 155:
                strcpy(filePath, "data/visuals/Crack.png");
                break;


            default:
                break;
        }

        if (filePath[0] != '~')
        {
            tempSurface = IMG_Load(filePath);

            if (!tempSurface)
            {
                printf( "Image loading failed: %s\n", IMG_GetError() );
                CoreFailure = true;
                return(false);
            }
        }

        if (filePath[0] != '~')
        {
            Sprites[index].Texture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

            Sprites[index].ScreenX = 320;
            Sprites[index].ScreenY = 240;
            Sprites[index].ScaleX = 1.0;
            Sprites[index].ScaleY = 1.0;
            Sprites[index].RotationDegree = 0;
            Sprites[index].RedHue = 255;
            Sprites[index].GreenHue = 255;
            Sprites[index].BlueHue = 255;
            Sprites[index].Transparency = 255;
            Sprites[index].Smooth = false;
            Sprites[index].FlipX = false;
            Sprites[index].FlipY = false;

            Sprites[index].AnimationTimer = 0;

            SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
            Sprites[index].TextureWidthOriginal = textureWidth;
            Sprites[index].TextureHeightOriginal = textureHeight;

            SDL_FreeSurface(tempSurface);
        }
    }

    if ( LoadBoxSpriteSheetIntoMemory() == false)
    {
        CoreFailure = true;
        return(false);
    }

    if ( LoadInterfaceIntoMemory() == false)
    {
        CoreFailure = true;
        return(false);
    }

    if ( PreloadStaffTextsIntoMemory() == false)
    {
        CoreFailure = true;
        return(false);
    }

    printf("Loaded all sprite images into memory.\n");
    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadBoxSpriteSheetIntoMemory(void)
{
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    SDL_Surface *spriteSheet = NULL;
    SDL_Surface *boxTemp = NULL;
    SDL_Rect dstRect;
    SDL_Rect srcRect;
    int x = 1;
    for (int index = 200; index < 260; index++)
    {
        spriteSheet = IMG_Load("data/visuals/Boxes.png");
        if (!spriteSheet)
        {
            printf( "Image loading failed: %s\n", IMG_GetError() );
            CoreFailure = true;
            return(false);
        }

        boxTemp = IMG_Load("data/visuals/Box.png");
        if (!boxTemp)
        {
            printf( "Image loading failed: %s\n", IMG_GetError() );
            CoreFailure = true;
            return(false);
        }

        dstRect.x = 0;
        dstRect.y = 0;
        dstRect.w = 13;
        dstRect.h = 18;

        srcRect.w = 13;
        srcRect.h = 18;

        if (index > 199 && index < 210)
        {
            if (index == 200) x = 1;

            srcRect.x = x;
            srcRect.y = 1;
        }
        else if (index > 209 && index < 220)
        {
            if (index == 210) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*1;
        }
        else if (index > 219 && index < 230)
        {
            if (index == 220) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*2;
        }
        else if (index > 229 && index < 240)
        {
            if (index == 230) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*3;
        }
        else if (index > 239 && index < 250)
        {
            if (index == 240) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*4;
        }
        else if (index > 249 && index < 260)
        {
            if (index == 250) x = 1;

            srcRect.x = x;
            srcRect.y = 1+19*5;
        }

        SDL_BlitSurface(spriteSheet, &srcRect, boxTemp, &dstRect);
        x+=14;

        Sprites[index].Texture = SDL_CreateTextureFromSurface(Renderer, boxTemp);

        Sprites[index].ScreenX = 320;
        Sprites[index].ScreenY = 240;
        Sprites[index].ScaleX = 1.0;
        Sprites[index].ScaleY = 1.0;
        Sprites[index].RotationDegree = 0;
        Sprites[index].RedHue = 255;
        Sprites[index].GreenHue = 255;
        Sprites[index].BlueHue = 255;
        Sprites[index].Transparency = 255;
        Sprites[index].Smooth = false;
        Sprites[index].FlipX = false;
        Sprites[index].FlipY = false;

        Sprites[index].AnimationTimer = 0;

        SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
        Sprites[index].TextureWidthOriginal = textureWidth;
        Sprites[index].TextureHeightOriginal = textureHeight;

        SDL_FreeSurface(boxTemp);
    }

    SDL_FreeSurface(spriteSheet);

    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadInterfaceIntoMemory(void)
{
char filePath[256];
SDL_Surface *tempSurface = NULL;
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;
int indexOffset = 0;
char temp[256];

    for ( int index = 999; index < (1010+65+10); index++ )
    {
        Sprites[index].Texture = NULL;
    }

    for ( int index = 999; index < (1010+65+10); index++ )
    {
        strcpy(filePath, "~");

        switch(index)
        {
            case 999:
                strcpy(filePath, "data/visuals/Letter-Tile.png");
                break;

            case 1000:
                strcpy(filePath, "data/visuals/Button-Selector-Left.png");
                break;

            case 1001:
                strcpy(filePath, "data/visuals/Button-Selector-Right.png");
                break;

            case 1002:
                strcpy(filePath, "data/visuals/Selector-Line.png");
                break;

            case 1003:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1004:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1005:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1006:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1007:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1008:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1009:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            case 1010:
                strcpy(filePath, "data/visuals/Button.png");
                break;

            default:
                break;
        }

        if (index > 1019)  strcpy(filePath, "data/visuals/Letter-Tile.png");

        if (filePath[0] != '~')
        {
            tempSurface = IMG_Load(filePath);

            if (!tempSurface)
            {
                printf( "Image loading failed: %s\n", IMG_GetError() );
                CoreFailure = true;
                return(false);
            }
        }

        if ( index > 1002 && index < (1010+65+10) && filePath[0] != '~')
        {
            char buttonText[64];

            strcpy(buttonText, " \0");

            if      (index == 1003)  strcpy(buttonText, "START!\0");
            else if (index == 1004)  strcpy(buttonText, "Options\0");
            else if (index == 1005)  strcpy(buttonText, "How To Play\0");
            else if (index == 1006)  strcpy(buttonText, "High Scores\0");
            else if (index == 1007)  strcpy(buttonText, "About\0");
            else if (index == 1008)  strcpy(buttonText, "Exit\0");
            else if (index == 1009)  strcpy(buttonText, "Back\0");
            else if (index == 1010)  strcpy(buttonText, "Send Email\0");


            if ( index == (1020+26) )  indexOffset = 0;
            else if ( index == (1020+26+26) ) indexOffset = 0;

            if ( index > 1019 && index < (1020+26) )
            {
                sprintf( temp, "%c", (65+indexOffset) );
                strcpy(buttonText, temp);
                indexOffset++;
            }
            else if ( index > (1020+25) && index < (1020+26+26) )
            {
                sprintf( temp, "%c", (97+indexOffset) );
                strcpy(buttonText, temp);
                indexOffset++;
            }
            else if ( index > (1020+25+25) && index < (1020+26+26+10) )
            {
                sprintf( temp, "%c", (48+indexOffset) );
                strcpy(buttonText, temp);
                indexOffset++;
            }
            else if ( index == (1020+26+26+10) )
            {
                sprintf( temp, "%c", 43 );
                strcpy(buttonText, temp);
                indexOffset++;
            }
            else if ( index == (1020+26+26+10+1) )
            {
                sprintf( temp, "%c", 95 );
                strcpy(buttonText, temp);
                indexOffset++;
            }
            else if ( index == (1020+26+26+10+1+1) )
            {
                sprintf( temp, "%c", 60 );
                strcpy(buttonText, temp);
                indexOffset++;
            }

            SDL_Color textColor = { 0, 0, 0, 255 };
            SDL_Color outlineColor = { 255, 255, 255, 255 };
            SDL_Surface *text;
            SDL_Surface *textOutline;
            SDL_Rect destRect;

            text = TTF_RenderText_Blended(Font[0], buttonText, textColor);
            textOutline = TTF_RenderText_Solid(Font[0], buttonText, outlineColor);

            for (int posY = -2; posY < 3; posY++)
            {
                for (int posX = -2; posX < 3; posX++)
                {
                    destRect.x = (tempSurface->w / 2) - (text->w / 2) + posX;
                    destRect.y = (tempSurface->h / 2) - (text->h / 2) + posY;

                    SDL_BlitSurface(textOutline, NULL, tempSurface, &destRect);
                }
            }

            destRect.x = (tempSurface->w / 2) - (text->w / 2);
            destRect.y = (tempSurface->h / 2) - (text->h / 2);

            SDL_BlitSurface(text, NULL, tempSurface, &destRect);

            SDL_FreeSurface(text);
            SDL_FreeSurface(textOutline);
        }

        if (filePath[0] != '~')
        {
            Sprites[index].Texture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

            Sprites[index].ScreenX = 320;
            Sprites[index].ScreenY = 240;
            Sprites[index].ScaleX = 1.0;
            Sprites[index].ScaleY = 1.0;
            Sprites[index].RotationDegree = 0;
            Sprites[index].RedHue = 255;
            Sprites[index].GreenHue = 255;
            Sprites[index].BlueHue = 255;
            Sprites[index].Transparency = 255;
            Sprites[index].Smooth = false;
            Sprites[index].FlipX = false;
            Sprites[index].FlipY = false;

            Sprites[index].AnimationTimer = 0;

            SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
            Sprites[index].TextureWidthOriginal = textureWidth;
            Sprites[index].TextureHeightOriginal = textureHeight;

            SDL_FreeSurface(tempSurface);
        }
    }

    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadStaffTextIntoMemory(const char *staffText, Uint8 textBlue)
{
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    Sprites[1100+TotalNumberOfLoadedStaffTexts].Texture = NULL;

    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Color outlineColor = { 0, 0, 0, 255 };
    SDL_Surface *text;
    SDL_Surface *textOutline;
    SDL_Rect destRect;

    SDL_Surface *tempSurface = IMG_Load("data/visuals/Staff-Text-Image.png");

    if (!tempSurface)
    {
        printf( "Image loading failed: %s\n", IMG_GetError() );
        CoreFailure = true;
        return(false);
    }

    text = TTF_RenderText_Blended(Font[8], staffText, textColor);
    textOutline = TTF_RenderText_Solid(Font[8], staffText, outlineColor);

    for (int posY = -4; posY < 6; posY++)
    {
        for (int posX = -4; posX < 6; posX++)
        {
            destRect.x = (tempSurface->w / 2) - (text->w / 2) + posX;
            destRect.y = (tempSurface->h / 2) - (text->h / 2) + posY;

            SDL_BlitSurface(textOutline, NULL, tempSurface, &destRect);
        }
    }

    destRect.x = (tempSurface->w / 2) - (text->w / 2);
    destRect.y = (tempSurface->h / 2) - (text->h / 2);

    SDL_BlitSurface(text, NULL, tempSurface, &destRect);

    SDL_FreeSurface(text);
    SDL_FreeSurface(textOutline);

    Sprites[1100+TotalNumberOfLoadedStaffTexts].Texture = SDL_CreateTextureFromSurface(Renderer, tempSurface);

    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScreenX = 320;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScreenY = 240;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScaleX = 1.0;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].ScaleY = 1.0;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].RotationDegree = 0;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].RedHue = 255;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].GreenHue = 255;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].BlueHue = textBlue;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].Transparency = 255;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].Smooth = false;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].FlipX = false;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].FlipY = false;

    Sprites[1100+TotalNumberOfLoadedStaffTexts].AnimationTimer = 0;

    SDL_QueryTexture(Sprites[1100+TotalNumberOfLoadedStaffTexts].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);
    Sprites[1100+TotalNumberOfLoadedStaffTexts].TextureWidthOriginal = textureWidth;
    Sprites[1100+TotalNumberOfLoadedStaffTexts].TextureHeightOriginal = textureHeight;

    SDL_FreeSurface(tempSurface);

    TotalNumberOfLoadedStaffTexts++;
    return(true);
}

//-------------------------------------------------------------------------------------------------
bool Visuals::PreloadStaffTextsIntoMemory(void)
{
char textToDisplay[100];
char *copyright = new char[2];
char *reg = new char[2];
sprintf(copyright, "%c", 0xA9);
sprintf(reg, "%c", 0xAE);

    if ( LoadStaffTextIntoMemory(" ", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''T-Crisis 4 110% A.I. Turbo Remix''", 0) == false)  return(false);
    strcpy(textToDisplay, "Team 16BitSoft");
    if ( LoadStaffTextIntoMemory(textToDisplay, 255) == false)  return(false);

    strcpy(textToDisplay, "Original");
    strcat(textToDisplay, " Video Game Concept By:");
    if ( LoadStaffTextIntoMemory(textToDisplay, 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Alexey Pajitnov", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Video Game Made Possible By Our Mentors:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Garry Kitchen", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Andre' LaMothe", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Technology Credits:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''SDL'' Version 2.0 - Simple DirectMedia Layer", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("(SDL2_Image / SDL2_Mixer / SDL2_TTF)", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- Cross-Platform Open-Source -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("www.LibSDL.org", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Video Game Project Produced By:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Video Game Project Directed By:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''GT-R Twin TurboCharged'' SDL2 Game Engine Programmer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("100% Arcade Perfect To Home Conversion By:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("(Original Gameboy Version)", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''Gift Of Sight'' Artificial Intelligence Core Programmer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("32,000+ Average Lines Per Game!", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Game Designer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Game Programmer:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Game QA Tester:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Graphic Artist:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Storyboard Graphic Artist:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''erdalcetin83''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("For Hire Professional Graphic Artist On:", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("www.Fiverr.com", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Lead Music Composer/Remixer / Sound Editor:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''D.J. Fading Twilight''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Win ''Crisis+Mode'' BGM:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''No Easy Way out'' - Robert Tepper", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Cover by ''Minniva'' Feat Quentin Cornet", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''Story+Mode'' Music Soundtrack By:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("------------------------------------------------------------------------------", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("EpicBattle by PeriTune", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("http://peritune.com", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Music promoted by https://www.free-stock-music.com", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Attribution 4.0 International (CC BY 4.0)", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://creativecommons.org/licenses/by/4.0/", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("------------------------------------------------------------------------------", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Defense Matrix by Vyra", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://soundcloud.com/vyramusic", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Music promoted by https://www.free-stock-music.com", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Creative Commons Attribution-ShareAlike 3.0 Unported", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://creativecommons.org/licenses/by-sa/3.0/deed.en_US", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("------------------------------------------------------------------------------", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Running In The Streets by Vyra", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://soundcloud.com/vyramusic", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Music promoted by https://www.free-stock-music.com", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Creative Commons Attribution-ShareAlike 3.0 Unported", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://creativecommons.org/licenses/by-sa/3.0/deed.en_US", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("------------------------------------------------------------------------------", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Dragon Slayer by Makai-Symphony", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://soundcloud.com/makai-symphony", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Music promoted by https://www.free-stock-music.com", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Creative Commons Attribution-ShareAlike 3.0 Unported", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://creativecommons.org/licenses/by-sa/3.0/deed.en_US", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("------------------------------------------------------------------------------", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''Neo's Kiss'' Graphical User Interface Core By:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Support Designers/Programmers/Testers:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''fogobogo''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Daotheman''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''mattmatteh''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Video Game Made On:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Genuine ''openSUSE Tumbleweed KDE 64Bit'' Linux", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("www.openSUSE.org", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Windows EXE Executable Built With:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Code::Blocks'' Cross-Platform C++ I.D.E.", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("www.CodeBlocks.org", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Video Game Engine Performance Tested With:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Battle Encoder Shirase''' Free Windows Application", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("http://mion.faireal.net/BES/", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Graphics Edited In:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Genuine ''PixelNeo'' Windows Graphic Editor", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("https://visualneo.com/product/pixelneo", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("(Free Linux Alternative: ''Krita'')", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Audio Edited In:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Genuine ''GoldWave'' Windows Audio Editor", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("www.GoldWave.com", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("(Free Linux Alternative: ''Audacity'')", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Video Game Built On:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("A ''JeZxLee'' Pro-Built Thin Desktop System", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- Desktop Code Name: ''BumbleBee'' -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- Genuine ''openSUSE Tumbleweed KDE 64Bit'' Linux O.S. -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- 300 Watt Power Supply -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- ASRock Q1900M Motherboard -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- Intel Celeron 2GHz(2.42GHz OverClock) 4-Core CPU -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- Corsair 2 x 8GB(16GB Total) DDR3 RAM Memory -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- nVidia GeForce GT 1030 2GB GDDR5 PCIexpress GPU -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- ADATA 1TB SSD Solid State Drive(OS/Apps) -", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("- Western Digital 1TB HDD Hard Drive(Personal Data) -", 255) == false)  return(false);

    strcpy(textToDisplay, "Microsoft");
    strcat(textToDisplay, " Windows");
    strcat(textToDisplay, " Technical Advisors:");
    if ( LoadStaffTextIntoMemory(textToDisplay, 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Daotheman''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Linux Technical Advisors:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''JeZxLee''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''mattmatteh''", 255) == false)  return(false);

    strcpy(textToDisplay, "Apple");
    strcat(textToDisplay, " macOS");
    strcat(textToDisplay, " Technical Advisors:");
    if ( LoadStaffTextIntoMemory(textToDisplay, 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Someone?", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("Email: ''JeZxLee @ 16BitSoft'' To Help!", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("Big Thank You To All The People Who Helped Us:", 0) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''XaeL''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Blitz''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Blink''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''vipjun''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''farter''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''insatiate''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''clincher''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Integration''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''muf''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''simonlc''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''colour_thief''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Blockman''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Caithness''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Evi''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Zaphod77''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Sparks''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''rtrussell''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''AntTheAlchemist''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''icculus''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''sezero''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Zorg''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''slvn_''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Dominus''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Alienmilk''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''shachaf''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''sgibber2018''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''EvanR''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''demonicm1niac1''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''MrFlibble''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''qeed''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''slvr''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Terlisimo''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''tvault''", 255) == false)  return(false);
    if ( LoadStaffTextIntoMemory("''Daniel_Gibson''", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("You!", 255) == false)  return(false);

    if ( LoadStaffTextIntoMemory("''A 110% By Team 16BitSoft!''", 0) == false)  return(false);

    return(true);
}

//-------------------------------------------------------------------------------------------------
void Visuals::DrawSpriteOntoScreenBuffer(Uint16 index)
{
SDL_Rect destinationRect;
int windowWidth;
int windowHeight;
Uint32 textureFormat;
int textureAccess;
int textureWidth;
int textureHeight;

    SDL_GetWindowSize(Window, &windowWidth, &windowHeight);

    SDL_QueryTexture(Sprites[index].Texture, &textureFormat, &textureAccess, &textureWidth, &textureHeight);

    float winWidthFixed;
    float winHeightFixed;
    if (ForceAspectRatio == false)
    {
        winWidthFixed = (float)windowWidth / 640;
        winHeightFixed = (float)windowHeight / 480;
    }
    else
    {
        winWidthFixed = 1;
        winHeightFixed = 1;
    }

    destinationRect.x = ( Sprites[index].ScreenX * (winWidthFixed) )
                        - (  ( (textureWidth * Sprites[index].ScaleX) * (winWidthFixed) ) / 2  );
    destinationRect.y = ( Sprites[index].ScreenY * (winHeightFixed) )
                        - (  ( (textureHeight * Sprites[index].ScaleY) * (winHeightFixed) ) / 2  );
    destinationRect.w = textureWidth * Sprites[index].ScaleX * (winWidthFixed);
    destinationRect.h = textureHeight * Sprites[index].ScaleY * (winHeightFixed);

    SDL_SetTextureColorMod(Sprites[index].Texture, Sprites[index].RedHue, Sprites[index].GreenHue, Sprites[index].BlueHue);
    SDL_SetTextureAlphaMod(Sprites[index].Texture, Sprites[index].Transparency);

    if (Sprites[index].FlipX == false && Sprites[index].FlipY == false)
    {
        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, Sprites[index].RotationDegree
                         , NULL, SDL_FLIP_NONE);
    }
    else if (Sprites[index].FlipX == true && Sprites[index].FlipY == false)
    {
        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, Sprites[index].RotationDegree
                         , NULL, SDL_FLIP_HORIZONTAL);
    }
    else if (Sprites[index].FlipX == false && Sprites[index].FlipY == true)
    {
        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, Sprites[index].RotationDegree
                         , NULL, SDL_FLIP_VERTICAL);
    }
    else if (Sprites[index].FlipX == true && Sprites[index].FlipY == true)
    {
        double flipHorizontallyAndVerticallyDegreeFix = Sprites[index].RotationDegree+180;

        SDL_RenderCopyEx(Renderer, Sprites[index].Texture, NULL, &destinationRect, flipHorizontallyAndVerticallyDegreeFix
                         , NULL, SDL_FLIP_NONE);
    }
}

//-------------------------------------------------------------------------------------------------
bool Visuals::LoadFontsIntoMemory(void)
{
    Font[0] = NULL;
    if ( !(Font[0] = TTF_OpenFont("data/fonts/Font-01.ttf", 27)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[1] = NULL;
    if ( !(Font[1] = TTF_OpenFont("data/fonts/Font-01.ttf", 20)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[2] = NULL;
    if ( !(Font[2] = TTF_OpenFont("data/fonts/Font-01.ttf", 14)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[3] = NULL;
    if ( !(Font[3] = TTF_OpenFont("data/fonts/Line-01.ttf", 26)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[4] = NULL;
    if ( !(Font[4] = TTF_OpenFont("data/fonts/Font-01.ttf", 55)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[5] = NULL;
    if ( !(Font[5] = TTF_OpenFont("data/fonts/Font-02.ttf", 15+3)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[6] = NULL;
    if ( !(Font[6] = TTF_OpenFont("data/fonts/Font-02.ttf", 16)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[7] = NULL;
    if ( !(Font[7] = TTF_OpenFont("data/fonts/Line-01.ttf", 18)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    Font[8] = NULL;
    if ( !(Font[8] = TTF_OpenFont("data/fonts/Line-01.ttf", 22)) )
    {
        printf( "Error loading font: %s\n", TTF_GetError() );
        CoreFailure = true;
        return false;
    }

    printf("Loaded all TTF fonts into memory.\n");

    return true;
}

//-------------------------------------------------------------------------------------------------
void Visuals::UnloadFontsFromMemory(void)
{
    for (int index = 0; index < NumberOfFonts; index++)
    {
        if (Font[index] != NULL)  TTF_CloseFont(Font[index]);
    }
    printf("Unloaded all TTF fonts from memory.\n");
}

//-------------------------------------------------------------------------------------------------
void Visuals::ClearTextCache(void)
{
    for (int index = 0; index < NumberOfTextsCached; index++)
    {
        if (TextTexture[index] != NULL)  SDL_DestroyTexture(TextTexture[index]);
        if (TextOutlineTexture[index] != NULL)  SDL_DestroyTexture(TextOutlineTexture[index]);

        TextTexture[index] = NULL;
        TextOutlineTexture[index] = NULL;

        TextCachedText[index][0] = '\0';
        TextCachedScreenX[index] = 320;
        TextCachedScreenY[index] = 240;

        TextCachedWidth[index] = 0;
        TextCachedHeight[index] = 0;

        TextCacheRed[index] = -1;
        TextCacheGreen[index] = -1;
        TextCacheBlue[index] = -1;
    }

    TextCacheCurrentIndex = 0;

    printf("Cleared TTF text cache.\n");
}

//-------------------------------------------------------------------------------------------------
void Visuals::DrawTextOntoScreenBuffer(const char *textToDisplay, TTF_Font *font, int posX, int posY
                                       , Uint8 XJustification, Uint8 textRed, Uint8 textGreen, Uint8 textBlue
                                       , Uint8 outlineRed, Uint8 outlineGreen, Uint8 outlineBlue)
{
SDL_Color textColor = { textRed, textGreen, textBlue, 255 };
SDL_Color outlineColor = { outlineRed, outlineGreen, outlineBlue, 255 };
SDL_Surface *text = NULL;
SDL_Surface *textOutline = NULL;
SDL_Texture *textTexture = NULL;
SDL_Texture *textOutlineTexture = NULL;
SDL_Rect destinationRect;
SDL_Rect destinationOutlineRect;
int windowWidth;
int windowHeight;
int textCacheCheckIndex = NumberOfTextsCached;
bool sentenceMatches = true;
bool sentenceIsInCache = false;

    if (font != Font[5])  posY-=3;
    else  posY+=2;

    for (textCacheCheckIndex = 0; textCacheCheckIndex < NumberOfTextsCached; textCacheCheckIndex++)
    {
        sentenceMatches = true;
        if ( strlen(TextCachedText[textCacheCheckIndex]) == strlen(textToDisplay) )
        {
            for (Uint16 index = 0; index < strlen(textToDisplay); index++)
            {
                if (TextCachedText[textCacheCheckIndex][index] != textToDisplay[index])  sentenceMatches = false;
            }
        }
        else  sentenceMatches = false;

        if (sentenceMatches == true)
        {
            if (TextCachedScreenX[textCacheCheckIndex] == posX && TextCachedScreenY[textCacheCheckIndex] == posY)
            {
                sentenceIsInCache = true;

                if (TextCacheRed[textCacheCheckIndex] != textRed)  sentenceIsInCache = false;
                if (TextCacheGreen[textCacheCheckIndex] != textGreen)  sentenceIsInCache = false;
                if (TextCacheBlue[textCacheCheckIndex] != textBlue)  sentenceIsInCache = false;

                if (sentenceIsInCache == true)  break;
            }
        }
    }

    if (sentenceIsInCache == false)
    {
        if (TextTexture[TextCacheCurrentIndex] != NULL)  SDL_DestroyTexture(TextTexture[TextCacheCurrentIndex]);
        if (TextOutlineTexture[TextCacheCurrentIndex] != NULL)  SDL_DestroyTexture(TextOutlineTexture[TextCacheCurrentIndex]);
        TextTexture[TextCacheCurrentIndex] = NULL;
        TextOutlineTexture[TextCacheCurrentIndex] = NULL;
        TextCachedText[TextCacheCurrentIndex][0] = '\0';
        TextCachedScreenX[TextCacheCurrentIndex] = 320;
        TextCachedScreenY[TextCacheCurrentIndex] = 240;
        TextCachedWidth[TextCacheCurrentIndex] = 0;
        TextCachedHeight[TextCacheCurrentIndex] = 0;
        TextCacheRed[TextCacheCurrentIndex] = textRed;
        TextCacheGreen[TextCacheCurrentIndex] = textGreen;
        TextCacheBlue[TextCacheCurrentIndex] = textBlue;

        text = TTF_RenderText_Solid(font, textToDisplay, textColor);
        textOutline = TTF_RenderText_Solid(font, textToDisplay, outlineColor);

        strcpy(TextCachedText[TextCacheCurrentIndex], textToDisplay);

        TextCachedScreenX[TextCacheCurrentIndex] = posX;
        TextCachedScreenY[TextCacheCurrentIndex] = posY;

        TextCachedWidth[TextCacheCurrentIndex] = text->w;
        TextCachedHeight[TextCacheCurrentIndex] = text->h;

        textTexture = SDL_CreateTextureFromSurface(Renderer, text);
        TextTexture[TextCacheCurrentIndex] = SDL_CreateTextureFromSurface(Renderer, text);

        textOutlineTexture = SDL_CreateTextureFromSurface(Renderer, textOutline);
        TextOutlineTexture[TextCacheCurrentIndex] = SDL_CreateTextureFromSurface(Renderer, textOutline);

        if (XJustification == JustifyLeft)
        {
            posX = posX + (text->w / 2);
        }
        else if (XJustification == JustifyCenter)
        {
            posX = (640 / 2);
        }
        else if (XJustification == JustifyRight)
        {
            posX = (640 - posX) - (text->w / 2);
        }
        else if (XJustification == JustifyCenterOnPoint)
        {
            posX = posX;
        }

        if ( TextCacheCurrentIndex < (NumberOfTextsCached-1) )  TextCacheCurrentIndex++;
        else  TextCacheCurrentIndex = 0;
    }
    else
    {
        if (XJustification == JustifyLeft)
        {
            posX = posX + (TextCachedWidth[textCacheCheckIndex] / 2);
        }
        else if (XJustification == JustifyCenter)
        {
            posX = (640 / 2);
        }
        else if (XJustification == JustifyRight)
        {
            posX = (640 - posX) - (TextCachedWidth[textCacheCheckIndex] / 2);
        }
        else if (XJustification == JustifyCenterOnPoint)
        {
            posX = posX;
        }
    }

    SDL_GetWindowSize(Window, &windowWidth, &windowHeight);

    float winWidthFixed;
    float winHeightFixed;
    if (ForceAspectRatio == false)
    {
        winWidthFixed = (float)windowWidth / 640;
        winHeightFixed = (float)windowHeight / 480;
    }
    else
    {
        winWidthFixed = 1;
        winHeightFixed = 1;
    }

    if (sentenceIsInCache == false)
    {
        destinationRect.x = (posX * winWidthFixed) - ( (text->w * winWidthFixed) / 2 );
        destinationRect.y = (posY * winHeightFixed) - (winHeightFixed / 2) + 3;
        destinationRect.w = text->w * (winWidthFixed);
        destinationRect.h = text->h * (winHeightFixed);
    }
    else
    {
        destinationRect.x = (posX * winWidthFixed) - ( (TextCachedWidth[textCacheCheckIndex] * winWidthFixed) / 2 );
        destinationRect.y = (posY * winHeightFixed) - (winHeightFixed / 2) + 3;
        destinationRect.w = TextCachedWidth[textCacheCheckIndex] * (winWidthFixed);
        destinationRect.h = TextCachedHeight[textCacheCheckIndex] * (winHeightFixed);
    }

    destinationOutlineRect.x = destinationRect.x;
    destinationOutlineRect.y = destinationRect.y;
    destinationOutlineRect.w = destinationRect.w;
    destinationOutlineRect.h = destinationRect.h;

    for (Sint16 y = -3; y < 4; y+=1)
    {
        for (Sint16 x = -3; x < 4; x+=1)
        {
            destinationOutlineRect.x = destinationRect.x + x;
            destinationOutlineRect.y = destinationRect.y + y;

            if (sentenceIsInCache == false)
                SDL_RenderCopyEx(Renderer, textOutlineTexture, NULL, &destinationOutlineRect, 0, NULL, SDL_FLIP_NONE);
            else
            {
                SDL_RenderCopyEx(Renderer, TextOutlineTexture[textCacheCheckIndex], NULL, &destinationOutlineRect, 0, NULL, SDL_FLIP_NONE);
            }
        }
    }

    if (sentenceIsInCache == false)
        SDL_RenderCopyEx(Renderer, textTexture, NULL, &destinationRect, 0, NULL, SDL_FLIP_NONE);
    else
        SDL_RenderCopyEx(Renderer, TextTexture[textCacheCheckIndex], NULL, &destinationRect, 0, NULL, SDL_FLIP_NONE);

    SDL_DestroyTexture(textOutlineTexture);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(text);
    SDL_FreeSurface(textOutline);
}
