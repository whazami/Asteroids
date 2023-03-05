#include "../../include/Utils/Joystick.h"
#pragma warning(disable : 4996)

static JOY_SHOCK_STATE getRoundedState(const JOY_SHOCK_STATE& s);

namespace wl
{
    int Joystick::jslJoystickId = 0;
    bool Joystick::rumbleOn = false;
    bool Joystick::isBluetooth = false;

    bool Joystick::pollEvent(sf::Event& e)
    {
        static JOY_SHOCK_STATE oldState = JslGetSimpleState(Joystick::jslJoystickId);
        const JOY_SHOCK_STATE state = JslGetSimpleState(Joystick::jslJoystickId);
        const int oldButtons = oldState.buttons & ~0xf; // le masque permet d'ignorer les
        const int buttons = state.buttons & ~0xf;       // fleches, qui seront gerees autrement

        if (!Joystick::isBluetooth)
            return false;

        if (!state) {
            oldState = state;
            emptyEvent(e);
            return false;
        }

        if (buttons != oldButtons) {
            e.type = sf::Event::JoystickButtonPressed;
            e.joystickButton.button = -1;
            for (int mask = JSMASK_OPTIONS, tmp = buttons; tmp; tmp &= ~mask, mask <<= 1)
                if ((tmp & mask) && !(oldButtons & mask))
                    e.joystickButton.button = (unsigned)Joystick::jslToSfml(tmp & mask);
            e.joystickButton.joystickId = Joystick::jslJoystickId;
            oldState = state;
            if (e.joystickButton.button == -1)
                return emptyEvent(e), false;
            return true;
        }

        int arrows = state.buttons & 0xf, oldArrows = oldState.buttons & 0xf;
        if (state != oldState || (arrows && arrows != oldArrows)) {
            e.type = sf::Event::JoystickMoved;
            e.joystickMove.joystickId = Joystick::jslJoystickId;
            e.joystickMove.axis = Joystick::getMovedAxis(state, oldState);
            if (e.joystickMove.axis != -1) {
                e.joystickMove.position = Joystick::getAxisPosition(e.joystickMove.axis);
                if (abs(e.joystickMove.position) < 10)
                    return oldState = state, emptyEvent(e), false;
                e.joystickMove.position = e.joystickMove.position > 0 ? 100 : -100;
            }
            oldState = state;
            if (e.joystickMove.axis == -1)
                return emptyEvent(e), false;
            return true;
        }

        emptyEvent(e);
        return false;
    }

    // Fonction de test de bouton de joystick (si SFML ne marche pas (bluetooth), JSL prend le relais)
    bool Joystick::isButtonPressed(const DualShock& sfmlButton)
    {
        if (!Joystick::isBluetooth)
            return sf::Joystick::isButtonPressed(0, (unsigned)sfmlButton);

        int jslButtonMask = Joystick::sfmlToJsl(sfmlButton);

        return (JslGetButtons(Joystick::jslJoystickId) & jslButtonMask) == jslButtonMask;
    }

    float Joystick::getAxisPosition(const sf::Joystick::Axis& axis)
    {
        if (!Joystick::isBluetooth)
            return sf::Joystick::getAxisPosition(0, axis);

        int arrows = JslGetButtons(Joystick::jslJoystickId) & 0xf;
        switch (axis) {
        case sf::Joystick::X:
            return JslGetLeftX(Joystick::jslJoystickId) * 100.f;
        case sf::Joystick::Y:
            return -JslGetLeftY(Joystick::jslJoystickId) * 100.f;
        case sf::Joystick::Z:
            return JslGetRightX(Joystick::jslJoystickId) * 100.f;
        case sf::Joystick::R:
            return -JslGetRightY(Joystick::jslJoystickId) * 100.f;
        case sf::Joystick::U:
            return JslGetLeftTrigger(Joystick::jslJoystickId) * 100.f;
        case sf::Joystick::V:
            return JslGetRightTrigger(Joystick::jslJoystickId) * 100.f;
        case sf::Joystick::PovX:
            arrows /= 4;            // arrows can be 0, 1 or 2
            if (arrows)
                return (arrows == 2 ? 1 : -1) * 100.f;
            return 0;
        case sf::Joystick::PovY:
            arrows &= 3;            // arrows can be 0, 1 or 2
            if (arrows)
                return (arrows == 1 ? 1 : -1) * 100.f;
            return 0;
        default:
            cout << "Error: Unknown axis (code: " << axis << ")." << endl;
            return -1;
        }
    }

    void Joystick::rumble(float rumblePercentage)
    {
        int bigRumble = 255.f * rumblePercentage / 100.f;
        if (bigRumble < 0)
            bigRumble = 0;
        if (bigRumble > 255)
            bigRumble = 255;

        if (Joystick::rumbleOn)
            JslSetRumble(Joystick::jslJoystickId, 0, bigRumble);
    }

