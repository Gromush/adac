# adac

Arduino small FW code to support DAM 1021 Soekris DAC under Arduino control.

Current version is V1.1.2

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

Version 1.1.0
- Add pin to use analog output to set backlight on 16/2 LCD only when all power up HW finished to avoid power up blinking
- Preliminary released mode for emulation analog indicators as on old amps.
Levels looking like:
L ==========
R =======
All symbols are configurable.
Input signals taken from DAC second outputs with diodes.
One click now used to change mode from normal looking and mode indicators.
: known limitations :
Currently this feature is not support long button pressing to change filters and inputs. Next update will provide the small changes to fix this limitation

Version 1.1.1
- Indicators update

Working with more accurate calculation of value
Has max value label doungraded slowly like was on vintage indicators

Version 1.1.2
- Optimization of loop() main status checking
- Fix long button press issue in indicators mode to avoid entering to other modes without display and changing supported

Version 1.1.3
- Auto mode to change output indicators on when music is detected.
- Several HW changes support for auto mode as required Shotki diodes less working music detection etc.

Version 1.1.4
- Add support for temperature sensor DS18B20 on pin A3 
