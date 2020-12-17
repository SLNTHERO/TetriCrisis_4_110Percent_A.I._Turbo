/*
    Copyright 2020 Team 16BitSoft

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

#ifndef INPUT
#define INPUT

class Input
{
public:

	Input(void);
	virtual ~Input(void);

    bool DEBUG;

    bool EXIT_Game;

    SDL_Event Event;

    SDL_Keycode KeyOnKeyboardPressedByUser;

    int DelayAllUserInput;

    bool ShiftKeyPressed;

    SDL_Keymod KeyModState;

    bool MouseButtonPressed[2];
    bool MouseButtonWasClicked[2];
    int MouseX, MouseY;
    int MouseButtonClicked;

    #define Keyboard        0
    #define JoystickOne     1
    #define JoystickTwo     2
    #define JoystickThree   3
    #define Any             4
    #define CPU             5
    #define Mouse           6

    #define CENTER      0
    #define UP          1
    #define RIGHT       3
    #define DOWN        5
    #define LEFT        7
    Uint8 JoystickDirectionHorizontal[5];
    Uint8 JoystickDirectionVertical[5];

    #define OFF         0
    #define ON          1
    Uint8 JoystickButtonOne[5];
    Uint8 JoystickButtonTwo[5];
    bool JoystickButtonOnePressed[5];
    bool JoystickButtonTwoPressed[5];

    #define JoySetupNotStarted          0
    #define Joy1SetupPressUP            1
    #define Joy1SetupPressDOWN          2
    #define Joy1SetupPressLEFT          3
    #define Joy1SetupPressRIGHT         4
    #define Joy1SetupPressBUTTONOne     5
    #define Joy1SetupPressBUTTONTwo     6
    #define Joy2SetupPressUP            7
    #define Joy2SetupPressDOWN          8
    #define Joy2SetupPressLEFT          9
    #define Joy2SetupPressRIGHT         10
    #define Joy2SetupPressBUTTONOne     11
    #define Joy2SetupPressBUTTONTwo     12
    #define Joy3SetupPressUP            13
    #define Joy3SetupPressDOWN          14
    #define Joy3SetupPressLEFT          15
    #define Joy3SetupPressRIGHT         16
    #define Joy3SetupPressBUTTONOne     17
    #define Joy3SetupPressBUTTONTwo     18
    int JoystickSetupProcess;

    #define KeyboardSetupNotStarted     0
    #define KeyboardSetupPressOne       1
    #define KeyboardSetupPressTwo       2
    #define KeyboardSetupPressUP        3
    #define KeyboardSetupPressRIGHT     4
    #define KeyboardSetupPressDOWN      5
    #define KeyboardSetupPressLEFT      6
    #define KeyboardSetupPressPause     7
    int KeyboardSetupProcess;

    int UserDefinedKeyButtonOne;
    int UserDefinedKeyButtonTwo;
    int UserDefinedKeyUP;
    int UserDefinedKeyRIGHT;
    int UserDefinedKeyDOWN;
    int UserDefinedKeyLEFT;
    int UserDefinedKeyPause;

    int JoyUP[3];
    int JoyDOWN[3];
    int JoyLEFT[3];
    int JoyRIGHT[3];
    int JoyButton1[3];
    int JoyButton2[3];

    #define Axis0       0
    #define Axis1       1
    #define Axis2       2
    #define Axis3       3
    #define Axis4       4
    #define Axis5       5
    #define Axis6       6
    #define Axis7       7
    #define Button0     8
    #define Button1     9
    #define Button2     10
    #define Button3     11
    #define Button4     12
    #define Button5     13
    #define Button6     14
    #define Button7     15
    #define Button8     16
    #define Button9     17
    #define Button10    18
    #define Button11    19
    #define Button12    20
    #define Button13    21
    #define Button14    22
    #define Button15    23
    #define Button16    24

	SDL_Joystick *JoystickDeviceOne;
	SDL_Joystick *JoystickDeviceTwo;
	SDL_Joystick *JoystickDeviceThree;
    int NumberOfJoyButtons[3];
    int NumberOfJoyAxises[3];

    int JoystickDisabled[3];

    int QueryJoysticksForAction(Uint8 joy, bool justButtons);

    void GetAllUserInput(void);

};

#endif