    static int manageRumbleDeactivation()
    {
        Sleep(300);
        Joystick::rumble(0);
        return 0;
    }

    void Joystick::activateRumble()
    {
        Joystick::rumbleOn = true;
        Joystick::rumble(100);
        std::thread t(manageRumbleDeactivation);
        t.detach();
    }

    void Joystick::deactivateRumble()
    {
        Joystick::rumbleOn = false;
        Joystick::rumble(0);
    }

    bool Joystick::isRumbleActivated()
    {
        return Joystick::rumbleOn;
    }

    int Joystick::updateJsl()
    {
        // Printing JslConnectDevices output to the file '.jsl.log'
        int jslFile, save_out = dup(fileno(stdout));
        if ((jslFile = open(string(assetsPath + "/Game Data/.jsl.log").c_str(), O_WRONLY | O_CREAT), 07) == -1)
            cerr << "Error: cannot open jsl.log as output file." << endl;
        rewind(fdopen(jslFile, "w"));

        if (dup2(jslFile, fileno(stdout)) == -1)
            cerr << "Error: cannot redirect stdout" << endl;

        int nbJslDevices = JslConnectDevices();

        fflush(stdout); close(jslFile);
        if (dup2(save_out, fileno(stdout)) == -1)
            cerr << "Error: cannot bring stdout back" << endl;
        close(save_out);

        // Getting the printed output from '.jsl.log'
        ifstream jslOutput(assetsPath + "/Game Data/.jsl.log");
        string oldLine = "", line;

        Joystick::isBluetooth = false;
        while (!jslOutput.eof()) {
            getline(jslOutput, line);
            if (oldLine == "DS" && line == "initialise, set colour")
                Joystick::isBluetooth = true;
            oldLine = line;
        }

        if (!sf::Joystick::isConnected(0))
            Joystick::isBluetooth = false;

        jslOutput.close();

        // Clearing content in 'jsl.log'
        std::ofstream jslClear(assetsPath + "/Game Data/.jsl.log", std::ofstream::out | std::ofstream::trunc);
        jslClear.close();

        return nbJslDevices;
    }

    // Utils
    int Joystick::sfmlToJsl(DualShock sfmlButton)
    {
        int jslButton = 0;

        switch (sfmlButton)
        {
        case DualShock::Carre:
            jslButton = JSMASK_W;
            break;
        case DualShock::Croix:
            jslButton = JSMASK_S;
            break;
        case DualShock::Rond:
            jslButton = JSMASK_E;
            break;
        case DualShock::Triangle:
            jslButton = JSMASK_N;
            break;
        case DualShock::Share:
            jslButton = JSMASK_SHARE;
            break;
        case DualShock::Options:
            jslButton = JSMASK_OPTIONS;
            break;
        default:
            if (sfmlButton >= DualShock::L1 && sfmlButton <= DualShock::R2)
                jslButton = JSMASK_L * pow(2, sfmlButton - DualShock::L1);
            else if (sfmlButton == DualShock::L3 || sfmlButton == DualShock::R3)
                jslButton = JSMASK_LCLICK * pow(2, sfmlButton - DualShock::L3);
            else if (sfmlButton == DualShock::PS || sfmlButton == DualShock::PaveTactile)
                jslButton = JSMASK_PS * pow(2, sfmlButton - DualShock::PS);
            else
                cout << "Error: Unknown SFML button (code: " << (unsigned)sfmlButton << ")." << endl;
            break;
        }

        return jslButton;
    }

    DualShock Joystick::jslToSfml(int jslButton)
    {
        DualShock sfmlButton = (DualShock)-1;

        // Il ne peut y avoir qu'un bouton activé dans jslButton
        int oldJslButton = 0;
        for (int mask = 1; jslButton; jslButton &= ~mask, mask <<= 1)
            oldJslButton = jslButton;
        jslButton = oldJslButton;

        switch (jslButton)
        {
        case JSMASK_W:
            sfmlButton = DualShock::Carre;
            break;
        case JSMASK_S:
            sfmlButton = DualShock::Croix;
            break;
        case JSMASK_E:
            sfmlButton = DualShock::Rond;
            break;
        case JSMASK_N:
            sfmlButton = DualShock::Triangle;
            break;
        case JSMASK_SHARE:
            sfmlButton = DualShock::Share;
            break;
        case JSMASK_OPTIONS:
            sfmlButton = DualShock::Options;
            break;
        default:
            if (jslButton >= JSMASK_L && jslButton <= JSMASK_ZR)
                sfmlButton = (DualShock)((unsigned)DualShock::L1 + log2(jslButton / JSMASK_L));
            else if (jslButton == JSMASK_LCLICK || jslButton == JSMASK_RCLICK)
                sfmlButton = (DualShock)((unsigned)DualShock::L3 + log2(jslButton / JSMASK_LCLICK));
            else if (jslButton == JSMASK_PS || jslButton == JSMASK_TOUCHPAD_CLICK)
                sfmlButton = (DualShock)((unsigned)DualShock::PS + log2(jslButton / JSMASK_PS));
            else
                cout << "Error: Unknown JSL button (code: " << jslButton << ")." << endl;
            break;
        }

        return sfmlButton;
    }

