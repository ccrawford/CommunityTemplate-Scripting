> [!NOTE]  
> This article is for developers who would like to create their own community device implementation. If you are interested in simply using community devices, check out [this page](https://github.com/MobiFlight/MobiFlight-Connector/wiki/Using-a-pre-build-custom-device)

## Quick overview
MobiFlight supports general purpose input (buttons, encoders, potentiometers, etc) and output (7-segment displays, LEDs, etc) "devices" running on Arduino-compatible "boards". Community (aka Custom) devices allow specialty output devices to be included as new devices. A typcial use case would be creating a graphical airspeed indicator for a small LCD screen. MobiFlight client sends the current airspeed, and the Community device draws the airspeed indicator background and needle on the LCD. Currently, only Output devices (devices that receive information *from* the Mobiflight connector) are supported; there is no Community "Input" type.

## Prerequisites
MobiFlight firmware development uses the following free/open source tools:
*  [VSCode](https://code.visualstudio.com/) Microsoft's free IDE
*  [PlatformIO](https://platformio.org) extension to VS code
*  [Python](https://www.python.org/downloads/) as a runtime for scripts
*  [Git client](https://git-scm.com/install/) (useful, but not strictly necessary)
  
## **Create** a new reposority by using the [Community Template repository](https://github.com/MobiFlight/CommunityTemplate)
Clone the repository and open it in PlatformIO.
* Open a Windows shell
* cd to the parent directory for your project
* run ```git clone https://github.com/MobiFlight/CommunityTemplate```
* rename the new directory to something appropriate for your project

## Renaming script to prepare the firmware
* cd into your new directory
* run `python renaming.py` from your terminal and answer the 2 questions (Author and Device name)
* :warning:NOTE: Do not use spaces or special characters. The script does not do any input validation!
* Best practice: Keep both entries short. E.g. use your lastname or initials for 'Author'. Use a short name for 'Device'. You can modify longer descriptions later.
* * Note: Below we will refer to these answers as [author] and [device]
The renaming.py script will rename classes, folders, constants and files and a directory. While it is possible to do this by hand, there are a number of places where strings must perfectly match in order for the project to work with the MobiFlight Connector software. Note that you can change values by hand, but you cannot re-run the connector software as it's looking for templated values. If you make a mistake, the best option is to re-clone the repo and start again.

## Initial Build
Now it's a good point to test everything you have set up. The existing firmware itself will do nothing, but you can confirm that the renaming script worked correctly and that your new community board is recognized by the MF Connector software.
* From the your shell window you can start VS code with ```code .``` or Start VS Code from the Windows menu and open your project directory. 
* Use the PlatformIO build icon at the bottom to compile. If you're new to PlatformIO, documentation is [here](https://docs.platformio.org/en/latest/integration/ide/pioide.html) The Check Mark compiles, while the right arrow compiles and uploads to your board. :star: Pro Tip: PIO will try to auto-detect the board. If you have more than one board connected, click on the Plug icon with the word "Auto" after it and choose the correct COM port from the drop down.
* The first compile will take a while as it pulls in the core MF Firmware repository and installs necessary libraries. Subsequent builds will go much faster. 
* Issues? Look at the compiler output. Possible issues could include spaces or special characters in the author or device name. Or missing libraries/repos. Try closing and reopening VS Code to and rebuild to re-pull libraries if first attempt failed.
* :star:PRO Tip: You can speed up your builds by only building for your current testing board. In the bottom blue PlatformIO status bar, you should see folder-with-target icon followed by "Default ([device name])" Click on that and then select your current board configuation from the top drop-down menu. E.g. "env:[author]\_[device]\_[board]"

## Test with the MF Connector Client
If your build succeeded, you can now try it with the MobiFlight Connector Client
* Copy the contents of the Community folder under your [device] folder to a new directory in the MobiFlight Community directory. By default this foler is '''%LOCALAPPDATA%\MobiFlight\MobiFlight Connector\Community'''
* Name the new folder [device] and copy the three directories: Firmawre, Device, Board under that new directory.
* Start the MobiFlight client. Make sure your "bare" board is connected and there are no active Serial connections to the board.
* The example below is on Version 10 of the MF client using a new Arduino Mega with no firmware or configuration

![image](https://github.com/MobiFlight/MobiFlight-Connector/assets/3263285/7167ecb9-c254-400c-88be-fc5ef5b103b3)

In the list of available firmware there should be an entry which matches `'-DMOBIFLIGHT_TYPE="YourDevice_board"'` in `YourChangedName_platformio.ini`. This should be [author]\_[device]
Choose this entry and your new firmware will be uploaded.
You should now be able to add your custom community device to your flashed board.

![image](https://github.com/MobiFlight/MobiFlight-Connector/assets/3263285/59f292f7-cbb1-4570-b0be-c5a933958e9e)

For each `[author]\_[device].device.json` a list item with `"Type": "[author]\_[device]"` should show up. Choose one of them and check if all pins will show up. If you have not changed these, they will be "Pin 1", "Pin 2", and "Pin 3".

![image](https://github.com/MobiFlight/MobiFlight-Connector/assets/3263285/55d15e50-39ee-4474-a251-61da51754320)

## Troubleshooting
* In general, see if it's an issue with your board or your Community Device code by seeing if your board works with the base MF board firmware. Just flash default firmware and test to narrow down the issue.
* Make sure there are no other programs that have it's COM port open. In Windows, only one program can interact with the COM port at a time.
* Did you copy the Community files into the correct Community directory?
* Are you using an unusual or clone hardware? The USB IDs could be off. Try flashing the basic MF firmware as a test.
* Are you reusing a board? You may want to reset the board first.
* Does your board use one of the knock-off CH340 USB/Serial driver chips? See MF documentation on installing the correct drivers.
* Did you upload bad code and the board is in a reboot loop? Connect to the board and see if there is any serial output.
* Do you have delays or Serial output in your code? These can confuse the client and cause it not to recognize the board as a MF board.

## Implement your community firmware
This is where the fun starts! You've gotten past all of the pre-requisites and connection configuration and you can now write code to implement your custom device.
See all hints in the files. It is also a good idea to check how the examples are set up. The basic GNC255 community device supports an 256x128 OLED, so just one community class is supported. The community device for the FCU and EFIS display from KAV simulation supports five different classes, so it's a good example how to set up two ore more supported devices.

# Implementation tips and tricks
## File organization
* All of your custom code should fall unther the [device] folder of your project. 
* You will mainly be editing the [device].cpp and [device].h files.
* If your project will only support a single hardware type (e.g. RP2040) you may safely delete the other [env.*] sections of your [device]_platformio.ini file, all the extra board.json files, and any unnecessary files in the Community/firmware folder.
* Do NOT edit the base platformio.ini file in the root directory.
* All of the base mobiflight firmware is under the ./src/src directory. You do NOT need to edit any of them, but feel free to browse.
* :star: Pro-tip: Don't forget: Your *.json files are NOT automatically saved when you compile! Don't forget to manually save them before copying to the MF Community directory
* :star: Pro-tip: You can compile and upload to your board directly from PlatformIO. You don't need to compile, move firmware, and upload from the client! However you cannot upload from PIO if MF client is running. Close it first.

## Boards
Boards are the base hardware unit of MobiFlight. They are configured in the [device]/Community/boards/*.json files. 
* "Boards" are the different Arduinos types. Your custom device could support one or multiple board types. The project [device]/Community/boards/*.json files define the board properties.
* If you are only supporting one particular hardware type, you may safely delete all the other board.json files (and the [env.*] sections mentioned above). 
* If you are using unusual hardware with unusual USB VID/PID, set those up in the HardwareIds of the board.
* ESP32 is not yet officially supported, but Ralf has a fork that supports it [here](https://github.com/elral/MF_CommunityTemplate/tree/ESP32_support)
* If your device uses some of the pins, remove them from the "Pins" list in the board.
* Be careful editing other named values. Many of them are linked to values in other files and if the string compare fails your project won't work.
* The "Community" section is not modified by the renaming script. Feel free to hand-update.
* The "Friendly name" can get too long. MF Client expects this to be 16 characters or less. Edit it appropriately.
* Edit "ModuleLimits" appropriately. E.g. if you are using a hard-wired screen, you may want to limit your project to 1 "MaxCustomDevices"

## Devices
Devices are the base MobiFlight connectable element. (Buttons, encoders, 7-segment displays are all "devices") Your custom device(s) are configured in the [device]/Community/devices/*.json files.
* You project can have one or many different device types. The default template comes with two device types. You may safely delete the second device if you are only using one. 
* The default template uses three pins. You may add/delete/rename these. If you aren't using any user-configurable pins, you may delete them, but leave an empty "Pins": [] section in the "Config".
* The "Label" section of "Info" can safely be edited. Keep it under 16 characters, and no special characters.
* the "MessageTypes" section defines the connection between your device and the MobiFlight Connector. The id Numbers here correspond to the value passed in the `set` method of your [device] class in [device].cpp.
* MessageTypes key on the id! If you change the id value, the mobiflight connector does not update and will send the wrong values to your project.
* Stop message (`-1`) - The device should turn off on receiving this message. The message is sent by the Connector any time the MobiFlight execution stops (Stop-button) or the application is shutdown. You should not define this message in your MessageTypes. It's sent by the MF Client.
* PowerSavingMode message (`-2`) - The device should go into a power saving mode (value=1) or wake up from it again (value=0). You should not define this message in your MessageTypes. It's sent by the MF Client.
* :star: pro-tip: The order in the .json file is the order they appear in the MF Connector, not the id #. However, be careful that you don't re-use id numbers. They do not need to be consecutive, just unique.
* :star: The compile step builds a .zip file with with your community files bundled up. However this file will NOT be rebuilt at each compile. You must delete the .\_build and .\_deploy folders to rebuild the .zip

## Coding tips
* Add any necessary libraries to lib_deps in the top [env\_[device]] section of your [device]\_plaformio.ini files.
* There are two main ways to 
* If you need the memory, you can disable device types you won't need/support in the build_unflags section of your [device]\_platformio.ini
* 

# Debugging/Testing
* On a brand new project your custom code will NOT run until you add your new device to the board using the MF Connector. Obvious, but easy to forget.
* Many people find it easier to work out details of screen configuration and core logic in a simple "hello world" project before moving it to the MF Community template.
* You can open a serial port to directly test your device. (115200 baud) The communication protocol is CmdMessenger. The command ids are in the ./src/src/commandmessenger.h file. The general format of the command is ```command_id,command_args;``` No CR/LF.
* To send a message with id 3 with value 4.5 to your first added custom device, from a serial terminal enter: ```32,0,3,4.5;``` Note that it will not echo. Use a semi-colon, not an enter to terminate the command. Other useful comands are ```5;``` quick 'are you alive?' test. ```9;``` get info on board. ```12``` get board config string.
* Be careful with Serial.printf() debugging. They can confuse the MF client. You can use them with serial debugging, but you will need to disable them before resuming testing with the MF client.
* MF Client expects a certain response from your board when it boots. If you have long delays at startup or extraneous serial output this step may fail and your board will not be recognized.


# Further information

## YouTube video series
In early 2026, CACrawf recorded a six part series of screen share videos walking through these steps. The video playlist is [here](https://youtube.com/playlist?list=PLs8cKRa3_buI8JBxz_b9rZXPu8PL7GuUh&si=TwfioTsuSwvoiGhU)

## Discord discussion
The MobiFlight Discord server has a channel devoted to showing off your Community devices and asking questions [here](https://discord.com/channels/608690978081210392/1202389947173052467)


### Special message
There are some special messages with their respective IDs defined:

### Overview how the json files are related
![image](https://github.com/MobiFlight/MobiFlight-Connector/assets/3263285/0123829b-27c1-49ad-96d2-30a751da6e25)

