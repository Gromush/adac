# adac

Arduino small FW code to support DAM 1021 Soekris DAC under Arduino control.

Current version is V1.0.1

Arduino controller version:
- Arduino Nano

DAM1021 version:

HW: rev7
FW: 1.24

Supported actions:
- Power up Arduino without power on DAC (HW support with power supply and relay required)
- Set power On signal usinng digital output to open relays for power on DAM1021 DAC scheme. (digital pin coulld be changewd in defs.h)
- Use one buttons and two led's for filter changing and indication about used mode (HW required - button and led's connected to Arduino)
                                    Settings could be found and changed in defs.h file

- LED monitor should be configured accordingly to your HW (used 2 lines 16 digits simple display using I2C connection)


Files description:
defs.h      - all contants and structures defines - could be used to adapt for used HW.
dacfw.ino   - main scatch with setup and loop functions
button.ino  - used to button action support
display.ino - API for LCD usage
led.ino     - API for led control
monitor.ino - API to use commands of DAC using serial interface
parser.ino  - Serial output parser to provide current state of DAC configuratio and stream played
serial.ino  - API for Serial interface usage



Version 1.0.1
- Button functionality update

Short press - show FW version
First long press (1 second) - set filter mode
        Short press - change filter by cycle
Second long press (1 second) - set input mode
        Short press - change input by cycle
Third long press (1 second) - return to main window

Version 1.0.2
- Automatic return to main mode after 30s idle time from changing modes

Version 1.0.6
- Automatic return changed to 15s
- Ready Led blinking differently when entered to filter and input mode and didn't when in main mode

Small optimization and bug fixes
 - final binary reduced to ~600 bytes.
 - fixed bug with millis() used inside if() condition (Arduino issue periodically return always true)
 - speed optimization (logical) avoid check impossible cases

 Version 1.0.7
 - bug fix with set up input when set to Auto mode has limit to set USB input from the list when USB cable connected.

 Version 1.0.8
 - fix DSD/PCM detection
 