    sf::Joystick::Axis Joystick::getMovedAxis(const JOY_SHOCK_STATE& state, const JOY_SHOCK_STATE& oldState)
    {
        JOY_SHOCK_STATE s = getRoundedState(state);
        JOY_SHOCK_STATE os = getRoundedState(oldState);

        if (s.stickLX && !os.stickLX)
            return sf::Joystick::X;
        else if (s.stickLY && !os.stickLY)
            return sf::Joystick::Y;
        else if (s.stickRX && !os.stickRX)
            return sf::Joystick::Z;
        else if (s.stickRY && !os.stickRY)
            return sf::Joystick::R;
        else if (s.lTrigger && !os.lTrigger)
            return sf::Joystick::U;
        else if (s.rTrigger && !os.rTrigger)
            return sf::Joystick::V;
        else if (s.buttons & 0xc && !(os.buttons & 0xc))
            return sf::Joystick::PovX;
        else if (s.buttons & 0x3 && !(os.buttons & 0x3))
            return sf::Joystick::PovY;
        return (sf::Joystick::Axis)-1;
    }

    bool Joystick::initVibrations()
    {
        ifstream options(assetsPath + "/Game Data/Options.txt");
        if (!options) {
            cout << "Error: Could not open file + \"" + assetsPath + "/Game Data/Options.txt\"" << endl;
            return 0;
        }

        string line;
        while (!options.eof()) {
            getline(options, line);
            if (line.find("Vibrations") != string::npos) {
                if (line.find(':') == string::npos)
                    break;
                Joystick::rumbleOn = true;
                if (line.find("off") != string::npos)
                    Joystick::rumbleOn = false;
                return true;
            }
        }
        options.close();

        Joystick::rumbleOn = Joystick::isBluetooth;
        return false;
    }
}

/* Fonctions qui rendent compatibles les tests et les
 * soustractions entre les unsigned int (renvoyés par SFML)
 * et les membres de l'enum class DualShock. */
bool operator==(const unsigned& bouton1, const DualShock& bouton2)
{
    return (bouton1 == (unsigned)bouton2);
}

unsigned operator-(const DualShock& bouton1, const DualShock& bouton2)
{
    return (unsigned)bouton1 - (unsigned)bouton2;
}

void emptyEvent(sf::Event& e)
{
    e.type = sf::Event::Count;
    e.joystickButton.button = -1;
    e.joystickButton.joystickId = -1;
}

// JOY_SHOCK_STATE struct operators
bool JOY_SHOCK_STATE::operator!() const
{
    return (!this->buttons && abs(this->lTrigger) < axisDeadZone
        && abs(this->rTrigger) < axisDeadZone && abs(this->stickLX) < axisDeadZone
        && abs(this->stickLY) < axisDeadZone && abs(this->stickRX) < axisDeadZone
        && abs(this->stickRY) < axisDeadZone);
}

static JOY_SHOCK_STATE getRoundedState(const JOY_SHOCK_STATE& s)
{
    JOY_SHOCK_STATE ret = s;

    ret.lTrigger = abs(ret.lTrigger) < axisDeadZone ? 0 : ret.lTrigger;
    ret.rTrigger = abs(ret.rTrigger) < axisDeadZone ? 0 : ret.rTrigger;
    ret.stickLX = abs(ret.stickLX) < axisDeadZone ? 0 : ret.stickLX;
    ret.stickLY = abs(ret.stickLY) < axisDeadZone ? 0 : ret.stickLY;
    ret.stickRX = abs(ret.stickRX) < axisDeadZone ? 0 : ret.stickRX;
    ret.stickRY = abs(ret.stickRY) < axisDeadZone ? 0 : ret.stickRY;

    return ret;
}

bool operator!=(const JOY_SHOCK_STATE& s1, const JOY_SHOCK_STATE& s2)   // Compares only axes, not buttons
{
    JOY_SHOCK_STATE s1c = getRoundedState(s1);
    JOY_SHOCK_STATE s2c = getRoundedState(s2);

    return (s1c.lTrigger != s2c.lTrigger || s1c.rTrigger != s2c.rTrigger
        || s1c.stickLX != s2c.stickLX || s1c.stickLY != s2c.stickLY
        || s1c.stickRX != s2c.stickRX || s1c.stickRY != s2c.stickRY);
}